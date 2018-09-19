

#ifndef RecordDetailsBox_hpp
#define RecordDetailsBox_hpp
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;
class RecordDetailsBox;
class RecordDetailsItem : public cocos2d::ui::Layout
{
    
public:
    static RecordDetailsItem *createWithData(const Json::Value &info, int rate,int bigWinnerScore);
    bool initWithData(const Json::Value &info, int rate,int bigWinnerScore);
    
    Node * mRoot;
    RecordDetailsBox* mParent;

};


class  RecordDetailsBox: public Layer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(RecordDetailsBox);
    
    void show(const Json::Value& json);
    void getData(float dt);
    void showView(const Json::Value& json);
    
    void gotoRecordVideo();
    
    void showVideo(){mShowVideo =true;}
private:

    int num;
    bool mShowVideo;
    
    Node * mRoot;
    
    Json::Value mJsonData;
    int mRoomID;
    int mGameType;
    
    
    void shareClick(const string& key );

};

#endif /* RecordDetailsBox_hpp */
