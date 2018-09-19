#ifndef _MD5_H_
#define _MD5_H_

#include <fstream>
#include <iostream>

using namespace std;

//#define uint8_t unsigned char
//#define uint32_t unsigned int

typedef struct MD5Context {
    uint32_t buf[4];
    uint32_t bytes[2];
    uint32_t in[16];
} MD5_CTX;

#define MD5_DIGEST_LENGTH 16

class MD5
{
public:
    MD5();
    ~MD5();
public:
    static void MD5Init(struct MD5Context *context);
    static void MD5Update(struct MD5Context *context, const void *buf, unsigned len);
    static void MD5Final(uint8_t digest[16], struct MD5Context *context);
    static void MD5Transform(uint32_t buf[4], uint32_t const in[16]);
    static int MD5Cal(string file_name, string &md5);
    static int MD5Cal(string file_name, long long size, string &md5);
    static void MD5Str(string str, string &md5);
};

#ifdef WORDS_BIGENDIAN
void
byteSwap(uint32_t * buf, unsigned words)
{
    uint8_t *p = (uint8_t *) buf;

    do {
        *buf++ = (uint32_t) ((unsigned) p[3] << 8 | p[2]) << 16 |
            ((unsigned) p[1] << 8 | p[0]);
        p += 4;
    } while (--words);
}
#else
#define byteSwap(buf,words)
#endif

#endif

