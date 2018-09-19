//
//  ImagePicker.cpp
//  ZJH
//
//  Created by hyhy on 6/5/14.
//
//

#include "ImagePicker.h"
#include "ZJHModel.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ImagePickerIOS.h"
#include <sys/stat.h>
#endif

using namespace cocos2d;

///*******
//Image *createCustomCCImage(char *data, int len)
//{
//    Image *image = new Image();
//    if (image && image->initWithImageData(data, len)) {
//        image->autorelease();
//        return image;
//    }
//    CC_SAFE_DELETE(image);
//    return NULL;
//}
//
//Texture2D *createCustomCCTexture2D(char *data, int len)
//{
//    Texture2D *texture = new Texture2D();
//    if (texture && texture->initWithImage(createCustomCCImage(data, len))) {
//        texture->autorelease();
//        return texture;
//    }
//    CC_SAFE_DELETE(texture);
//    return NULL;
//}

ImagePicker::ImagePicker()
{
    
}

void ImagePicker::showImagePicker()
{
    float winWindth = Director::getInstance()->getWinSize().width * 0.5;
#ifdef __ZJH__ImagePickerIOS__
    [ImagePickerIOS showImagePicker: winWindth].imagePickerDelegate = this;
#endif
}

void ImagePicker::onImageData(char *data, int len)
{
    if (len == 0) {
        return;
    }
    
    std::string avatarPath = getAvatarPath(ZJHModel::getInstance()->uid);
    FILE *fp = fopen(avatarPath.c_str(), "wb+");
    if (fp) {
        fwrite(data, len, 1, fp);
        fclose(fp);
    }
    CCLOG("onImageData avatarPath=%s", avatarPath.c_str());
}


void makeMultiDir(const char *mulDir)
{
    int i, len;
    char str[512];
    
    strncpy(str, mulDir, 512);
    len = strlen(str);
    for (i = 0; i < len; i++) {
        if (str[i] == '/') {
            str[i] = '\0';
            if (access(str, F_OK) != 0) {
                mkdir(str, S_IRWXU | S_IRWXG | S_IRWXO);
            }
            str[i] = '/';
        }
    }
    if ((len > 0) && (access(str, F_OK) != 0)) {
        mkdir(str, S_IRWXU | S_IRWXG | S_IRWXO);
    }
}

std::string ImagePicker::getAvatarShortPath(int uid)
{
    //获取头像在服务器上的存储路径
//    char buffer[512];
//    snprintf(buffer, sizeof(buffer), "image_%d.png", uid);
//    std::string pathString = buffer;
//    std::string md5String;
//    MD5::MD5Str(pathString, md5String);
//    if (!md5String.empty()) {
//        snprintf(buffer, sizeof(buffer), "%c%c/%c%c/image_%d.png", md5String.at(0), md5String.at(1), md5String.at(2), md5String.at(3), uid);
//        pathString = buffer;
//    } else {
//        pathString = "";
//    }
//    CCLOG("avator file storage short path:%s", pathString.c_str());
//    return pathString;
    return Utils::getAvatarUri(uid);
}

std::string ImagePicker::getAvatarPath(int uid)
{
//    char buffer[512];
//    snprintf(buffer, sizeof(buffer), "image_%d.png", uid); ///服务器上存储为png格式，为了保持一致
//    std::string pathString = buffer;
//    std::string md5String;
//    MD5::MD5Str(pathString, md5String);
//    if (!md5String.empty()) {
//        //std::string dir = getStoragePath("ihuizhi/zjh/avatar/");
//        std::string dir = getStoragePath("Assets/Avatars/");
//        
//        snprintf(buffer, sizeof(buffer), "%s%c%c/%c%c", dir.c_str(), md5String.at(0), md5String.at(1), md5String.at(2), md5String.at(3));
//        pathString = buffer;
//        if (access(pathString.c_str(), F_OK)) {
//            makeMultiDir(pathString.c_str());
//        }
//        //snprintf(buffer, sizeof(buffer), "%s/image_%d.bin", pathString.c_str(), uid); ///本地存储为bin格式
//        snprintf(buffer, sizeof(buffer), "%s/image_%d.png", pathString.c_str(), uid); ///本地存储为bin格式
//        pathString = buffer;
//    } else {
//        pathString = "";
//    }
//    CCLOG("avator file storage path:%s", pathString.c_str());
//    return pathString;
    
    std::string pathString = Utils::getAvatarSavePath(uid);
    ///如果文件夹不存在，需要创建文件夹
    unsigned int pos = pathString.rfind("/image_");
    std::string dirString = pathString.substr(0, pos);
    if (access(dirString.c_str(), F_OK)) {
        makeMultiDir(dirString.c_str());
    }
    return pathString;
}

std::string ImagePicker::getAvatarFile(int uid)
{
    std::string filePath = "noFile";
    std::string avatarPath = getAvatarPath(uid);
    if (!avatarPath.empty()) {
        if (access(avatarPath.c_str(), 0) == 0) {
            filePath = avatarPath;
        }
    }
    return filePath;
}

std::string ImagePicker::getStoragePath(const char *dir)
{
    std::string writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
    cocos2d::FileUtils::getInstance()->getWritablePath();
    writablePath.append(dir);
    return writablePath;
}