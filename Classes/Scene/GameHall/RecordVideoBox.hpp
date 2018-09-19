//
//  RecordDegitalLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/16.
//
//

#ifndef RecordVideoBox_hpp
#define RecordVideoBox_hpp

#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  RecordVideoItem : public Layout
{
    
public:
    static RecordVideoItem *createWithData(const Json::Value &info);
    bool initWithData(const Json::Value &info);
    
private:
    
    void postReviewData();
    void onExit();
    Node * mRoot;
    
    int mRoomID;
    int mGameID;
    int mIndex;
    int mPkgId;
    
    Json::Value jsonData;
    
};

class  RecordVideoBox: public Layer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(RecordVideoBox);
    
    void showView( Json::Value& json);
    
private:
    
    Node * mRoot;


};

#endif /* RecordVideoBox_hpp */
