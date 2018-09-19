//
//  HotUpdateLayer.cpp
//  ZJH
//
//  Created by mac on 16/10/12.
//
//

#include "HotUpdateLayer.hpp"
#include "JsUtils.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene *HotUpdateLayer::createScene(const Json::Value& dat)
{
    Scene *scene = Scene::create();
    auto layer = HotUpdateLayer::create();
    scene->addChild(layer,1,"update");
    layer->m_data = dat;
    layer->m_run = dat["entry"].asString();

    layer->onLoadEnd(true);

    return scene;
}

bool HotUpdateLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
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
    _loadLayer = Layer::create();
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
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(ZJHModel::getInstance()->bgMusic);
    SimpleAudioEngine::getInstance()->setEffectsVolume(ZJHModel::getInstance()->sound);

    return true;
}

void HotUpdateLayer::onLoadEnd(bool is)
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
                _tip->setString(StringUtils::format("首次进入，游戏正在安装中(%d/%d)...",m_index,m_data["list"].size()) );
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
             JsUtils::initLoad();
             JsUtils::runScript(m_run);
            }), NULL));
        }

    }
    else
    {
        PlatformHelper::showToast("资源更新失败，请重试！");
        FileUtils::getInstance()->purgeCachedEntries();
        this->runAction(Sequence::create(DelayTime::create(0.01),CallFunc::create([=](){
            ZJHModel::getInstance()->roomToHall(false);
        }), NULL));
    }
}

void HotUpdateLayer::startDownloadCallback()
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
                                        _tip->setString(StringUtils::format("发现新版本，游戏正在更新中(%d/%d)...",m_index,m_data["list"].size()) );
                                    }
                                    else
                                    {
                                        
                                        _tip->setString(StringUtils::format("首次进入，游戏正在安装中(%d/%d)...",m_index,m_data["list"].size()) );
                                    }
                                    str = StringUtils::format("%.2f%%", percent);
                                    _loading->setPercent(percent);
                                    _loadingBg->setVisible(true);

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
                                        _tip->setString("资源更新成功，正在进入游戏");
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
void HotUpdateLayer::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(_amListener);
    _am->release();
}
