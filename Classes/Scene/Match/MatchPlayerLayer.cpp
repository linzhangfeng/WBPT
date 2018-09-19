//
//  MatchPlayerLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/15.
//
//

#include "MatchPlayerLayer.hpp"
#include "MatchItem.hpp"
MatchPlayerLayer::MatchPlayerLayer()
{
    viewList = NULL;
    curLayout = NULL;
}
bool MatchPlayerLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("对战玩家");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    Size winSize = Director::getInstance()->getWinSize();
    
    createBg = Layout::create();
    createBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    createBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    createBg->setContentSize(Size(winSize.width, 210));
    this->addChild(createBg);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 210));
    line->setContentSize(Size(winSize.width, 3));
    createBg->addChild(line);
    
    Button *createBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png","Public/btn_yellow.png");
    createBtn->setScale9Enabled(true);
    createBtn->setContentSize(Size(960, 115));
    createBtn->setPosition(Vec2(winSize.width/2, 105));
    createBg->addChild(createBtn);
    createBtn->setTitleText("一键加为好友");
    createBtn->setTitleFontSize(40);
    createBtn->setTitleColor(Color3B(0x65, 0x41, 0x00));
    createBtn->setTitleFontName("");
    createBtn->addClickEventListener([=](Ref*){
        
        createBtn->setEnabled(false);
        
        if(frindString == "")
        {
            return;
        }
        Json::Value json;
        json["friendIds"] = frindString;
        
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            createBtn->setEnabled(true);
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                PlatformHelper::showToast("一键添加好友成功");
                Json::Value data = loginPacket->recvVal["resultMap"]["playerFriendList"];
                __NotificationCenter::getInstance()->postNotification("addAllFriends_refesh", __Integer::create(data.size()));
                
                showView(data);
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
            
        },"friend/fastAddFriend",json.toStyledString(),"fastAddFriend");
    });
    
    //发送请求数据
    postData();
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(MatchPlayerLayer::canDelete), "player_add_finish", NULL);
    
    return true;
}

void MatchPlayerLayer::canDelete(Ref*ref)
{
    __Integer *userId = (__Integer*)ref;
    int ID = userId->getValue();
    char buf[128];
    sprintf(buf, "%d", ID);
    Layout *item = (Layout *)viewList->cocos2d::Node::getChildByName(buf);
    if (item != NULL)
    {
        item->setTouchEnabled(false);
    }
    
}
void  MatchPlayerLayer::postData()
{
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
            Json::Value data = loginPacket->recvVal["resultMap"]["playerFriendList"];
            showView(data);
        }
        
    },"duiju/integral/playerFriendList","","playerFriendList");
    
}

void MatchPlayerLayer::showView(Json::Value list)
{
    listData = list;
    Size winSize = Director::getInstance()->getWinSize();
    float H = winSize.height - getHeadH() - 210;
    if(viewList != NULL)
    {
        viewList->removeFromParent();
    }
    viewList = ListView::create();
    viewList->ignoreAnchorPointForPosition(false);
    viewList->setDirection(ScrollView::Direction::VERTICAL);
    viewList->setBounceEnabled(false);
    viewList->setContentSize(Size(winSize.width, H));
    viewList->setAnchorPoint(Vec2(0, 0));
    viewList->setPosition(Vec2(0, 210));
    viewList->setItemsMargin(0);
    viewList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(viewList);
    
    int nums = list.size();
    if (nums == 0)
    {
        createBg->setVisible(false);
        Text *text = Text::create("暂无对战玩家", "", 36);
        text->setPosition(Vec2(winSize.width/2, H/2));
        text->setTextColor(Color4B::GRAY);
        this->addChild(text);
        return;
    }
    //创建item
    char buf[128];
    
    Size itemSize = Size(winSize.width, 180);
    for (int i = 0; i < nums; i++)
    {
        sprintf(buf, "%d", list[i]["userId"].asInt());
        
        Layout *layoutItem = Layout::create();
        layoutItem->setTag(i);
        layoutItem->setName(buf);
        string temp = buf;
        frindString += temp + ",";
        
        layoutItem->ignoreAnchorPointForPosition(false);
        layoutItem->setTouchEnabled(true);
        //layoutItem->addTouchEventListener(CC_CALLBACK_2(MatchPlayerLayer::clickItemMove, this));
        
        layoutItem->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                      {
                                          if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
                                          {
                                              Layout* n = (Layout*)sender;
                                              
                                              if (curLayout && n != curLayout)
                                              {
                                                  curLayout->setPositionX(0);
                                                  curLayout = NULL;
                                              }
                                              
                                              Vec2 dvec2 = n->getTouchBeganPosition() - n->getTouchEndPosition();
                                              if (dvec2.length() < 30 && type == Widget::TouchEventType::ENDED)
                                              {
                                                  if (curLayout)
                                                  {
                                                      curLayout->setPositionX(0);
                                                      curLayout = NULL;
                                                  }
                                              }
                                              else
                                              {
                                                  if(dvec2.x > 0)
                                                  {
                                                      if (abs(dvec2.y) > abs(dvec2.x) || curLayout == n)return;
                                                      
                                                      n->setPositionX(-160);
                                                      
                                                      this->curLayout = n;
                                                      
                                                      index = n->getTag();
                                                      MatchPlayerItem *playerItem = (MatchPlayerItem *)n->getChildByTag(100);
                                                      Layout *delet = playerItem->getDeleteLayout();
                                                      delet->addTouchEventListener(CC_CALLBACK_2(MatchPlayerLayer::deleteOn, this));
                                                  }
                                                  else
                                                  {
                                                      if (curLayout)
                                                      {
                                                          curLayout->setPositionX(0);
                                                          curLayout = NULL;
                                                      }
                                                  }
                                              }
                                          }
                                      });

        
        layoutItem->setContentSize(itemSize);
        layoutItem->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        layoutItem->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
        
        MatchPlayerItem *item = MatchPlayerItem::createWithData(list[i]);
        item->ignoreAnchorPointForPosition(false);
        item->setAnchorPoint(Vec2(0, 0));
        item->setPosition(Vec2(0, 0));
        item->setTouchEnabled(true);
        item->setSwallowTouches(false);
        item->setTag(100);
        layoutItem->addChild(item);
        viewList ->addChild(layoutItem);
    }
    
}

void MatchPlayerLayer::clickItemMove(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::BEGAN)
    {
        isDelete = !isDelete;
        Layout *item = (Layout *)pSender;
        index = item->getTag();
        MatchPlayerItem *playerItem = (MatchPlayerItem *)item->getChildByTag(100);
        Layout *delet = playerItem->getDeleteLayout();
        MoveTo* actionBy;
        if (isDelete)
        {
            _deleteItem = item;
            actionBy = MoveTo::create(0.1, Vec2(-180,item->getPositionY()));
            delet->addTouchEventListener(CC_CALLBACK_2(MatchPlayerLayer::deleteOn, this));
            
        }else
        {
            actionBy = MoveTo::create(0.1, Vec2(0,item->getPositionY()));
        }
        
        item->runAction( Sequence::create(actionBy, nullptr));
    }
}

void MatchPlayerLayer::deleteOn(Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Json::Value json;
        json["userId"] = listData[index]["userId"];
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
                __NotificationCenter::getInstance()->postNotification("player_Num_change");
                curLayout->removeFromParent();
                viewList->refreshView();
                
            }else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
            
        },"duiju/integral/ignoreFriend",json.toStyledString(),"ignore");
        
        
    }
}