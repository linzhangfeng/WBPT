//
//  SecKeyWrapper.m
//
/**
 *
 * 模块名: 功能
 * 文件名: SecKeyWrapper.m
 * 相关文件: MettingHttp.m
 * 功能: RSA加密所需的功能类
 * 作者: 产品研发中心---谢叶栋
 * 日期: 2013-8-1
 * 备注:
 * 修改记录：
 * 日期             修改人                修改内容
 *  YYYY/MM/DD      <作者或修改者名>      <修改内容>
 */
#import "SecKeyWrapper.h"
#import <Security/Security.h>
#import "Base64Data.h"

#define MAX_BLOCK_SIZE 117

@implementation SecKeyWrapper

+ (SecKeyRef)addPublicKey:(NSString*)keyName keyString:(NSString*)keyString
{
    NSData* publicKey = [SecKeyWrapper stripPublicKeyHeader:[NSData dataFromBase64String:keyString]];

    return [SecKeyWrapper addPublicKey:keyName keyBits:publicKey];

}

+ (NSData *)stripPublicKeyHeader:(NSData *)keyData
{
    // Skip ASN.1 public key header
    if (0 == [keyData length])
    {
        return nil;
    }

    unsigned char *keyBytes = (unsigned char *)[keyData bytes];
    unsigned int  index    = 0;

    if (keyBytes[index++] != 0x30) return nil;

    if (keyBytes[index] > 0x80) index += keyBytes[index] - 0x80 + 1;
    else index++;

    // PKCS #1 rsaEncryption szOID_RSA_RSA
    static unsigned char seqiod[] =
    { 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
        0x01, 0x05, 0x00 };
    if (memcmp(&keyBytes[index], seqiod, 15)) return nil;

    index += 15;

    if (keyBytes[index++] != 0x03) return nil;

    if (keyBytes[index] > 0x80) index += keyBytes[index] - 0x80 + 1;
    else index++;

    if (keyBytes[index++] != '\0') return nil;

    // Now make a new NSData from this buffer
    return([NSData dataWithBytes:&keyBytes[index] length:[keyData length] - index]);
}

+ (SecKeyRef)addPublicKey:(NSString *)keyName keyBits:(NSData *)publicKey
{
    OSStatus sanityCheck = noErr;
    SecKeyRef peerKeyRef = NULL;
    CFTypeRef persistPeer = NULL;
    NSAssert( keyName != nil, @"Key name parameter is nil." );
    NSAssert( publicKey != nil, @"Public key parameter is nil." );

    NSData * keyTag = [[NSData alloc] initWithBytes:(const void *)[keyName UTF8String] length:[keyName length]];
    NSMutableDictionary * peerPublicKeyAttr = [[NSMutableDictionary alloc] init];

    [peerPublicKeyAttr setObject:(__bridge id)kSecClassKey forKey:(__bridge id)kSecClass];
    [peerPublicKeyAttr setObject:(__bridge id)kSecAttrKeyTypeRSA forKey:(__bridge id)kSecAttrKeyType];
    [peerPublicKeyAttr setObject:keyTag forKey:(__bridge id)kSecAttrApplicationTag];
    [peerPublicKeyAttr setObject:publicKey forKey:(__bridge id)kSecValueData];
    [peerPublicKeyAttr setObject:[NSNumber numberWithBool:YES] forKey:(__bridge id)kSecReturnPersistentRef];

    sanityCheck = SecItemAdd((__bridge CFDictionaryRef) peerPublicKeyAttr, (CFTypeRef *)&persistPeer);

//    LOGGING_FACILITY1( sanityCheck == noErr || sanityCheck == errSecDuplicateItem, @"Problem adding the app public key to the keychain, OSStatus == %ld.", sanityCheck );

    if (persistPeer)
    {
        peerKeyRef = [SecKeyWrapper getKeyRefWithPersistentKeyRef:persistPeer];
    }
    else
    {
        [peerPublicKeyAttr removeObjectForKey:(__bridge id)kSecValueData];
        [peerPublicKeyAttr setObject:[NSNumber numberWithBool:YES] forKey:(__bridge id)kSecReturnRef];
        // Let's retry a different way.
        sanityCheck = SecItemCopyMatching((__bridge CFDictionaryRef) peerPublicKeyAttr, (CFTypeRef *)&peerKeyRef);
    }

//    LOGGING_FACILITY1( sanityCheck == noErr && peerKeyRef != NULL, @"Problem acquiring reference to the public key, OSStatus == %ld.", sanityCheck );

    if (persistPeer)
    CFRelease(persistPeer);
    return peerKeyRef;
}

+ (SecKeyRef)getKeyRefWithPersistentKeyRef:(CFTypeRef)persistentRef
{
    OSStatus sanityCheck = noErr;
    SecKeyRef keyRef = NULL;

//    LOGGING_FACILITY(persistentRef != NULL, @"persistentRef object cannot be NULL." );

    NSMutableDictionary * queryKey = [[NSMutableDictionary alloc] init];

    // Set the SecKeyRef query dictionary.
    [queryKey setObject:(__bridge id)persistentRef forKey:(__bridge id)kSecValuePersistentRef];
    [queryKey setObject:[NSNumber numberWithBool:YES] forKey:(__bridge id)kSecReturnRef];

    // Get the persistent key reference.
    sanityCheck = SecItemCopyMatching((__bridge CFDictionaryRef)queryKey, (CFTypeRef *)&keyRef);

    return keyRef;
}

+ (BOOL)verifySignature:(NSData *)plainText secKeyRef:(SecKeyRef)publicKey signature:(NSData *)sig
{
    size_t signedHashBytesSize = 0;
    OSStatus sanityCheck = noErr;

    // Get the size of the assymetric block.
    signedHashBytesSize = SecKeyGetBlockSize(publicKey);
    sanityCheck = SecKeyRawVerify(    publicKey,
                                  kTypeOfSigPadding,
                                  (const uint8_t *)[[SecKeyWrapper getHashBytes:plainText] bytes],
                                  kChosenDigestLength,
                                  (const uint8_t *)[sig bytes],
                                  signedHashBytesSize
                                  );

    return (sanityCheck == noErr) ? YES : NO;
}


+ (NSData*)encrypt:(NSData*)plainText publicKey:(NSData*)key
{
    NSString* keyName = @"MeetingPublicKey";
    OSStatus sanityCheck = noErr;
    SecKeyRef publicKeyRef = NULL;
    NSMutableData* cipherData = [NSMutableData data];

    publicKeyRef = [SecKeyWrapper addPublicKey:keyName keyBits:[SecKeyWrapper stripPublicKeyHeader:key]];
    if (NULL == publicKeyRef)
    {
        return nil;
    }

    size_t cipherLen = SecKeyGetBlockSize(publicKeyRef);
    void *blockBuf = malloc(sizeof(uint8_t) * MAX_BLOCK_SIZE);
    void *cipherTextBuf = malloc(sizeof(uint8_t) * cipherLen);
    int plainTextLen = [plainText length];

    for (int i = 0 ; i < plainTextLen; i += MAX_BLOCK_SIZE) {
        int blockSize = MIN(MAX_BLOCK_SIZE, plainTextLen - i);
        memset(blockBuf, 0, MAX_BLOCK_SIZE);
        memset(cipherTextBuf, 0, cipherLen);
        [plainText getBytes:blockBuf range:NSMakeRange(i, blockSize)];
        sanityCheck = SecKeyEncrypt(publicKeyRef,
                                    kSecPaddingPKCS1,//修改了加密类型的值以后可正常加密
                                    blockBuf,
                                    blockSize,
                                    cipherTextBuf,
                                    &cipherLen);

        if(sanityCheck == noErr) {
            [cipherData appendBytes:cipherTextBuf length:cipherLen];
        } else {
            cipherData = nil;
            break;
        }
    }

    free(blockBuf);
    free(cipherTextBuf);
    CFRelease(publicKeyRef);
    [SecKeyWrapper removePublicKey:keyName];

    return cipherData;
}



+ (NSData *)getHashBytes:(NSData *)plainText
{
    CC_SHA1_CTX ctx;
    uint8_t * hashBytes = NULL;
    NSData * hash = nil;

    // Malloc a buffer to hold hash.
    hashBytes = malloc( kChosenDigestLength * sizeof(uint8_t) );
    memset((void *)hashBytes, 0x0, kChosenDigestLength);

    // Initialize the context.
    CC_SHA1_Init(&ctx);
    // Perform the hash.
    CC_SHA1_Update(&ctx, (void *)[plainText bytes], [plainText length]);
    // Finalize the output.
    CC_SHA1_Final(hashBytes, &ctx);

    // Build up the SHA1 hash.
    hash = [NSData dataWithBytes:(const void *)hashBytes length:(NSUInteger)kChosenDigestLength];

    if (hashBytes) free(hashBytes);

    return hash;
}

+ (void)removePublicKey:(NSString *)keyName
{
    OSStatus sanityCheck = noErr;

    NSAssert( keyName != nil, @"Peer name parameter is nil." );

    NSData * peerTag = [[NSData alloc] initWithBytes:(const void *)[keyName UTF8String] length:[keyName length]];
    NSMutableDictionary * peerPublicKeyAttr = [[NSMutableDictionary alloc] init];

    [peerPublicKeyAttr setObject:(__bridge id)kSecClassKey forKey:(__bridge id)kSecClass];
    [peerPublicKeyAttr setObject:(__bridge id)kSecAttrKeyTypeRSA forKey:(__bridge id)kSecAttrKeyType];
    [peerPublicKeyAttr setObject:peerTag forKey:(__bridge id)kSecAttrApplicationTag];

    sanityCheck = SecItemDelete((__bridge CFDictionaryRef) peerPublicKeyAttr);

//    LOGGING_FACILITY1( sanityCheck == noErr || sanityCheck == errSecItemNotFound, @"Problem deleting the peer public key to the keychain, OSStatus == %ld.", sanityCheck );

}

@end
