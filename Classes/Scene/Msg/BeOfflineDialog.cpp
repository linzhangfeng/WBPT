//
//  BeOfflineDialog.cpp
//  ZJH
//
//

#include "BeOfflineDialog.h"
#include "../../Model/MsgModel.h"

USING_NS_CC;

bool BeOfflineDialog::init()
{
    if (Layer::init() == false) {
        return  false;
    }
    
    MsgModel::getInstance()->isShowBeOfflineDialog = true;
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 100));
    this->addChild(mask);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Size textSize = Size(600, 300);
    Size bgSize = textSize + Size(100, 100);
    Sprite *dialog =  Utils::createDialogBG(bgSize.width, bgSize.height);
    dialog->setPosition(winSize / 2);
    dialog->setName("dialog");
    this->addChild(dialog, 100);
    
    Label *contentLabel = Label::createWithSystemFont("你的账号在其他设备上登录了，如非本人操作请尽快修改密码，是否重新登录？", "Arial", 40, textSize);
    contentLabel->setColor(Color3B(208, 208, 208));
    contentLabel->setAnchorPoint(Vec2(0.5, 0.5));
    contentLabel->setPosition(bgSize / 2);
    dialog->addChild(contentLabel);
    
    Button *confirmBtn = Button::create("Room/dialog/v_alert_dialog_btn_green.png", "Room/dialog/v_alert_dialog_btn_green_press.png");
    confirmBtn->setPosition(Vec2(bgSize.width * 0.75, 70));
    confirmBtn->setTag(ALERT_DIALOG_BUTTON_CONFIRM);
    confirmBtn->addClickEventListener(CC_CALLBACK_1(BeOfflineDialog::buttonClicked, this));
    confirmBtn->setTitleText("确认");
    confirmBtn->setTitleColor(Color3B(208, 208, 208));
    confirmBtn->setTitleFontSize(35);
    dialog->addChild(confirmBtn);
    
    Button *cancelBtn = Button::create("Room/dialog/v_alert_dialog_btn_red.png", "Room/dialog/v_alert_dialog_btn_red_press.png");
    cancelBtn->setPosition(Vec2(bgSize.width * 0.25, confirmBtn->getPositionY()));
    cancelBtn->setTag(ALERT_DIALOG_BUTTON_CANCEL);
    cancelBtn->addClickEventListener(CC_CALLBACK_1(BeOfflineDialog::buttonClicked, this));
    cancelBtn->setTitleText("取消");
    cancelBtn->setTitleColor(Color3B(208, 208, 208));
    cancelBtn->setTitleFontSize(35);
    dialog->addChild(cancelBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event){return true;};
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void BeOfflineDialog::buttonClicked(cocos2d::Ref *sender)
{
    Button *btn = dynamic_cast<Button *>(sender);
    if (btn)
    {
//        MsgModel::getInstance()->isShowBeOfflineDialog = false;
        
        int tag = btn->getTag();
        
        if (tag == ALERT_DIALOG_BUTTON_CONFIRM)
        {
            ZJHModel::getInstance()->UserKey = "";
            this->removeFromParentAndCleanup(true);
            if (!ZJHModel::getInstance()->enterGame)
            {
                if (PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
                {
                    ZJHModel::getInstance()->UserKey = "";
                    PlatformHelper::sdkDoForVoid_C("logout");
                }
                else
                {
                    ZJHModel::getInstance()->gotoView(VIEW_LOAD);
                }
                return;
            }
//            string phone = ZJHModel::getInstance()->phone;
//            string password =  ZJHModel::getInstance()->pwd;
//            
//            if(phone == "" || password == "")
//            {
//                Json::Value json;
//                json["platFrom"] = 1;
//                json["thirdType"] = 4;
//                json["loginType"] = 3;
//                json["userFrom"] = 1;
//                json["thirdId"] = ZJHModel::getInstance()->randomThirdID(false);
//                CCHttpAgent::getInstance()->sendHttpPost(NULL,"user/thirdLogin",json.toStyledString(),"login");
//            }
//            else
//            {
//                Json::Value json_s;
//                json_s["phone"] = phone;
//                json_s["loginType"] = 1;  //0:unknown 1:normal 2:fast 3:third
//                json_s["password"] = password;
//                
//                json_s["platFrom"] = 1; //1:app 2:wap 网页
//                json_s["userFrom"] = 1; //1:app 2:wap 网页
//                
//                //登录请求
//                CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
//                    if (getReferenceCount() == 0 || getReferenceCount() > 10)return;
//                    
//                    CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
//                    
//                    if (loginPacket->status != 3)
//                    {
//                        return;
//                    }
//                    
//                    loginPacket->status = 0;
//                    
//                    if (loginPacket->resultIsOK())
//                    {
//                        Json::Value data = loginPacket->recvVal["resultMap"];
//                        ZJHModel::getInstance()->UserKey = data["userKey"].asString();
//                        
//                        this->removeFromParentAndCleanup(true);
//                        MsgModel::getInstance()->isShowBeOfflineDialog = false;
//                        CCTcpClient::getInstance(TCP_MSG)->connect();
//                    }
//                    else
//                    {
//                        PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
//                    }
//                    
//                    CCHttpAgent::getInstance()->packets[tag] = NULL;
//                    delete loginPacket;
//                },"user/login", json_s.toStyledString(),"login");
//            }
        }
        else if (tag == ALERT_DIALOG_BUTTON_CANCEL)
        {
//            this->removeFromParentAndCleanup(true);
            Director::getInstance()->end();
            exit(0);
        }
    }
}
