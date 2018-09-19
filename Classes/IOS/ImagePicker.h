//
//  ImagePicker.h
//  ZJH
//
//  Created by hyhy on 6/5/14.
//
//

#ifndef __ZJH__ImagePicker__
#define __ZJH__ImagePicker__

#include "cocos2d.h"

class ImagePickerDelegate
{
public:
    virtual void onImageData(char *data, int len) = 0;
};

class ImagePicker : public ImagePickerDelegate
{
public:
    ImagePicker();
    void showImagePicker();
    virtual void onImageData(char *data, int len);
    
    static std::string getAvatarShortPath(int uid);
    static std::string getAvatarPath(int uid);
    static std::string getAvatarFile(int uid);
    
    static std::string getStoragePath(const char *dir);
};

#endif /* defined(__ZJH__ImagePicker__) */
