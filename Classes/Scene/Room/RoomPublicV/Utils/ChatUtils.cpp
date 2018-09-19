//
//  ChatUtils.cpp
//  ZJH
//
//  Created by mac on 2016/12/9.
//
//

#include "ChatUtils.hpp"

#define ss 0.67

static int getFaceIndex_roomchat(std::string faceText)
{
    int result = -1;
    if (faceText == "[呵呵]") result = 0;
    else if (faceText == "[嘻嘻]") result = 1;
    else if (faceText == "[哈哈]") result = 2;
    else if (faceText == "[晕]") result = 3;
    else if (faceText == "[泪]") result = 4;
    else if (faceText == "[馋嘴]") result = 5;
    else if (faceText == "[抓狂]") result = 6;
    else if (faceText == "[哼]") result = 7;
    else if (faceText == "[可爱]") result = 8;
    else if (faceText == "[怒]") result = 9;
    else if (faceText == "[困]") result = 10;
    else if (faceText == "[汗]") result = 11;
    else if (faceText == "[睡觉]") result = 12;
    else if (faceText == "[偷笑]") result = 13;
    else if (faceText == "[吃惊]") result = 15;
    else if (faceText == "[闭嘴]") result = 16;
    else if (faceText == "[花心]") result = 17;
    else if (faceText == "[失望]") result = 18;
    else if (faceText == "[生病]") result = 19;
    else if (faceText == "[亲亲]") result = 20;
    else if (faceText == "[右哼哼]") result = 21;
    else if (faceText == "[嘘]") result = 22;
    else if (faceText == "[挤眼]") result = 23;
    else if (faceText == "[酷]") result = 24;
    else if (faceText == "[感冒]") result = 25;
    else if (faceText == "[做鬼脸]") result = 26;
    else if (faceText == "[阴险]") result = 27;
    else if (faceText == "[热吻]") result = 28;
    else if (faceText == "[心]") result = 29;
    else if (faceText == "[ok]") result = 30;
    else if (faceText == "[不要]") result = 31;
    else if (faceText == "[弱]") result = 32;
    else if (faceText == "[good]") result = 33;
    else if (faceText == "[拳头]") result = 34;
    else if (faceText == "[耶]") result = 35;
    else if (faceText == "[0]") result = 36;
    else if (faceText == "[1]") result = 37;
    else if (faceText == "[2]") result = 38;
    else if (faceText == "[3]") result = 39;
    else if (faceText == "[4]") result = 40;
    else if (faceText == "[5]") result = 41;
    else if (faceText == "[6]") result = 42;
    else if (faceText == "[7]") result = 43;
    else if (faceText == "[8]") result = 44;
    else if (faceText == "[9]") result = 45;
    return result;
}

void ChatUtils::componentsSeparatedByString_roomchat(std::string str, ui::RichText *_richText, int fontSize, cocos2d::Color3B color){
    __Array* result = __Array::create();
    const char* str_tmp = str.c_str();
    int l = strlen(str_tmp);
    int beginIndex = -1;
    int lastIndex = 0;
    for(int i = 0; i < l;i++)
    {
        if(str_tmp[i] == '[')
        {
            beginIndex = i;
        }
        else if (str_tmp[i] == ']')
        {
            if (beginIndex != -1)
            {
                if (beginIndex - lastIndex > 0)result->addObject(__String::create(str.substr(lastIndex, beginIndex - lastIndex)));
                
                if (i + 1 - beginIndex > 0)result->addObject(__String::create(str.substr(beginIndex, i + 1 - beginIndex)));
                lastIndex = i + 1;
                beginIndex = -1;
            }
        }
    }
    
    if (lastIndex < l)
    {
        result->addObject(__String::create(str.substr(lastIndex, l - lastIndex)));
    }
    
    int label_count=(int)result->count();
    for (int i=0; i< label_count; i++)
    {
        __String * _string = (__String*)result->getObjectAtIndex(i);
        //        log("%s",_string->getCString());
        std::string s = _string->getCString();
        int faceIndex = getFaceIndex_roomchat(s);
        if (faceIndex != -1)
        {
            RichElementImage * reimg = RichElementImage::create(i, Color3B::WHITE, 255,__String::createWithFormat("Chat/face/tt_e%d.png",faceIndex)->getCString());
            reimg->setWidth(fontSize+20);
            reimg->setHeight(fontSize+20);
            _richText->pushBackElement(reimg);
        }
        else
        {
            auto _element_text = RichElementText::create(i, color, 255, s.c_str(), "AmericanTypewriter", fontSize);
            _richText->pushBackElement(_element_text);
        }
        
    }
}

void ChatUtils::componentsSeparatedByString_roomchat(std::string str,ui::RichText* _richText){
    componentsSeparatedByString_roomchat(str, _richText, 35, Color3B(154,154,154));
}
