
#ifndef RecordBox_hpp
#define RecordBox_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class RecordBox;
class RecordItem : public cocos2d::ui::Layout
{
public:
    static RecordItem *createWithData(const Json::Value &info);
    bool initWithData(const Json::Value &info);
    void dealTimeFormate(long long sec, Text *text1);
    
    Node * mRoot;
    RecordBox* mParent;
};



class  RecordBox: public Layer
{
    
public:
    virtual bool init();
    CREATE_FUNC(RecordBox);
    
    
    void gotoRecordDetail(int index);

private:
    
    std::vector<Json::Value> mRecordJson;
    int mPageIndex;
    int mRecordIndex;
    bool mHaveMore;
    
    Node * mRoot;
    Loading *mLoading;
    
    void getDataMore(float);
    void getData(float);
    void sendRecord();
    void showRecordList(const Json::Value& json);
    void addDataInList(const Json::Value& json);
    

};


#endif 