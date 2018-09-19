//
//  HobbyLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/30.
//
//

#include "HobbyLayer.hpp"
bool HobbyLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("喜欢玩什么");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    setMore_txt("确认", [=]
                {
                    //判断选择是否有变化
                    
                    _countStr = "";
                    for (int i = 0; i < hobbyList.size(); i++)
                    {
                        Layout *layout = (Layout *)listView1->getChildByTag(200+i);
                        ImageView *selecImag = (ImageView *)layout->getChildByTag(101);
                        if (selecImag->isVisible())
                        {
                            int id = hobbyList[i]["id"].asInt();
                            char idC[10];
                            sprintf(idC, "%d", id);
                            _countStr = _countStr + idC + ",";
                        }
                    }
                    
                    //请求爱好更改
                    Json::Value json;
                    json["hobby"] = _countStr;
                    
                    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                        
                        if (loginPacket->status != 3)
                        {
                            PlatformHelper::showToast("网络链接失败，请稍后再试");
                            return;
                        }
                        
                        if (loginPacket->resultIsOK())
                        {
                            Json::Value data = loginPacket->recvVal["resultMap"]["user"];
                            log("爱好：＝＝＝ [%s]",data.toStyledString().c_str());
                            
                            ZJHModel::getInstance()->hobby.clear();
                            
                            std::string hobbyData = data["hobby"].asString();
                            const char *hobbyStr = hobbyData.c_str();
                            const char * split = ",";
                            char *p;
                            std::string hobby;
                            
                            p = strtok((char *)hobbyStr, split);
                            while(p!=NULL)
                            {
                                hobby = hobby + p + "  ";
                                ZJHModel::getInstance()->hobby.push_back(p);
                                p = strtok(NULL,split);
                            }
                            
                            __NotificationCenter::getInstance()->postNotification("hobby_change");

                        }
                    },"/account/updAccount",json.toStyledString(),"hobbyChange");
                    
                    this->disappear();
                });
    
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                                                 if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                                                 
                                                 if (loginPacket->status != 3)
                                                 {
                                                     PlatformHelper::showToast("网络链接失败，请稍后再试");
                                                     return;
                                                 }
                                                 
                                                 if (loginPacket->resultIsOK())
                                                 {
                                                     Json::Value data = loginPacket->recvVal["resultMap"]["hobby"];
                                                     hobbyList = data;
                                                     
                                                     log("爱好列表：＝＝＝ [%s]",data.toStyledString().c_str());
                                                     showView(data);
                                                 }
                                             },"account/getAllHobby","","hobbyList");
    
    return true;
}


void HobbyLayer::showView(Json::Value data)
{
    
    auto visibleSize = Director::getInstance()->getWinSizeInPixels();
    float H = visibleSize.height - getHeadH();
    
    listView1 = ListView::create();
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(visibleSize.width, H));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 0));
    listView1->setItemsMargin(0);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    Size cellSize = Size(visibleSize.width, 210);
    for (int i = 0; i < data.size(); i++)
    {
        auto cell = Layout::create();
        cell->setTag(200+i);
        cell->setTouchEnabled(true);
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x14, 0x15, 0x15));
        cell->setContentSize(cellSize);
        cell->addTouchEventListener(CC_CALLBACK_2(HobbyLayer::selectFun, this));
        listView1->addChild(cell);
        
        auto line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->setBackGroundColor(Color3B(0x30, 0x30, 0x31));
        line->setContentSize(Size(visibleSize.width - 40, 3));
        line->setPosition(Vec2(40, 0));
        cell->addChild(line);
        
        Text *left = Text::create(data[i]["hobbyName"].asString(), "", 36);
        left->setAnchorPoint(Vec2(0, 0.5));
        left->setTextColor(Color4B(0xcc, 0xcc, 0xcc, 255));
        left->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        left->setPosition(Vec2(215, cellSize.height/2));
        cell->addChild(left);
        
        //图片
        ImageView *icon = ImageView::create("Match/icon_wanbei.png");
        icon->setContentSize(Size(150, 150));
        icon->setAnchorPoint(Vec2(0, 0.5));
        icon->ignoreContentAdaptWithSize(false);
        icon->setPosition(Vec2(40, cellSize.height/2));
        cell->addChild(icon);
        
        ZJHModel::getInstance()->loadingImageResource(icon, data[i]["logUrl"].asString());
        
        ImageView *seleImage = ImageView::create("Mine/choice_icon_check.png");
        seleImage->setAnchorPoint(Vec2(1, 0.5));
        seleImage->setPosition(Vec2(visibleSize.width - 50, cellSize.height/2));
        cell->addChild(seleImage);
        seleImage->setTag(101);
        
        ImageView *normalImage = ImageView::create("Mine/choice_icon.png");
        normalImage->setAnchorPoint(Vec2(1, 0.5));
        normalImage->setPosition(Vec2(visibleSize.width - 50, cellSize.height/2));
        cell->addChild(normalImage);
        normalImage->setTag(102);
        
        bool isSelect = false;
        for (int j = 0; j < ZJHModel::getInstance()->hobby.size(); j++)
        {
            if (ZJHModel::getInstance()->hobby[j] == data[i]["hobbyName"].asString())
            {
                isSelect = true;
                break;
            }
        }
        if (isSelect)
        {
            seleImage->setVisible(true);
            normalImage->setVisible(false);
        }else
        {
            seleImage->setVisible(false);
            normalImage->setVisible(true);
        }
    }
}

void HobbyLayer::backFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        this->removeFromParent();
    }
}
void HobbyLayer::selectFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Layout *layout = (Layout *)pSender;
        ImageView *selecImag = (ImageView *)layout->getChildByTag(101);
        ImageView *normalImag = (ImageView *)layout->getChildByTag(102);
        selecImag->setVisible(!selecImag->isVisible());
        normalImag->setVisible(!normalImag->isVisible());
    }
}