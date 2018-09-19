//
//  ZJHModel.cpp
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#include "ZJHModel.h"
#include "ZJH.h"

#include "../Scene/Load/GameLoad.hpp"

#include "../Scene/Msg/AddFriends.hpp"
#include "../Scene/Msg/AgainstNotify.hpp"
#include "../Scene/Msg/RoomChargNotify.hpp"
#include "../Scene/Msg/TribleJoinNotify.hpp"
#include "../Scene/Msg/DynamicNotify.hpp"
#include "../Scene/Msg/MyFriends.hpp"
#include "../Scene/Msg/FoundFriends.hpp"
#include "../Scene/Msg/NearFriends.hpp"
#include "../Scene/Msg/ChatFriends.hpp"
#include "../Scene/Msg/ChatFriendsInfo.hpp"
#include "../Scene/Msg/ChatFriendsDT.hpp"
#include "../Scene/Msg/ChatFriendsRooms.hpp"
#include "../Scene/Msg/ImageListView.hpp"
#include "../Scene/Msg/BlackFriends.hpp"
#include "../Scene/Msg/ChatFriendsMore.hpp"
#include "../Scene/Msg/ChatFriendsMore_Nick.hpp"
#include "../Scene/Msg/ChatFriendsMore_Jubao.hpp"
#include "../Scene/Msg/ChatClubVIew.hpp"

#include "../Scene/Mine/UserInfoLayer.h"
#include "../Scene/Mine/ShopAllLayer.hpp"
#include "../Scene/Mine/HobbyLayer.hpp"
#include "../Scene/Mine/VIPCentreLayer.hpp"
#include "../Scene/Mine/TeQuanLayer.hpp"
#include "../Scene/Mine/TaskLayer.hpp"
#include "../Scene/Mine/InviteLayer.hpp"
#include "../Scene/Mine/SettingLayer.hpp"
#include "../Scene/Mine/LoadLayer.hpp"
#include "../Scene/Mine/RegistLayer.hpp"
#include "../Scene/Mine/RigistFinishLayer.hpp"
#include "../Scene/Mine/ChangePhone2Layer.hpp"
#include "../Scene/Mine/ChangePassWordLayer.hpp"
#include "../Scene/Mine/FastLoadLayer.hpp"
#include "../Scene/Mine/ForgetPassLayer.hpp"
#include "../Scene/Mine/ForgetPass2Layer.hpp"
#include "../Scene/Mine/MineRoomLayer.hpp"
#include "../Scene/Mine/CreateRoomLayer.hpp"
#include "../Scene/Mine/InviteFriendListLayer.hpp"
#include "../Scene/Mine/KeFuCenter.hpp"

#include "../Scene/BuLuo/MyBuLuoLayer.h"
#include "../Scene/BuLuo/BuLuoInfoLayer.hpp"
#include "../Scene/BuLuo/BuLuoManagerLayer.h"
#include "../Scene/BuLuo/BuLuoTuiGuangLayer.h"
#include "../Scene/BuLuo/ZuBuLuoInfoLayer.hpp"

#include "../Scene/Hall/HotList.h"
#include "../Scene/Hall/SearchRoom.h"
#include "../Scene/Hall/RichList.hpp"

#include "../Scene/Match/CreateMatchLayer.hpp"
#include "../Scene/Match/GameWinLayer.hpp"
#include "../Scene/Match/MatchPlayerLayer.hpp"
#include "../Scene/Match/RecordDegitalLayer.hpp"
#include "../Scene/Match/ShareWanBeiFriend.hpp"
#include "../Scene/Match/LuXiangReviewLayer.hpp"
#include "../Scene/Match/MatchResultLayer.hpp"

#include "../Scene/Mine/ErWeiMaLayer.hpp"
#include "../Scene/Mine/MyPropLayer.hpp"
#include "../Scene/Mine/MineMoreLayer.hpp"
#include "../Scene/Mine/FeedBacklayer.hpp"
#include "../Scene/Mine/AboutOurLayer.hpp"
#include "../Scene/Mine/PhoneLoadLayer.hpp"
#include "../Scene/Mine/MyDaiLiLayer.h"

#include "../Scene/Play/Play.hpp"

#include "../Scene/Load/LoadTable.h"

#include "../Scene/Mine/RealNameAuthLayer.hpp"

#include "../JsUtils/HotUpdateLayer.hpp"
#include "../JsUtils/JsUtils.h"

#include "../Scene/Hall/HallScene.hpp"
#include "../Scene/GameHall/GameHallScene.hpp"
#include "../Scene/GameHall/RecordDetailsBox.hpp"

#include "SoundModel.h"
#include "MsgModel.h"

ZJHModel *zjhModel;

ZJHModel::ZJHModel()
{
    portrit = "Mine/user4_unlogin.png";
    nick = "";
}

ZJHModel::~ZJHModel()
{
    
}

ZJHModel* ZJHModel::getInstance()
{
    if (zjhModel == NULL) {
        zjhModel = new ZJHModel();
        zjhModel->init();
        zjhModel->retain();
    }
    
    return zjhModel;
}

std::string ZJHModel::randomThirdID(bool isReset)
{
    std::string userStr = UserDefault::getInstance()->getStringForKey("thirdID","");
    if (isReset || userStr == "")
    {
        std::stringstream strio;
        strio.clear();
        strio<<phoneimei<<Utils::getCurrentTime();
        
        std::string md5_str;
        MD5::MD5Str(strio.str(), md5_str);
        UserDefault::getInstance()->setStringForKey("thirdID", md5_str);
        return md5_str;
    }
    return userStr;
}

void ZJHModel::initPhoneData()
{
    channel_id = PlatformHelper::sdkDoForStr_C("channel_id");
    phoneimei = PlatformHelper::sdkDoForStr_C("phoneimei");
    phonename = PlatformHelper::sdkDoForStr_C("phonename");
    phoneOs = PlatformHelper::sdkDoForStr_C("phoneos");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   
    phonePlatfm = "ios";
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    phonePlatfm = "android";
    
#endif
}

int ZJHModel::init()
{
    thirdId = randomThirdID(false);
    UserKey = "";
    vname = "2.7.0";
    vcode = 20056000;
    uid = 0;
    mySeatid = -1;

    curView = NULL;
    phone = UserDefault::getInstance()->getStringForKey(USER_PHONE,"");
    pwd = UserDefault::getInstance()->getStringForKey(USER_PASSWORD,"");
    
    userType = UserDefault::getInstance()->getIntegerForKey(USER_TYPE, 6);//默认游客类型
    
    //当前发布的版本
    versionIOS = "2.7.0";
    
    isGameLoad = false;
    enterGame = false;
    count = 0;
    isGamePhzFrist =true;
    curRoomID = 0;
    
     __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ZJHModel::reLoginLoadGame), "reLogin", NULL);
     __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ZJHModel::noticeGamePlayEvent), "game_play_event", NULL);
    isPause = 0;
    
    isOnVideo = false;
    
    curGameID = UserDefault::getInstance()->getIntegerForKey("curGameID",-1);
    curGameHasHall = UserDefault::getInstance()->getIntegerForKey("curGameHasHall",-1);
    
    firtEnterApp = true;
    isNoticeFirstShow = true;

    gameUseDefaultSetting = true;
    gameUseDefaultChat = true;
    gameDefaultChat = "";
    
    diquSelfCach = Utils::ReadFile("diquCache");

    iosAppBuySuceesePostTimes = 0;
    latestRunningSize = Size(1080,1920);
    isReplace = true;
    recoveryHallParam = "";
    return 0;
}

void ZJHModel::setCurGameID(int id,int hall)
{
    UserDefault::getInstance()->setIntegerForKey("curGameID", id);
    UserDefault::getInstance()->setIntegerForKey("curGameHasHall", hall);
    curGameID = id;
    curGameHasHall = hall;
}

void ZJHModel::reLoginLoadGame(Ref *ref)
{
    Director::getInstance()->replaceScene(GameLoad::createScene());
}

void ZJHModel::GameResum()
{
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(ZJHModel::listnerGameResum),this,1.0,false);
}

void ZJHModel::listnerGameResum(float dt)
{
    cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(ZJHModel::listnerGameResum),this);
    
    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    ZJHModel::getInstance()->isPause = 0;
    
//    if(ZJHModel::getInstance()->enterGame)
    {
        if (time(NULL) - ZJHModel::getInstance()->pauseTime > 5)__NotificationCenter::getInstance()->postNotification("notify_game_resume");
        MsgModel::getInstance()->start();
        SoundModel::getInstance()->clear();
    }
}

const char* ZJHModel::getStr(const char* key)
{
    return ((__String*)zjhModel->pLuaguaDict->valueForKey(key))->getCString();
}

void ZJHModel::checkUploadMoblieContacts()
{
    
}

void ZJHModel::requestPay(int goodsId, int num, int payType)
{
    Json::Value json;
    json["id"] = goodsId;
    json["UserKey"] = ZJHModel::getInstance()->UserKey;
    json["num"] = num;
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                {
                                                    CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                    if (packet) {
                                                        if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                        {
                                                            //nothing
                                                        }
                                                        else
                                                        {
                                                            if (packet->status == 3)
                                                            {
                                                                packet->status = 0;
                                                                
                                                                if (packet->recvVal["success"].asBool())
                                                                {
                                                                    PlatformHelper::requestPay(packet->recvVal["resultMap"], payType);
                                                                }
                                                                else
                                                                {
                                                                    __NotificationCenter::getInstance()->postNotification("appstore_deleteLoad");
                                                                    PlatformHelper::showToast(packet->recvVal["message"].asString().c_str());
                                                                }
                                                                
                                                            }
                                                        }
                                                        
                                                        //删除packet
                                                        CCHttpAgent::getInstance()->packets.erase(tag);
                                                        delete packet;
                                                    }
                                                },
                                                "store/buy/more", json.toStyledString(), "generate_order");
    return;
}

void ZJHModel::requestThirdPay(int goodsId, int num)
{
    Json::Value json;
    json["id"] = goodsId;
    json["UserKey"] = ZJHModel::getInstance()->UserKey;
    json["num"] = num;
    json["type"] = PlatformHelper::sdkDoForInt_C("thirdPayType");
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                             {
                                                 CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                 if (packet) {
                                                     if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                     {
                                                         //nothing
                                                     }
                                                     else
                                                     {
                                                         if (packet->status == 3) {
                                                             packet->status = 0;
                                                             
                                                             if (packet->recvVal["success"].asBool())
                                                             {
                                                                 PlatformHelper::sdkDoForVoid_C("thirdPay", packet->recvVal["resultMap"].toStyledString());
                                                             }
                                                             else
                                                             {
                                                                 PlatformHelper::showToast(packet->recvVal["message"].asString().c_str());
                                                             }
                                                             
                                                         }
                                                     }
                                                     
                                                     //删除packet
                                                     CCHttpAgent::getInstance()->packets.erase(tag);
                                                     delete packet;
                                                 }
                                             },
                                             "store/buy/more", json.toStyledString(), "generate_order");
    return;
}

void ZJHModel::doPay(Json::Value data, int payType)
{
    
}

BaseLayer* ZJHModel::gotoView(int id,Json::Value json)
{
    if(id == VIEW_WAN_SHOP)
    {
        return NULL;
    }
    
    Size size = Director::getInstance()->getWinSize();
    BaseLayer* newView = NULL;
    switch (id)
    {
        case VIEW_SHOP:
            break;
            
        case VIEW_VIP:
            break;
            
        case VIEW_MY_FRIENDS:
            newView = MyFriends::create();
            break;
            
        case VIEW_ADD_FRIENDS:
            newView = AddFriend::create();
            break;
            
        case VIEW_FOUND_FRIENDS:
            newView = FoundFriends::create();
            break;
            
        case VIEW_NEAR_FRIENDS:
            newView = NearFriends::create();
            break;
            
        case VIEW_MINE_USER_INFO:
            newView = UserInfoLayer::create();
            break;
        case VIEW_HOBBY:
            newView = HobbyLayer::create();
            break;

        case VIEW_WAN_SHOP:
            newView = ShopAllLayer::create();
            break;

        case VIEW_VIP_CENTER:
        {
            newView = VIPCentreLayer::create();
            VIPCentreLayer *VIPView = (VIPCentreLayer*)newView;
            VIPView->postVIPInfoRequest(json);
        }
            break;
        case VIEW_TE_QUAN:
            newView = TeQuanlayer::create();
            break;
            
        case VIEW_INVITE:
            newView = InviteLayer::create();
            break;
        case VIEW_MINE_SETTING:
            newView = SettingLayer::create();
            break;
        case VIEW_LOAD:
            newView = LoadLayer::create();
            break;
        case VIEW_REGIST:
            newView = RegistLayer::create();
            break;
        case VIEW_REGIST_FINISH:
        {
            newView = RigistFinishLayer::create();
            RigistFinishLayer *finish = (RigistFinishLayer*)newView;
            finish->getInfoJson(json);
        }
            break;
        case VIEW_UPDATE_PHONE2:
            newView = ChangePhone2Layer::create();
            break;
        case VIEW_UPDATE_PASSWORD:
            newView = ChangePassWordLayer::create();
            break;
        case VIEW_FAST_LOAD:
            newView = FastLoadLayer::create();
            break;
        case VIEW_FORGET_PASSWORD1:
            newView = ForgetPassLayer::create();
            break;
        case VIEW_FORGET_PASSWORD2:
        {
            newView = ForgetPass2Layer::create();
            ForgetPass2Layer *forget2 = (ForgetPass2Layer*)newView;
            forget2->getInfoJson(json);
        }
            break;
        case VIEW_MINE_ROOM:
            newView = MineRoomLayer::create();
            break;
        case VIEW_MINE_CREAT_ROOM:
            newView = CreateRoomLayer::create();
            break;
        case VIEW_HOT_LIST:
            newView = HotList::create();
            break;
            
        case VIEW_SEARCH_ROOM:
            newView = SearchRoom::create();
            break;

        case VIEW_CHAT_FRIENDS:
        {
            ChatFriends* chatFriend = ChatFriends::create();
            chatFriend->myInit(json);
            newView = chatFriend;
            //测试
//            ChatClubVIew* chatClub = ChatClubVIew::create();
//            chatClub->myInit(json);
//            newView = chatClub;
            break;
        }
        case VIEW_CHAT_CLUB:
        {
            ChatClubVIew* chatClub = ChatClubVIew::create();
            chatClub->myInit(json);
            newView = chatClub;
            break;
        }
        case VIEW_FRIENDS_INFO:
        {
            ChatFriendsInfo* chatFriend = ChatFriendsInfo::create();
            chatFriend->myInit(json);
            newView = chatFriend;
            break;
        }
            
        case VIEW_FRIENDS_DT:
        {
            ChatFriendsDT* chatFriend = ChatFriendsDT::create();
            chatFriend->myInit(json);
            newView = chatFriend;
            break;
        }
            
        case VIEW_FRIENDS_ROOMS:
        {
            ChatFriendsRooms* chatFriend = ChatFriendsRooms::create();
            chatFriend->myInit(json);
            newView = chatFriend;
            break;
        }
        case VIEW_SHOW_IMAGE:
        {
            ImageListView* chatFriend = ImageListView::create();
            chatFriend->myInit(json);
            newView = chatFriend;
            break;
        }

        case VIEW_INVITE_FRIEND:
            newView = InviteFriendListLayer::create();
            break;
        case VIEW_CREATE_MATCH:
        {
            CreateMatchLayer * create = CreateMatchLayer::create();
            create->showView(json);
            newView = create;
            break;
        }
            break;
        case VIEW_WINRATE:
            newView = GameWinLayer::create();
            break;
        case VIEW_MATCH_PLAYER:
            newView = MatchPlayerLayer::create();
            break;

        case VIEW_RECORD_DETIL:
        {
            RecordDegitalLayer * recordDetail = RecordDegitalLayer::create();
            recordDetail->show(json);
            newView = recordDetail;
            break;
        }

        case VIEW_FRIEND_BLACK:
            newView = BlackFriends::create();
            break;
        case VIEW_FRIENDS_MORE:
        {
            ChatFriendsMore* fMoreSet = ChatFriendsMore::create();
            fMoreSet->myInit(json);
            newView = fMoreSet;
        }
            break;
        case VIEW_FRIENDS_MORE_NICK:
        {
            ChatFriendsMore_Nick* fMoreNick = ChatFriendsMore_Nick::create();
            fMoreNick->myInit(json);
            newView = fMoreNick;
        }
            break;
        case VIEW_FRIENDS_MORE_JUBAO:
        {
            ChatFriendsMore_Jubao* fMoreJB = ChatFriendsMore_Jubao::create();
            fMoreJB->myInit(json);
            newView = fMoreJB;
        }
            break;
        case VIEW_ER_WEI_MA:
        {
            ErWeiMaLayer * view = ErWeiMaLayer::create();
            view->getUserData(json["qrCode"].asString());
            newView = view;
        }
            
            break;
        case VIEW_MY_PROP:
            newView = MyPropLayer::create();
            break;
        case VIEW_RICH_LIST:
           newView= RichList::create();
            break;
        case VIEW_START_FAST:
            newView= Play::create();
            break;
        case VIEW_MINE_MORE:
            newView= MineMoreLayer::create();
            break;
        case VIEW_FEEDBACK:
            newView= FeedBacklayer::create();
            break;
        case VIEW_ABOUT_OUR:
            newView= AboutOurLayer::create();
            break;
        case VIEW_SHARE_WANBEI_FRIEND:
        {
            newView= ShareWanBeiFriend::create();
            ShareWanBeiFriend *finish = (ShareWanBeiFriend*)newView;
            finish->getInfoJson(json);
        }
            break;
        case VIEW_TRIBLE_JOIN_REQUEST:{
            //部落加入申请
            TribleJoinNotify* trView = TribleJoinNotify::create();
            trView->initData(json);
            newView = trView;
        }
            break;
        case VIEW_AGAINST_NOTIFY:
        {
            AgainstNotify* myView = AgainstNotify::create();
            myView->initData(json);
            newView = myView;
        }
            break;
        case VIEW_ROOM_CHARGE:
        {
            RoomChargNotify* chView = RoomChargNotify::create();
            chView->initData(json);
            newView = chView;
        }
            break;
        case VIEW_MY_BULUO:
            newView = MyBuLuoLayer::create();
            break;
        case VIEW_BULUO_INFO:
        {
            BuLuoInfoLayer *info = BuLuoInfoLayer::create();
            info->showView(json);
            newView = info;
        }
            break;
        case VIEW_BULUO_MANAGER:
            newView = BuLuoManagerLayer::create();
            break;
        case VIEW_REAL_NAME_AUTH:
            newView = RealNameAuthLayer::create();
            break;
        case VIEW_BULUO_TUI_GUANG:
        {
            BuLuoTuiGuangLayer *tuiGuang = BuLuoTuiGuangLayer::create();
            tuiGuang->showInfoView(json);
            newView = tuiGuang;

        }
            break;
        case VIEW_ZUZHANG_BULUO_INFO:
        {
            ZuBuLuoInfoLayer *zuInfo = ZuBuLuoInfoLayer::create();
            zuInfo->showView(json);
            
            newView = zuInfo;
            
        }
            break;
        case VIEW_LUXIANG_REVIEW:
        {
            LuXiangReviewLayer *luXiang = LuXiangReviewLayer::create();
            luXiang->showView(json);
            newView = luXiang;
            break;
        }
        case VIEW_PAIJU_DONGTAI:
        {
            DynamicNotify* trView = DynamicNotify::create();
            trView->initData(json);
            newView = trView;
            break;
        }
        
        case VIEW_PHONE_LOAD:
            newView = PhoneLoadLayer::create();
            break;
        case  VIEW_MATCH_RESULT:
            newView = MatchResultLayer::create();
            break;
        case VIEW_MINE_MY_DAILI:
            newView = MyDaiLiLayer::create();
            break;
            
        case VIEW_KEFU_CENTER:
            newView = KeFuCenter::create();
            break;
    }
    
    if (newView)
    {
        int notPreView = json["notPreView"].asInt();
        if(notPreView != 1){
            newView->setPreView(curView);
        }
        newView->appear();
        newView->setPosition(size.width, 0);
        Director::getInstance()->getRunningScene()->addChild(newView, Z_ORDER_NEW_VIEW);
        return newView;
    }
    return NULL;
}


///begin=====
///@todo 房间相关头函数，完成后需要整理
bool ZJHModel::checkNoMoney(int value)
{
    if(money >= value)
    {
        return false;
    }
    //    if (value / 10000 > rmb)
    //    {
    //        BuyDiamondLayer::show();
    //        PlatformHelper::showToast("金币不足，您可购买钻石来兑换金币");
    //    }
    //    else
    {
//        Shop::showShop();
        PlatformHelper::showToast(__String::createWithFormat("金币不足%d，可使用钻石兑换金币",value)->getCString());
    }
    
    return true;
}

bool ZJHModel::checkNoRmb(int value)
{
    if(rmb >= value)
    {
        return false;
    }
//    BuyDiamondLayer::show();
    PlatformHelper::showToast(__String::createWithFormat("钻石不足%d个，请购买钻石",value)->getCString());
    return true;
}

///end=======
void ZJHModel::setPhoneAndPwd(std::string phone, std::string pwd)
{
    UserDefault::getInstance()->setStringForKey(USER_PHONE,phone);
    UserDefault::getInstance()->setStringForKey(USER_PASSWORD,pwd);
    
    this->phone = phone;
    this->pwd = pwd;
    
}

void ZJHModel::setUserType(int userType)
{
    UserDefault::getInstance()->setIntegerForKey(USER_TYPE,userType);
    this->userType = userType;
}

//登录账号的问题
bool ZJHModel::isYoukeLogin()
{
    if (ZJHModel::getInstance()->userType == 6)
    {
        return true;
    }
    return false;
}

void ZJHModel::gotoRoomWithCode(std::string code,bool replaceScenc,string param)
{
    Json::Value json;
    json["code"] = code;
    json["userJoin"] = 0;
    //加入对局的请求
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
            Json::Value data = loginPacket->recvVal["resultMap"];
            ZJHModel::getInstance()->gameHelpUrl = CCHttpAgent::getInstance()->url+data["gameRoom"]["gameHelpUrl"].asString();
            ZJHModel::getInstance()->roomData = data;
            if(replaceScenc){
                //替换掉当前场景  销毁之前的场景
                Director::getInstance()->replaceScene(LoadTable::createScene());
                isReplace = true;
            }else{
                //push新场景进去  不销毁之前的场景
                isReplace = false;
                latestRunningSize = Director::getInstance()->getWinSize();
                Director::getInstance()->pushScene(LoadTable::createScene());
            }
            recoveryHallParam = param;
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
            //刷新免房间号房间
            __NotificationCenter::getInstance()->postNotification("refresh_room_error", __String::create(code));
        }
    },"duiju/view",json.toStyledString(),"duijuCode");
}

void ZJHModel::gotoRoom(int id, int code)
{
    Json::Value json;
    if (code != -1)
    {
        json["code"] = code;
        
        
    }
    else
    {
        json["roomId"] = id;
    }
    
    json["userJoin"] = 0;
    
    //加入对局的请求
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
            Json::Value data = loginPacket->recvVal["resultMap"];
            
            ZJHModel::getInstance()->roomData = data;
            ZJHModel::getInstance()->gameHelpUrl = CCHttpAgent::getInstance()->url+data["gameRoom"]["gameHelpUrl"].asString();
            Director::getInstance()->replaceScene(LoadTable::createScene());
        }
        else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"duiju/view",json.toStyledString(),"duijuCode");
}

void ZJHModel::loadingImageResource(ImageView *image, string imageURL)
{
    if (Utils::hasImageFile(imageURL)) {
        image->loadTexture(Utils::getImageSavePath(imageURL));
    } else {
        image->loadTexture("Match/icon_wanbei.png");
        CCHttpAgent::getInstance()->sendHttpImage(imageURL, Utils::getImageSavePath(imageURL), Utils::getImageSavePath(imageURL), [=](std::string tag)
                                                  {
                                                      if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
                                                      
                                                      CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                      if (packet) {
                                                          CCHttpAgent::getInstance()->packets.erase(tag);
                                                          delete packet;
                                                      }
                                                      
                                                      if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                      
                                                      if(Utils::hasImageFile(imageURL))
                                                      {
                                                          image->loadTexture(Utils::getImageSavePath(imageURL));
                                                      }
                                                  });
    }

}

std::string ZJHModel::moneyShowDeal(int value)
{
    float temp;
    __String *bufStr = __String::createWithFormat("");
    
    if (value >= 10000 && value <= 99999999)
    {
        temp = value/10000.0;
        if (value >= 95999999)
        {
            bufStr = __String::createWithFormat("1.0亿");
        }else
        {
            bufStr = __String::createWithFormat("%.1f万", temp);
        }
    }else if (value >= 100000000)
    {
        temp = value/100000000.0;
        bufStr = __String::createWithFormat("%.1f亿", temp);
    }else if (value >= 0 && value <= 9999)
    {
        bufStr = __String::createWithFormat("%d", value);
    }
    
    return bufStr->getCString();
}

string ZJHModel::goldCovertStr2(int value)
{
    char buf[128];
    if (value > 0 && value < 10000)
    {
        sprintf(buf, "%d金币", value);
    }else if (value >= 10000)
    {
        sprintf(buf, "%d万金币", value/10000);
    }
    return buf;
}

bool ZJHModel::isCanPay()
{
    bool flagPay = false;
    Json::Value _pay = ZJHModel::getInstance()->gameConfigJson["list"]["payTypeFilter"];
    
    if (_pay["union"].asBool() || _pay["wechat"].asBool())
    {
        flagPay = true;
    }
    return flagPay;
}

bool ZJHModel::isOpenIPV6()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return ZJHModel::getInstance()->gameConfigJson["list"]["payTypeFilter"]["IPV6Filter"].asBool();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
    
#endif
    
}
std::string ZJHModel::converPointTwoBit(float value)
{
    char buf[128];
    sprintf(buf, "%.2f", value);

    long long len = strlen(buf);
    
    char to[len+1];
    int j = 0;
    for (long long i = 0; buf[i] != '\0'; i++)
    {
        if (i == len-1 && buf[i] == '0')
        {
            continue;
        }
        
        to[j++] = buf[i];
    }
    to[j] = '\0';
    string to2 = to;
    
    return to2;
}

string ZJHModel::hidePhoneNum(string phone)
{
    string temp;
    for (int i = 0; i < phone.size(); i++)
    {
        if (i == 3 || i == 4 || i == 5 || i == 6)
        {
            temp += "*";
        }else
        {
            temp += phone[i];
        }
    }
    return temp;
}

void ZJHModel::roomToHall(bool isFromRoom)
{
    CCTcpClient::getInstance(ZJHModel::getInstance()->curRoomID)->reset_connect();
    ZJHModel::getInstance()->curRoomID = -1;
    ZJHModel::getInstance()->curGameType = -1;
    ZJHModel::getInstance()->enterGame = false;
    ZJHModel::getInstance()->gamePuase = false;
    Sound::getInstance()->StopBgSound();
    
    __NotificationCenter::getInstance()->postNotification("will_exit_room");
    
    if(isReplace){
        if (recoveryHallParam == "" && (isFromRoom || curGameHasHall == 1)){
            ZJHModel::getInstance()->toGameHall();
        }else{
            
            //处理游戏中进入俱乐部竖屏时卡崩的问题
        
           cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(ZJHModel::delayToHall),this, 0.0,false);
        }
    }else{
        isReplace = true;
        Director::getInstance()->popScene();
        if(latestRunningSize.width > latestRunningSize.height){
            //切换成横屏
            changeScreenSize(1920,1080);
        }else{
            //切换成竖屏
            changeScreenSize(1080,1920);
        }
    }
}

//处理游戏中进入俱乐部竖屏时卡崩的问题
void ZJHModel::delayToHall(float dt)
{
    cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(ZJHModel::delayToHall),this);
    
    ZJHModel::getInstance()->toHall();
}


void ZJHModel::toHall()
{
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    __NotificationCenter::getInstance()->postNotification("will_exit_room");
    
    //    Director::getInstance()->popScene();
    
    Size size = cocos2d::Director::getInstance()->getWinSize();
    
    if(size.width > size.height)//如果当前是横的 要竖过来
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewH();
    }
    
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1080, 1920, ResolutionPolicy::EXACT_FIT);
    
    Director::getInstance()->replaceScene(HallScene::createScene());
    
    ZJHModel::getInstance()->setCurGameID(curGameID, 0);
}

void ZJHModel::toGameHall(int state)
{
    __NotificationCenter::getInstance()->postNotification("will_exit_room");
    Size size = cocos2d::Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(size.width < size.height)//如果当前是竖的 要横过来
    {
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewV();
    }
    glview->setDesignResolutionSize(1920, 1080, ResolutionPolicy::EXACT_FIT);
    
    auto scene = GameHallScene::createScene();
    Director::getInstance()->replaceScene(scene);
    ZJHModel::getInstance()->setCurGameID(curGameID, 1);
    
    if(state == 1)
    {
        Json::Value data;
        data["roomID"] = curRoomID;
        data["gameId"] = curGameType;

        auto box = RecordDetailsBox::create();
        box->show(data);
        scene->addChild(box,100);
    }
    else if(state == 2)
    {
        Json::Value data;
        data["roomID"] = curRoomID;
        data["gameId"] = curGameType;
        auto box = RecordDetailsBox::create();
        box->show(data);

        scene->addChild(box,100);
        
        box->showVideo();
    }

}

void ZJHModel::hallToRoomV()
{
//    auto director = cocos2d::Director::getInstance();
    Size size = cocos2d::Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(size.width < size.height)//如果当前是竖的 要横过来
    {
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewV();
    }
//    auto glview = director->getOpenGLView();
//    Size size = glview->getFrameSize();
//    glview->setFrameSize(size.height, size.width);
    glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::EXACT_FIT);
//    PlatformHelper::cConViewV();
    ZJHModel::getInstance()->enterGame = true;
    ZJHModel::getInstance()->gamePuase = false;
    
    uploadMyLocation();


}

void ZJHModel::hallToRoomH()
{
    Size size = cocos2d::Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(size.width > size.height)//如果当前是横的 要竖过来
    {
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewH();
    }
    
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(720, 1280, ResolutionPolicy::EXACT_FIT);
    ZJHModel::getInstance()->enterGame = true;
    ZJHModel::getInstance()->gamePuase = false;
    
    uploadMyLocation();
}

void ZJHModel::hallToRoom()
{
    Size size = cocos2d::Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(size.width > size.height)
    {
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewH();
    }
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(720, 1280, ResolutionPolicy::EXACT_FIT);
    
    ZJHModel::getInstance()->enterGame = true;
    ZJHModel::getInstance()->gamePuase = false;
    
    uploadMyLocation();
}

void ZJHModel::gotoGameLoad(bool isinit)
{
    if(ZJHModel::getInstance()->isYoukeLogin() && !PlatformHelper::sdkDoForInt_C("hasThirdChannel"))
    {
        auto scene = Scene::create();
        
        LoadLayer *load = LoadLayer::create();
        scene->addChild(load, 100);
        ZJHModel::getInstance()->curView = load;
        ZJHModel::getInstance()->isGameLoad = true;
        
        if (isinit)
        {
            Director::getInstance()->runWithScene(scene);
        }
        else
        {
            Director::getInstance()->replaceScene(scene);
        }
    }
    else
    {
        ZJHModel::getInstance()->isGameLoad = false;
        
        if (isinit)
        {
            Director::getInstance()->runWithScene(GameLoad::createScene());
        }
        else
        {
            Director::getInstance()->replaceScene(GameLoad::createScene());
        }
    }
}

void ZJHModel::gotoVideo(int gid,int rid,std::string data, int pkgId)
{
    curGameType = gid;
    curRoomID = rid;
    roomVideoData = data;
    isOnVideo = true;
    roomData["gameRoom"]["pkgId"] = pkgId;
    log("VVV = %s",roomData.toStyledString().c_str());
#if COCOS2D_DEBUG == 1
    JsUtils::initLoad();
//    JsUtils::runScript("mj_project/scripts/main.js");
//    JsUtils::runScript("mj_chengdu/scripts/main.js");
//    JsUtils::runScript("mj_new/scripts/main.js");
    JsUtils::runScript("mj2.5_new/scripts/main.js");
//    JsUtils::runScript("mj2d_2.5/scripts/main.js");
//    JsUtils::runScript("xiangqi_project/scripts/main.js");
    return;
#endif
    
    Json::Value json;
    json["gameId"] = curGameType;
    
    //检查脚本更新
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data;
            
            Json::Value& publicList = loginPacket->recvVal["resultMap"]["publicList"];
            Json::Value& list = loginPacket->recvVal["resultMap"]["list"];
            for(int i =0;i<publicList.size();++i)
            {
                string url =publicList[i]["jsurl"].asString();
                string pName = publicList[i]["entrance"].asString();
                
                Json::Value dat;
                dat["packageUrl"]="";
                dat["remoteManifestUrl"]=pName+"/version/project.manifest";
                dat["remoteVersionUrl"]=pName+"/version/version.manifest";
                dat["version"]="0";
                dat["assets"].append("");
                dat["assets"].clear();
                dat["searchPaths"].append("");
                dat["searchPaths"].clear();
                
                Json::Value dat1;
                dat1["baseData"]=dat.toStyledString();
                dat1["baseUrl"]=(url+"/");
                dat1["versionPath"]=(pName+"/version/");
                
                data["list"].append(dat1);
            }
            
            for(int i =0;i<list.size();++i)
            {
                if(list[i]["id"] == curGameType)
                {
                    string url =list[i]["jsurl"].asString();
                    string pName = list[i]["entrance"].asString();
                    ZJHModel::getInstance()->packName =pName;
                    Json::Value dat;
                    dat["packageUrl"]="";
                    dat["remoteManifestUrl"]=pName+"/version/project.manifest";
                    dat["remoteVersionUrl"]=pName+"/version/version.manifest";
                    dat["version"]="0";
                    dat["assets"].append("");
                    dat["assets"].clear();
                    dat["searchPaths"].append("");
                    dat["searchPaths"].clear();
                    
                    Json::Value dat1;
                    dat1["baseData"]=dat.toStyledString();
                    dat1["baseUrl"]=(url+"/");
                    dat1["versionPath"]=(pName+"/version/");
                    
                    data["list"].append(dat1);
                    
                    data["entry"]=pName+"/scripts/main.js";
                    Director::getInstance()->replaceScene(HotUpdateLayer::createScene(data));
                    break;
                }
            }
        }
    },"version/zipVersionList",json.toStyledString(),"check");
}

void ZJHModel::uploadMyLocation()
{
    std::map<std::string, double> locationMap = PlatformHelper::getLocation();
    Json::Value json;
    json["latitude"] = locationMap["latitude"];//4.9E-324;
    json["longitude"] = locationMap["longitude"];//4.9E-324;
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"account/uploadUserLocation",json.toStyledString(),"uploadUserLocation");
}

void ZJHModel::roomToShareView(int roomid)
{
    __NotificationCenter::getInstance()->postNotification("will_exit_room");
    //如果是俱乐部房间则直接返回竖版大厅  cloudId不为0 说明为俱乐部房间
    int cloudId = ZJHModel::getInstance()->roomData["clubId"].asInt();
    if (cloudId == 0){
        ZJHModel::getInstance()->toGameHall(isOnVideo?2:1);
    }else{
        Size size = cocos2d::Director::getInstance()->getWinSize();
        
        if(size.width > size.height)//如果当前是横的 要竖过来
        {
            auto glview = cocos2d::Director::getInstance()->getOpenGLView();
            Size _size = glview->getFrameSize();
            glview->setFrameSize(_size.height, _size.width);
            PlatformHelper::cConViewH();
        }
        
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1080, 1920, ResolutionPolicy::EXACT_FIT);
        
        auto scene = HallScene::createScene();
        
        Json::Value data;
        data["roomID"] = curRoomID;
        data["gameId"] = curGameType;
        RecordDegitalLayer * recordDetail = RecordDegitalLayer::create();
        recordDetail->show(data);
        scene->addChild(recordDetail);
        
        Director::getInstance()->replaceScene(scene);
        
        recordDetail->setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
//            Director::getInstance()->replaceScene();
            recordDetail->removeFromParent();
            
        });
        recordDetail->setPosition(Vec2(0, 0));
        int playingCount = UserDefault::getInstance()->getIntegerForKey("lastRoomPlaying");
        UserDefault::getInstance()->setIntegerForKey("lastRoomPlaying", --playingCount);
    }
    isOnVideo = false;

}

//type 0:取消或失败 1:付款成功
void ZJHModel::iosPurchaseResult(int type, const char *transactionId, const char *receipt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->IosTransactionId = transactionId;
    this->IosReceipt = receipt;
    
    Json::Value json;
    json["invoice"] = this->appStoreInvoice;
    json["receipt"] = this->IosReceipt;
    json["totalFee"] = this->iosTotalFee;
    
    //购买完成，删除加载
    if(type == 1)
    {
        PlatformHelper::showToast("正在处理购买的商品");
        
        //做恢复购买的保存
        __String *uidStr = __String::createWithFormat("%d", uid);
        PlatformHelper::saveStoreChainData(IosTransactionId, appStoreInvoice, IosReceipt, iosTotalFee, uidStr->getCString());
        
        //正常购买
        CCHttpAgent::getInstance()->sendAppStoreFinishHttpPost(NULL,"notify/a", json.toStyledString(),"notify-a");
        
        cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ZJHModel::listnerPaySucceed),this,0.0,false);
        
    }else if(type ==0)
    {
        __NotificationCenter::getInstance()->postNotification("appstore_deleteLoad");
    }
    
#endif
}

void ZJHModel::listnerPaySucceed(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["notify-a"];
    if (packet && packet->status != 0)
    {
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        //删除回调
        cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ZJHModel::listnerPaySucceed), this);
        CCHttpAgent::getInstance()->packets["notify-a"] = NULL;
        
        if (packet->recvVal["data"]["ret"].asString() == "0")
        {
            //做恢复购买的数据清空
            PlatformHelper::clearStoreChainData();
            
            PlatformHelper::IOSPayClearId(packet->recvVal["data"]["transaction_id"].asString().c_str());
            
            __NotificationCenter::getInstance()->postNotification("money_change");
            
            PlatformHelper::showToast("购买成功");
            
            __NotificationCenter::getInstance()->postNotification("appstore_deleteLoad");
        }else
        {
            /********************* 重试3次的请求 start **********************/
            if(this->iosAppBuySuceesePostTimes == 3)
            {
                //做恢复购买的数据清空
                std::map<std::string, std::string> storeData = PlatformHelper::getStoreChainData();
                PlatformHelper::clearStoreChainData();
                PlatformHelper::IOSPayClearId(storeData["IosTransactionId"].c_str());
                __NotificationCenter::getInstance()->postNotification("appstore_deleteLoad");
            }else
            {
                this->iosAppBuySuceesePostTimes++;
                
                Json::Value json;
                json["invoice"] = this->appStoreInvoice;
                json["receipt"] = this->IosReceipt;
                json["totalFee"] = this->iosTotalFee;
                CCHttpAgent::getInstance()->sendAppStoreFinishHttpPost(NULL,"notify/a", json.toStyledString(),"notify-a");
                
                cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ZJHModel::listnerPaySucceed),this,0.0,false);
                
            }
            /********************* 重试3次的请求 end **********************/
        }
        
        delete packet;
    }
    
#endif
}

void ZJHModel::dealAppUpdate()
{
    //处理android平台的升级
    if (getReferenceCount() == 0 || getReferenceCount() > 10)
    {
        return;
    }
    CCHttpPacket *checkUpdatePacket = CCHttpAgent::getInstance()->packets["check_update"];
    if (checkUpdatePacket && checkUpdatePacket->status != 0)
    {
        if (checkUpdatePacket->status != 3)
        {
            return;
        }
        
        checkUpdatePacket->status = 0;
        if (checkUpdatePacket->resultIsOK())
        {
            
            bool result = checkUpdatePacket->recvVal["resultMap"].isNull();
            if(result){
                return;
            }
            
            int isForce = checkUpdatePacket->recvVal["resultMap"]["version"]["force"].asInt();
            int curCode = atoi(checkUpdatePacket->recvVal["resultMap"]["version"]["code"].asCString());
            if (curCode > vcode)
            {
                //提示升级需要增加限制,一天提示一次
                if (isForce == 0)
                {
                    PlatformHelper::showUpdateDialog(checkUpdatePacket->recvVal["resultMap"]["version"]);
                }
                else
                {
                    //继续游戏  2、要升级不强制
                    double lastTime = UserDefault::getInstance()->getDoubleForKey("update_tips",0.0);
                    long long currentTime = Utils::getCurrentSecend();
                    double interval = (currentTime-lastTime)*0.001/60/60;
                    
                    log("＝＝＝ lastTime[%f]==currentTime[%lld]==interval[%f]",lastTime,currentTime, interval);
                    if (interval > 24)
                    {
                        UserDefault::getInstance()->setDoubleForKey("update_tips", currentTime);
                        PlatformHelper::showUpdateDialog(checkUpdatePacket->recvVal["resultMap"]["version"]);
                    }
                }
            }
        }
        CCHttpAgent::getInstance()->packets["check_update"] = NULL;
        delete checkUpdatePacket;
    }
}

void ZJHModel::startRoomHeart(){
//    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(ZJHModel::roomHearHttpRequest),this,2,false);
}

void ZJHModel::roomHearHttpRequest(float dt){
    if(curRoomID > 0){
        std::cout<<"==============startRoomHeart============"<<curRoomID<<endl;
    }
}

//游戏事件通知
//event:101 进入游戏失败  102 游戏命令异常 content为json后续自定义
void ZJHModel::noticeGamePlayEvent(cocos2d::Ref *ref){
    __String* str = (__String*)ref;
    if(str == NULL){
        return ;
    }
    Json::Value json = Utils::ParseJsonStr(str->getCString());
    log("===========noticeGamePlayEvent==========%s",json.toStyledString().c_str());
    string event = json["event"].asString();
    if(event == "101"){
        Json::Value param;
        Json::Value ary;
        param["httpCode"] = 0;
        param["businessCode"] = 600;
        param["content"] = json["content"].asString();
        param["produceTime"] = 0;
        param["requestUrl"] = "";
        param["requestParams"] = "";
        param["logInfo"]="";
        ary["logInfo"]="["+param.toStyledString()+"]";
        CCHttpAgent::getInstance()->sendHttpPost(NULL,"clientLog/upload",ary.toStyledString(), "noticeGamePlayEventError");
    }else if(event == "102"){
        Utils::gameCommandErrorRecord(json["content"].asString());
    }
}

//查询服务器数据  如果有则回调房间号
void ZJHModel::checkGamePlayingError(const MyCallBack_Str &callback){
    int playingCount = UserDefault::getInstance()->getIntegerForKey("lastRoomPlaying");
    if(playingCount == 0){
        if(callback != NULL){
            callback("normal");
        }
        return;
    }
    //请求服务器查看房间是否结束
    Json::Value json;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        if (loginPacket->status != 3)
        {
            return;
        }
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"];
            int roomList = data["currentList"].size();
            UserDefault::getInstance()->setIntegerForKey("lastRoomPlaying", roomList);
            if(roomList > 0){
                Json::Value room = data["currentList"][0U];
                string code = room["code"].asString();
                PlatformHelper::showToast("自动进入上次的房间");
                if(callback != NULL){
                    callback(code);
                }
            }else{
                if(callback != NULL){
                    callback("normal");
                }
            }
        }
        CCHttpAgent::getInstance()->packets[tag] = NULL;
        delete loginPacket;
    },"duiju/currentList",json.toStyledString(),"checkGamePlayingError");
}

void ZJHModel::setDiquCach(int cityId,std::string cityName)
{
    UserDefault::getInstance()->setIntegerForKey("diqu_cityId", cityId);
    UserDefault::getInstance()->setStringForKey("diqu_cityName", cityName);
    UserDefault::getInstance()->flush();
    
    bool need_add = true;
    int curSize = diquSelfCach.size();
    for (int i = 0; i < curSize; i ++)
    {
        if (diquSelfCach[i]["cityId"].asInt() == cityId)
        {
            need_add = false;
            break;
        }
    }
    
    if (need_add)
    {
        Json::Value json;
        int i = 0;
        json[i]["cityId"] = cityId;
        json[i]["cityName"] = cityName;
        
        for (int i = 0; i < curSize; i ++)
        {
            json[i + 1]["cityId"] = diquSelfCach[i]["cityId"].asInt();
            json[i + 1]["cityName"] = diquSelfCach[i]["cityName"].asString();
        }
        diquSelfCach = json;
        Utils::WriteFile("diquCache",diquSelfCach.toStyledString());
    }
}

void ZJHModel::toMatchHall()
{
    
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(ZJHModel::createTestMatchHall),this,0.1,false);
        return;
    
}

string ZJHModel::matchEnterName()
{
    string tempStr = ZJHModel::getInstance()->gameConfigJson["list"]["matchInfo"]["matchName"].asString();
    
    if(tempStr == "")
    {
        tempStr = "现金赛";
    }
   return  tempStr;
}

void ZJHModel::createTestMatchHall(float dt){
    cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(ZJHModel::createTestMatchHall),this);
//    #if COCOS2D_DEBUG == 1
//    JsUtils::initLoad();
//    JsUtils::runScript("matchHall/scripts/main.js");
//    return;
//    #endif
    hotUpdateGame(2109);
}

void ZJHModel::changeScreenSize(float screenWidth, float screenHeight){
    Size size = cocos2d::Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    bool currentScreenHorizen = true;//当前屏幕是横屏还是竖屏true横屏 false竖屏
    if(size.width < size.height)
    {
        currentScreenHorizen = false;
    }
    
    bool needHorizen = true;//当前是否需要横屏 true需要 false不需要
    if(screenWidth < screenHeight){
        //需要切换成竖屏
        needHorizen = false;
    }
    
    if(currentScreenHorizen && !needHorizen)
    {
        //如果当前是横的 且要切换竖屏 则要竖过来
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewH();
    }else if(!currentScreenHorizen && needHorizen){
        //如果当前是竖的且要切换成横屏  则要横过来
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewV();
    }
    glview->setDesignResolutionSize(screenWidth, screenHeight, ResolutionPolicy::EXACT_FIT);
}

void ZJHModel::hotUpdateGame(int gameId){
    Json::Value json;
    json["gameId"] = gameId;
    
    //检查脚本更新
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data;
            
            Json::Value& publicList = loginPacket->recvVal["resultMap"]["publicList"];
            Json::Value& list = loginPacket->recvVal["resultMap"]["list"];
            for(int i =0;i<publicList.size();++i)
            {
                string url =publicList[i]["jsurl"].asString();
                string pName = publicList[i]["entrance"].asString();
                
                Json::Value dat;
                dat["packageUrl"]="";
                dat["remoteManifestUrl"]=pName+"/version/project.manifest";
                dat["remoteVersionUrl"]=pName+"/version/version.manifest";
                dat["version"]="0";
                dat["assets"].append("");
                dat["assets"].clear();
                dat["searchPaths"].append("");
                dat["searchPaths"].clear();
                
                Json::Value dat1;
                dat1["baseData"]=dat.toStyledString();
                dat1["baseUrl"]=(url+"/");
                dat1["versionPath"]=(pName+"/version/");
                
                data["list"].append(dat1);
            }
            
            for(int i =0;i<list.size();++i)
            {
                if(list[i]["id"] == gameId)
                {
                    string url =list[i]["jsurl"].asString();
                    string pName = list[i]["entrance"].asString();
                    ZJHModel::getInstance()->packName =pName;
                    Json::Value dat;
                    dat["packageUrl"]="";
                    dat["remoteManifestUrl"]=pName+"/version/project.manifest";
                    dat["remoteVersionUrl"]=pName+"/version/version.manifest";
                    dat["version"]="0";
                    dat["assets"].append("");
                    dat["assets"].clear();
                    dat["searchPaths"].append("");
                    dat["searchPaths"].clear();
                    
                    Json::Value dat1;
                    dat1["baseData"]=dat.toStyledString();
                    dat1["baseUrl"]=(url+"/");
                    dat1["versionPath"]=(pName+"/version/");
                    
                    data["list"].append(dat1);
                    
                    data["entry"]=pName+"/scripts/main.js";
                    Director::getInstance()->replaceScene(HotUpdateLayer::createScene(data));
                    return;
                }
            }
            PlatformHelper::showToast("更新游戏失败,未找到资源包");
        }
    },"version/zipVersionList",json.toStyledString(),"check");
}

void ZJHModel::toMatchRoom(int gameid, int roomid)
{
    curGameType =gameid;
    hotUpdateGame(gameid);
}

void ZJHModel::setRoomData(std::string data)
{
    Json::Value value_date;
    if (data != "")
    {
        Json::Reader read;
        read.parse(data, value_date);
        roomData = value_date;
    }
}
