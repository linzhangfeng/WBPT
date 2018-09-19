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

class ImageHelper
{
public:
    static void openCamera();
    static void openPhotoLibrary();
    static void openSavedPhotosAlbum();
};

#endif /* defined(__ZJH__ImagePicker__) */
