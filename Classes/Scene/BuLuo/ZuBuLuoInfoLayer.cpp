//
//  MineMoreLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#include "ZuBuLuoInfoLayer.hpp"
#include "../Mine/MaskSelectLayer.hpp"
#include "../Mine/TipChangeEditLayer.hpp"
#include "../Loading/Loading.h"

bool ZuBuLuoInfoLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setTitle("俱乐部信息");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));

    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ZuBuLuoInfoLayer::updateName), "buluo_name_change", NULL);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ZuBuLuoInfoLayer::updatePortrait), "image_notify", NULL);
    
    return true;
}

void ZuBuLuoInfoLayer::updatePortrait(Ref*ref)
{
    
    
    auto size = Director::getInstance()->getVisibleSize();
    Loading *loading = Loading::startLoading(Director::getInstance()->getRunningScene(), size / 2);
    loading->maskSwllowTouch();
    //头像
    __String* str = (__String*)ref;
    
    
    if (str != NULL)
    {
        std::string s = str->getCString();
        
        //post image to server
        CCHttpAgent::getInstance()->sendHttpImagePost("image/uploadTribeImage", s, "buluoPortrit",[=](std::string tag){
            
            loading->removeFromParent();
            
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            CCHttpAgent::getInstance()->packets.erase(tag);
            CCHttpAgent::getInstance()->callbacks.erase(tag);
            
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("网络链接失败，请稍后再试");
                return;
            }
            
            loginPacket->status = 0;
            
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"];
                //imageUrl thumbUrl
                
                std::string userImage = data["thumbUrl"].asString();

                Data d = FileUtils::getInstance()->getDataFromFile(s);
                
                std::string savePath = Utils::getImageSavePath(userImage);
                CCHttpAgent::getInstance()->_makeMultiDir(savePath.c_str());
                FILE *fp = fopen(savePath.c_str(), "wb+");
                
                const unsigned char *buffer = d.getBytes();
                if (fp)
                {
                    fwrite(buffer, d.getSize(), 1, fp);
                    fclose(fp);
                }
                
                Sprite *userPortrit = (Sprite *)portraiCell->getChildByTag(100);
                Point point = userPortrit->getPosition();
                userPortrit->removeFromParent();
                
                Sprite *sp = Utils::createCircleAvatar(userImage, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(114,114));
                sp->setTag(100);
                sp->setPosition(point);
                portraiCell->addChild(sp);
                
                PlatformHelper::showToast("头像上传成功");
                
                __NotificationCenter::getInstance()->postNotification("buluo_potrite_change", __String::create(userImage));
                
            }
            else
            {
                PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            }
            
            delete loginPacket;
        });
        
    }
    
}

void ZuBuLuoInfoLayer::updateName(Ref*ref)
{
    __String *str = (__String*)ref;
    string tempStr = str->getCString();
    char buf[128];
    
    Utils::parseName(20, buf, tempStr.c_str());
    nameText->setString(buf);
}

void ZuBuLuoInfoLayer::showView(Json::Value jsonData)
{
    dataTemp = jsonData;
    
    auto visibleSize = Director::getInstance()->getWinSizeInPixels();
    float H = visibleSize.height - getHeadH();
    
    ListView *listView1 = ListView::create();
    listView1->setInertiaScrollEnabled(false);
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(visibleSize.width, H));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(0, 0));
    listView1->setItemsMargin(0);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    Size cellSize = Size(visibleSize.width, 150);
    for (int i = 0; i < 4; i++)
    {
        auto cell = Layout::create();
        cell->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        cell->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
        cell->setContentSize(cellSize);
        listView1->addChild(cell);
        
        Text *left = Text::create("", ".SFUIDisplay-Semibold", 40);
        left->setAnchorPoint(Vec2(0, 0.5));
        left->setTextColor(Color4B(0xe6, 0xe6, 0xe6, 255));
        left->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        left->setPosition(Vec2(40, cellSize.height/2));
        cell->addChild(left);
        
        Layout *line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setPosition(Vec2(40, 0));
        line->setContentSize(Size(cellSize.width - 40, 1));
        cell->addChild(line);
        
        Text *right = Text::create("", ".SFUIDisplay-Semibold", 40);
        right->setAnchorPoint(Vec2(1, 0.5));
        right->setTextColor(Color4B(0x66, 0x66, 0x66, 255));
        right->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        right->setPosition(Vec2(cellSize.width - 40, cellSize.height/2));
        cell->addChild(right);
        
        ImageView *arrow = ImageView::create("Mine/me_comeback.png");
        arrow->setAnchorPoint(Vec2(1, 0.5));
        arrow->setPosition(Vec2(visibleSize.width - 40, cellSize.height/2));
        cell->addChild(arrow);
        
        char buf[128];
        
        switch (i)
        {
            case 0:
            {
                portraiCell = cell;
                
                cell->setTouchEnabled(true);
                right->setString("设置");
                right->setPosition(Vec2(cellSize.width - 100, cellSize.height/2));
                cell->setContentSize(Size(visibleSize.width, 200));
                
                Sprite* portrit = Utils::createCircleAvatar(jsonData["logo"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(114,114));
                portrit->setTag(100);
                portrit->setPosition(93, cell->getContentSize().height/2);
                cell->addChild(portrit);
                cell->addTouchEventListener(CC_CALLBACK_2(ZuBuLuoInfoLayer::portraitFun, this));
                
            }   break;
            case 1:
                
                cell->setTouchEnabled(true);
                left->setString("俱乐部名称");
                right->setPosition(Vec2(cellSize.width - 100, cellSize.height/2));
                Utils::parseName(20, buf, jsonData["name"].asString().c_str());
                cell->addTouchEventListener(CC_CALLBACK_2(ZuBuLuoInfoLayer::nameFun, this));
                right->setString(buf);
                
                nameText = right;
                
                break;
                
            case 2:
                left->setString("俱乐部ID");
                sprintf(buf, "%d", jsonData["id"].asInt());
                right->setString(buf);
                arrow->setVisible(false);
                break;
                
            case 3:
                left->setString("开通日期");
                right->setString(createTimeBecomeString(jsonData["createDatetime"].asDouble()/1000));
                arrow->setVisible(false);
                break;
        }
    }
}

void ZuBuLuoInfoLayer::portraitFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        MaskSelectLayer *mask = MaskSelectLayer::create(1);
        mask->setPosition(Vec2::ZERO);
        this->addChild(mask);
    }
    
}

void ZuBuLuoInfoLayer::nameFun(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        //昵称修改弹窗
        TipChangeEditLayer *tip = TipChangeEditLayer::create(4, dataTemp["name"].asString());
        tip->setPosition(Vec2::ZERO);
        this->addChild(tip);
    }
    
}

string ZuBuLuoInfoLayer::createTimeBecomeString(long long sec)
{
    time_t time_sec1 = sec;

    struct tm * test = localtime(&time_sec1);
    int year = test->tm_year + 1900;
    int mon = test->tm_mon+1;
    int day = test->tm_mday;
    
    char buf[128];
    sprintf(buf, "%d.%d.%d", year, mon, day);
    
    return buf;
}
