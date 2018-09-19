//
//  HotUpdateWithNormalView.cpp
//  ZJH
//  普通UI的热更新   不更换场景
//  Created by mac on 2017/5/2.
//
//

#include "HotUpdateWithNormalView.hpp"
#include "JsUtils.h"

USING_NS_CC;
USING_NS_CC_EXT;

Layout *HotUpdateWithNormalView::createScene(const Json::Value& dat)
{
    HotUpdateWithNormalView* layout = HotUpdateWithNormalView::create();
    layout->setVisible(false);
    layout->m_data = dat;
    layout->m_run = dat["entry"].asString();
    layout->currentGameId = dat["enterGameId"].asInt();
    layout->onLoadEnd(true);
    return layout;
}

void HotUpdateWithNormalView::runTestModule()
{
    JsUtils::initLoad();
    JsUtils::runScript("club/scripts/main.js");
}

void HotUpdateWithNormalView::hotUpdateModule(int gameId,Node* parent,bool useCache){
    return;
    string startKey = ZJHModel::getInstance()->gameDownloadMap[gameId];
    if(startKey != "" && useCache){
        log("========游戏%d=已经下载好 启动=======%s,",gameId,startKey.c_str());
        JsUtils::initLoad();
        JsUtils::runScript(startKey);
        return;
    }
    
    if(ZJHModel::getInstance()->loadingGameId == gameId){
        log("========游戏%d加载中========",gameId);
        return;
    }

    Json::Value json;
    json["gameId"] = gameId;
    
    //检查脚本更新
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
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
//                    ZJHModel::getInstance()->packName =pName;
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
                    data["enterGameId"] = gameId;
                    ZJHModel::getInstance()->loadingGameId = gameId;
                    Director::getInstance()->getRunningScene()->addChild(HotUpdateWithNormalView::createScene(data),10000);
                    return;
                }
            }
            PlatformHelper::showToast("更新游戏失败,未找到资源包");
        }
    },"version/zipVersionList",json.toStyledString(),"check_normal");
}

bool HotUpdateWithNormalView::init()
{
    if (!Layout::init())
    {
        return false;
    }
    tips = "游戏更新中(%d/%d)...";
    Size size = cocos2d::Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(size.width < size.height)
    {
        glview->setDesignResolutionSize(1080, 1920, ResolutionPolicy::EXACT_FIT);
    }else{
        glview->setDesignResolutionSize(1920, 1080, ResolutionPolicy::EXACT_FIT);
    }
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    _loadLayer = nullptr;
    _am = nullptr;
    _progress = nullptr;
    _amListener = nullptr;
    
    auto winSize = Director::getInstance()->getWinSize();
    _loadLayer = Layout::create();
    _loadLayer->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    _loadLayer->setBackGroundColor(Color3B(0, 0, 0));
    _loadLayer->setBackGroundColorOpacity(170);
    _loadLayer->setContentSize(winSize);
    _loadLayer->setTouchEnabled(true);
    _loadLayer->setVisible(false);
    addChild(_loadLayer);
    
    Sprite *loadText = Sprite::create("Loading/loading_txt.png");
    loadText->setPosition(Vec2(winSize.width / 2, winSize.height / 2 ));
    _loadLayer->addChild(loadText);
    
    Sprite *dots = Sprite::create("Loading/loading_icon.png");
    dots->setPosition(Vec2(winSize.width / 2, winSize.height / 2 +140));
    _loadLayer->addChild(dots);
    dots->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    
    Color3B c =Color3B(255,230,129);
    _tip = Label::createWithSystemFont("", "a", 32);
    _tip->setPosition( Vec2(winSize.width*0.5f,winSize.height / 2 - 300) );
    _tip->setColor(c);
    _loadLayer->addChild(_tip);
    
    _loadingBg = ImageView::create("Loading/loading_bar_bg.png");
    _loadingBg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 160));
    _loadLayer->addChild(_loadingBg);
    
    _loading = LoadingBar::create("Loading/loading_bar_fg.png");
    _loading->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 160));
    _loading->setDirection(LoadingBar::Direction::LEFT);
    _loading->setPercent(0);
    _loadingBg->setVisible(false);
    
    _loadLayer->addChild(_loading);
    _progress = Label::createWithSystemFont("", "a", 32);
    _progress->setPosition( Vec2(winSize.width*0.5f,winSize.height / 2 - 210) );
    
    _progress->setColor(c);
    _loadLayer->addChild(_progress);
    
    frist = false;
    m_index =0;
    _am = nullptr;
    return true;
}

void HotUpdateWithNormalView::onLoadEnd(bool is)
{
    CCLOG("onLoadEnd：%d",is?1:0);
    if(is)
    {
        if(m_index < m_data["list"].size())
        {
            if(_am)
            {
                CC_SAFE_RELEASE_NULL(_am);
            }
            
            Manifest::_baseUrl =m_data["list"][m_index]["baseUrl"].asString();
            std::string manifestPath = m_data["list"][m_index]["versionPath"].asString()+"project.manifest", storagePath = Utils::getSavePath();
            if(!FileUtils::getInstance()->isFileExist(manifestPath))
            {
                frist = true;
//                _tip->setString(StringUtils::format("首次进入，游戏正在安装中(%d/%d)...",m_index,m_data["list"].size()) );
                _tip->setString(StringUtils::format(tips.c_str(),m_index,m_data["list"].size()) );
                manifestPath = m_data["list"][m_index]["baseData"].asString();
            }
            else
            {
            }
            _am = AssetsManagerEx::create(manifestPath, storagePath,m_data["list"][m_index]["versionPath"].asString());
            _am->retain();
            m_index++;
            
            startDownloadCallback();
        }
        else
        {
            this->runAction(Sequence::create(DelayTime::create(0.01),CallFunc::create([=](){
                ZJHModel::getInstance()->loadingGameId = 0;
                JsUtils::initLoad();
                JsUtils::runScript(m_run);
                ZJHModel::getInstance()->gameDownloadMap[currentGameId] = m_run;
                _am->release();
                this->removeFromParent();
            }), NULL));
        }
        
    }
    else
    {
        PlatformHelper::showToast("资源更新失败，请重试！");
        postNotify(3,0,0,0);
        ZJHModel::getInstance()->loadingGameId = 0;
        FileUtils::getInstance()->purgeCachedEntries();
        this->runAction(Sequence::create(DelayTime::create(0.01),CallFunc::create([=](){
            _am->release();
            _loadLayer->removeFromParent();
        }), NULL));
    }
}

void HotUpdateWithNormalView::startDownloadCallback()
{
    if (!_am->getLocalManifest()->isLoaded())
    {
        CCLOG("Fail to update assets, step skipped.");
        onLoadEnd(true);
    }
    else
    {
        if(_amListener)
        {
            
            Director::getInstance()->getEventDispatcher()->removeEventListener(_amListener);
            
        }
        _amListener = cocos2d::extension::EventListenerAssetsManagerEx::create(_am, [=](EventAssetsManagerEx* event){
            
            CCLOG("check update CB:%d",(int)event->getEventCode());
            bool isFail =false;
            bool restart =false;
            
            switch (event->getEventCode())
            {
                case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                {
                    CCLOG("No local manifest file found, skip assets update.");
                    restart = true;
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                {
                    CCLOG("Fail to download manifest file, update skipped.");
                    isFail = true;
                    
                }
                    break;
                case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                {
                    
                    CCLOG("Update finished. %s", event->getMessage().c_str());
                    restart = true;
                    
                }
                    break;
                case EventAssetsManagerEx::EventCode::NEW_VERSION_FOUND:
                {
                    Director::getInstance()->getEventDispatcher()->removeEventListener(_amListener);
                    
                    _amListener = cocos2d::extension::EventListenerAssetsManagerEx::create(_am, [=](EventAssetsManagerEx* event){
                        static int failCount = 0;
                        bool updateFail = false;
                        bool restart =false;
                        bool back = false;
                        CCLOG("NEW_VERSION_FOUND CB:%d",(int)event->getEventCode());
                        switch (event->getEventCode())
                        {
                            case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                            {
                                CCLOG("No local manifest file found, skip assets update.");
                                restart=true;
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
                            {
                                std::string assetId = event->getAssetId();
                                float percent = event->getPercentByFile();
                                std::string str;
                                if (assetId == AssetsManagerEx::VERSION_ID)
                                {
                                    str = "";
                                }
                                else if (assetId == AssetsManagerEx::MANIFEST_ID)
                                {
                                    str = "";
                                }
                                else
                                {
                                    if(!frist)
                                    {
//                                        _tip->setString(StringUtils::format("发现新版本，游戏正在更新中(%d/%d)...",m_index,m_data["list"].size()) );
                                        _tip->setString(StringUtils::format(tips.c_str(),m_index,m_data["list"].size()) );
                                    }
                                    else
                                    {
                                        
//                                        _tip->setString(StringUtils::format("首次进入，游戏正在安装中(%d/%d)...",m_index,m_data["list"].size()) );
                                        _tip->setString(StringUtils::format(tips.c_str(),m_index,m_data["list"].size()) );
                                    }
                                    str = StringUtils::format("%.2f%%", percent);
                                    _loading->setPercent(percent);
                                    _loadingBg->setVisible(true);
                                    _loadLayer->setVisible(true);
                                    postNotify(1, percent,m_index,m_data["list"].size());
                                    CCLOG("%.2f Percent", percent);
                                }
                                if (this->_progress != nullptr)
                                    this->_progress->setString(str);
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                            case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                            {
                                CCLOG("Fail to download manifest file, update skipped.");
                                updateFail=true;
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                            case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
                            {
                                CCLOG("Update finished. %s", event->getMessage().c_str());
                                std::string assetId = event->getAssetId();
                                restart=true;
                                
                                if (assetId == AssetsManagerEx::VERSION_ID)
                                {
                                }
                                else if (assetId == AssetsManagerEx::MANIFEST_ID)
                                {
                                }
                                else
                                {
                                    
                                    if(m_index < m_data["list"].size())
                                    {
                                        // _tip->setString("加载中。。。");
                                    }
                                    else
                                    {
                                        _tip->setString("资源更新成功");
                                        ZJHModel::getInstance()->loadingGameId = 0;
                                        postNotify(2, 100,0,0);
                                    }
                                    this->_progress->setString("");
                                }
                                
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::UPDATE_FAILED:
                            {
                                CCLOG("Update failed. %s", event->getMessage().c_str());
                                
                                failCount++;
                                if (failCount < 5)
                                {
                                    _am->downloadFailedAssets();
                                }
                                else
                                {
                                    CCLOG("Reach maximum fail count, exit update process");
                                    failCount = 0;
                                    back=true;
                                    
                                }
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ERROR_UPDATING:
                            {
                                CCLOG("Asset %s : %s", event->getAssetId().c_str(), event->getMessage().c_str());
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
                            {
                                CCLOG("%s", event->getMessage().c_str());
                            }
                                break;
                            default:
                                break;
                        }
                        if(updateFail)
                        {
                            this->onLoadEnd(false);
                        }
                        else if(back)
                        {
                            this->onLoadEnd(false);
                        }
                        else if(restart)
                        {
                            this->onLoadEnd(true);
                        }
                        
                    });
                    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_amListener, 1);
                    _am->update();
                }
                    break;
                default:
                    break;
            }
            if(isFail)
            {
                this->onLoadEnd(false);
            }
            else if(restart)
            {
                this->onLoadEnd(true);
            }
            
        });
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_amListener, 1);
        
        _am->checkUpdate();
        
        auto size = Director::getInstance()->getWinSize();    }
}

void HotUpdateWithNormalView::postNotify(int event, float process,int currenIndex,int total){//event 1进度 2成功  3失败
    Json::Value postJson;
    postJson["gameId"] = ZJHModel::getInstance()->loadingGameId;
    postJson["currenIndex"] = currenIndex;
    postJson["total"] = total;
    postJson["percent"] = process;
    postJson["event"] = event;
    __NotificationCenter::getInstance()->postNotification("notify_game_update_process",__String::create(postJson.toStyledString()));
}

HotUpdateWithNormalView::~HotUpdateWithNormalView(){
    ZJHModel::getInstance()->loadingGameId = 0;
}

void HotUpdateWithNormalView::onExit()
{
//    ZJHModel::getInstance()->loadingGameId = 0;
    _eventDispatcher->removeEventListener(_amListener);
//    _am->release();
    Layout::onExit();
}


