//
//  MineMoreLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#include "MineMoreLayer.hpp"
bool MineMoreLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("更多");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));

    auto visibleSize = Director::getInstance()->getWinSizeInPixels();
    float H = visibleSize.height - getHeadH();
    
    ListView *listView1 = ListView::create();
    listView1->setInertiaScrollEnabled(false);
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(visibleSize.width, H));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 0));
    listView1->setItemsMargin(30);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    Size cellSize = Size(visibleSize.width, 130);
    for (int i = 0; i < 4; i++)
    {
        if (ZJHModel::getInstance()->isOpenIPV6() && i == 1)
        {
            continue;
        }
        
        auto cell = Layout::create();
        cell->setTag(200+i);
        cell->setTouchEnabled(true);
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setContentSize(cellSize);
        listView1->addChild(cell);
        cell->addTouchEventListener(CC_CALLBACK_2(MineMoreLayer::selectFun, this));
        
        Text *left = Text::create("", "", 40);
        left->setAnchorPoint(Vec2(0, 0.5));
        left->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
        left->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        left->setPosition(Vec2(140, cellSize.height/2));
        cell->addChild(left);

        ImageView *arrow = ImageView::create("Public/detail_arrow.png");
        arrow->setAnchorPoint(Vec2(1, 0.5));
        arrow->setPosition(Vec2(cellSize.width - 40, cellSize.height/2));
        cell->addChild(arrow);
        
        Layout *line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->setBackGroundColor(Color3B(0, 0, 0));
        line->setPosition(Vec2(0, 0));
        line->setContentSize(Size(cellSize.width, 2));
        cell->addChild(line);
        
        ImageView *icon = ImageView::create();
        icon->setAnchorPoint(Vec2(0, 0.5));
        icon->setPosition(Vec2(40, cellSize.height/2));
        cell->addChild(icon);
        
        Text *right;
        switch (i)
        {
            case 0:
                left->setString("意见反馈");
                icon->loadTexture("Mine/i_yiJian.png");
                break;
            case 1:
                left->setString("检查更新");
                icon->loadTexture("Mine/i_updateIcon.png");
                
                right = Text::create("", "", 40);
                right->setAnchorPoint(Vec2(1, 0.5));
                right->setTextColor(Color4B(0x5a, 0x5a, 0x5a, 255));
                right->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
                right->setPosition(Vec2(cellSize.width - 80, cellSize.height/2));
                cell->addChild(right);
                
                break;

            case 2:
                left->setString("清理缓存");
                icon->loadTexture("Mine/i_qingLi.png");
                break;

            case 3:
                left->setString("关于我们");
                icon->loadTexture("Mine/i_aboutIcon.png");
                break;

            default:
                break;
        }

    }
    return true;
}

void MineMoreLayer::selectFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout *layout = (Layout *)pSender;
        int tag = layout->getTag();
        switch (tag)
        {
            case 200:
                ZJHModel::getInstance()->gotoView(VIEW_FEEDBACK);
                break;
            case 203:
                ZJHModel::getInstance()->gotoView(VIEW_ABOUT_OUR);
                break;
            case 202:
                PlatformHelper::showToast("清除完毕！");
                break;
            case 201:
                postCheckVern();
                
                break;
            default:
                break;
        }
        
    }
}

void MineMoreLayer::postCheckVern()
{
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
                                                     if(loginPacket->recvVal["resultMap"]["version"]["code"].isString() && !loginPacket->recvVal["resultMap"]["version"]["code"].isNull())
                                                     {
                                                         int curCode = atoi(loginPacket->recvVal["resultMap"]["version"]["code"].asCString());
                                                         if (curCode > ZJHModel::getInstance()->vcode)
                                                         {
                                                             PlatformHelper::showUpdateDialog(loginPacket->recvVal["resultMap"]["version"]);
                                                         }
                                                         else
                                                         {
                                                             PlatformHelper::showToast("已经是最新版本");
                                                         }
                                                     }else
                                                     {
                                                          PlatformHelper::showToast("已经是最新版本");
                                                     }
                                                 }else
                                                 {
                                                     if(loginPacket->recvVal["message"].isString() && !loginPacket->recvVal["message"].isNull())
                                                     {
                                                         PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                                                     }else
                                                     {
                                                         PlatformHelper::showToast("已经是最新版本");
                                                     }
                                                     
                                                 }
                                             },"version/clientGameVersion", "","version");
}
