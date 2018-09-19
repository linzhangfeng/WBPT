//
//  SetCityLayer.cpp
//  ZJH
//
//  Created by apple on 16/8/7.
//
//

#include "SetCityLayer.hpp"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

bool SetCityLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 创建一个事件监听器类型为 OneByOne 的单点触摸
    auto listener1 = EventListenerTouchOneByOne::create();
    // 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
    listener1->setSwallowTouches(true);
    
    // 使用 lambda 实现 onTouchBegan 事件回调函数
    listener1->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        // 获取事件所绑定的 target
        return true;
    };
    
    // 点击事件结束处理
    listener1->onTouchEnded = [=](Touch* touch, Event* event)
    {
        this->removeFromParent();
        
    };
    EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    Layout *bg = Layout::create();
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    bg->setContentSize(Size(visibleSize.width, 630));
    this->addChild(bg);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 625));
    line->setContentSize(Size(visibleSize.width, 5));
    bg->addChild(line);
    
    Button *sureBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(960, 120));
    sureBtn->setPosition(Vec2(visibleSize.width/2, 80));
    bg->addChild(sureBtn);
    sureBtn->setTitleText("确认");
    sureBtn->setTitleFontSize(40);
    sureBtn->setTitleColor(Color3B(255, 255, 255));
    sureBtn->setTitleFontName(".SFUIDisplay-Semibold");
    sureBtn->addClickEventListener([=](Ref*ref){
    
        vector<string> quTemp = districtArray[proviceArray[index1] + cityArray[index1][index2]];
        string cityChange = proviceArray[index1] + " "+ cityArray[index1][index2] + " " + quTemp[index3];
        
        if (cityChange != ZJHModel::getInstance()->userCity)
        {
            Json::Value json;
            json["province"] = proviceArray[index1];
            json["city"] = cityArray[index1][index2];
            json["area"] = quTemp[index3];
            json["address"] = "";
            
            CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
                
                CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
                
                if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)
                {
                    return;
                }
                if (loginPacket->status != 3)
                {
                    PlatformHelper::showToast("网络链接失败，请稍后再试");
                    return;
                }
                
                if (loginPacket->resultIsOK())
                {
                    Json::Value data = loginPacket->recvVal["resultMap"];
                    //签名修改成功
                    PlatformHelper::showToast("地区修改成功");
                    if (proviceArray[index1] == cityArray[index1][index2])
                    {
                        ZJHModel::getInstance()->userCity = cityArray[index1][index2] + " " + quTemp[index3];
                    }else
                    {
                        ZJHModel::getInstance()->userCity = cityChange;
                    }
                    ZJHModel::getInstance()->provice = proviceArray[index1];
                    ZJHModel::getInstance()->city = cityArray[index1][index2];
                    ZJHModel::getInstance()->qu = quTemp[index3];

                    __NotificationCenter::getInstance()->postNotification("adress_notify");
                    
                    this->removeFromParent();
                }else
                {
                    PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                }
            },"/account/updProvinceCity",json.toStyledString(),"City");
        }

    });
    
    Button *cancel = Button::create("Mine/yellow_cancel_btn.png","Mine/yellow_cancel_btn.png");
    cancel->setPosition(Vec2(bg->getContentSize().width - cancel->getContentSize().width, bg->getContentSize().height - cancel->getContentSize().height));
    cancel->addClickEventListener([=](Ref*ref){
    
        this->removeFromParent();
    });
    
    bg->addChild(cancel);
    
    auto file_path = FileUtils::getInstance()->fullPathForFilename("Mine/province_data.json");
    
    auto d =FileUtils::getInstance()->getStringFromFile(file_path);
    Json::Reader r;
     Json::Value data;
    r.parse(d, data);
    for (unsigned int i = 0; i < data["root"]["province"].size(); ++i) {
        auto& p1 =data["root"]["province"][i];
         proviceArray.push_back(p1["-name"].asString());
        
        vector<string> cityTemp;
        if(p1["city"].type() == Json::ValueType::arrayValue)
        {
            
        for (unsigned int j = 0; j < p1["city"].size(); ++j) {
            auto& p2 =p1["city"][j];
            cityTemp.push_back(p2["-name"].asString());
            
            vector<string> districtTemp;
            
            for (unsigned int z = 0; z < p2["district"].size(); ++z) {
                auto& p3 =p2["district"][z];
                districtTemp.push_back(p3["-name"].asString());
                
            }
            string str1 = p1["-name"].asString();
            string str2 = p2["-name"].asString();
            
            districtArray[str1 + str2] = districtTemp;
            
        }
        }
        else if (p1["city"].type() == Json::ValueType::objectValue)
        {
            auto& p2 =p1["city"];
            cityTemp.push_back(p2["-name"].asString());
            
            vector<string> districtTemp;
            
            for (unsigned int z = 0; z < p2["district"].size(); ++z) {
                auto& p3 =p2["district"][z];
                districtTemp.push_back(p3["-name"].asString());
                
            }
            string str1 = p1["-name"].asString();
            string str2 = p2["-name"].asString();
            
            districtArray[str1 + str2] = districtTemp;

        }


    
        cityArray.push_back(cityTemp);

        
    }
   /* XMLDocument* myDocment = new tinyxml2::XMLDocument();
    //加载文件
    myDocment->LoadFile(file_path);
    
    XMLElement* rootElement = myDocment->RootElement();

    XMLElement* proviceElement = rootElement->FirstChildElement();
    while (proviceElement)
    {
        const XMLAttribute* attri = proviceElement->FirstAttribute();
        proviceArray.push_back(attri->Value());

        XMLElement* cityElement = proviceElement->FirstChildElement();
        vector<string> cityTemp;
        while (cityElement)
        {
            const XMLAttribute* attriCity = cityElement->FirstAttribute();
            
            cityTemp.push_back(attriCity->Value());
            log("city%s\n", attriCity->Value());
            
            XMLElement* quElement = cityElement->FirstChildElement();
            vector<string> districtTemp;
            while (quElement)
            {
                const XMLAttribute* attriQu = quElement->FirstAttribute();
                districtTemp.push_back(attriQu->Value());
                log("%s\n", attriQu->Value());
                quElement = quElement->NextSiblingElement();
            }
            string str1 = attri->Value();
            string str2 = attriCity->Value();
            
            districtArray[str1 + str2] = districtTemp;
            
            cityElement = cityElement->NextSiblingElement();
        }
        cityArray.push_back(cityTemp);
        proviceElement = proviceElement->NextSiblingElement();

    }
    */
    showProvice();
    
    return true;
}

void SetCityLayer::showProvice()
{
    for (int i = 0; i < 3; i++)
    {
        ImageView *im = ImageView::create("Mine/setHome_bg.png");
        im->setAnchorPoint(Vec2(0, 0));
        im->setContentSize(Size(235, 240));
        im->setScale9Enabled(true);
        this->addChild(im);
        switch (i)
        {
            case 0:
                im->setPosition(Vec2(147, 250));
                break;
            case 1:
                im->setPosition(Vec2(425, 250));
                break;
            case 2:
                im->setPosition(Vec2(700, 250));
                break;
        }
        
    }
    listView1 = ListView::create();
    listView1->setDirection(ScrollView::Direction::VERTICAL);
    listView1->setBounceEnabled(false);
    listView1->setContentSize(Size(235, 240));
    listView1->setAnchorPoint(Vec2(0, 0));
    listView1->setPosition(Vec2(147, 250));
    listView1->setItemsMargin(0);
    listView1->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView1);
    
    Size cellSize = Size(235, 80);
    
    index1 = 0;
    index2 = 0;
    index3 = 0;
    for (int i = 0; i < proviceArray.size(); i++)
    {
        if (ZJHModel::getInstance()->provice  == proviceArray[i])
        {
            index1 = i;
            break;
        }
    }
    
    for (int i = 0; i < cityArray[index1].size(); i++)
    {
        if (ZJHModel::getInstance()->city == cityArray[index1][i])
        {
            index2 = i;
            break;
        }
    }
    
    
    vector<string> quTemp = districtArray[proviceArray[index1] + cityArray[index1][index2]];
    for (int i = 0; i < quTemp.size(); i++)
    {
        if (ZJHModel::getInstance()->qu == quTemp[i])
        {
            index3 = i;
            break;
        }
    }
    
    for (int i = 0; i < proviceArray.size(); i++)
    {
        auto cell = Layout::create();
        cell->setTag(100+i);
        cell->setTouchEnabled(true);
        cell->setContentSize(cellSize);
        cell->addTouchEventListener(CC_CALLBACK_2(SetCityLayer::clickList1, this));
        listView1->addChild(cell);
        
        auto line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->ignoreAnchorPointForPosition(false);
        line->setAnchorPoint(Vec2(0, 0));
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setContentSize(Size(cellSize.width, 3));
        cell->addChild(line);
        
        Text *title = Text::create(proviceArray[i], ".SFUIDisplay-Semibold", 36);
        title->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
        title->setTag(50);
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        title->setPosition(Vec2(cellSize.width/2, cellSize.height/2));
        cell->addChild(title);
        
        if(index1 == i)
        {
            title->setTextColor(Color4B::BLACK);
            title->setFontSize(50);
        }
    }
    
    CallFunc* func1 = CallFunc::create([=]
                                      {
                                          listView1->jumpToPercentHorizontal(index1*cellSize.height);
                                      });
    
    runAction(Sequence::create(DelayTime::create(0.5),func1, NULL));
    
    listView2 = ListView::create();
    listView2->setDirection(ScrollView::Direction::VERTICAL);
    listView2->setBounceEnabled(false);
    listView2->setContentSize(Size(235, 240));
    listView2->setAnchorPoint(Vec2(0, 0));
    listView2->setPosition(Vec2(425, 250));
    listView2->setItemsMargin(0);
    listView2->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView2);

    for (int i = 0; i < cityArray[index1].size(); i++)
    {
        auto cell = Layout::create();
        cell->setTag(200+i);
        cell->setTouchEnabled(true);
        cell->setContentSize(cellSize);
        cell->addTouchEventListener(CC_CALLBACK_2(SetCityLayer::clickList2, this));
        listView2->addChild(cell);
        
        auto line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->ignoreAnchorPointForPosition(false);
        line->setAnchorPoint(Vec2(0, 0));
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setContentSize(Size(cellSize.width, 3));
        cell->addChild(line);
        
        Text *title = Text::create(cityArray[index1][i], ".SFUIDisplay-Semibold", 36);
       title->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
        title->setTag(50);
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        title->setPosition(Vec2(cellSize.width/2, cellSize.height/2));
        cell->addChild(title);
        if(index2 == i)
        {
            title->setTextColor(Color4B::BLACK);
            title->setFontSize(50);
        }
    }
    
    CallFunc* func2 = CallFunc::create([=]
                                       {
                                           listView2->jumpToPercentHorizontal(index2*cellSize.height);
                                       });
    
    runAction(Sequence::create(DelayTime::create(0.5),func2, NULL));
    
    listView3 = ListView::create();
    listView3->setDirection(ScrollView::Direction::VERTICAL);
    listView3->setBounceEnabled(false);
    listView3->setContentSize(Size(235, 240));
    listView3->setAnchorPoint(Vec2(0, 0));
    listView3->setPosition(Vec2(700, 250));
    listView3->setItemsMargin(0);
    listView3->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(listView3);
    
    vector<string> temp3 = districtArray[proviceArray[index1] + cityArray[index1][index2]];
    
    for (int i = 0; i < temp3.size(); i++)
    {
        auto cell = Layout::create();
        cell->setTag(300+i);
        cell->setTouchEnabled(true);
        cell->setContentSize(cellSize);
        cell->addTouchEventListener(CC_CALLBACK_2(SetCityLayer::clickList3, this));
        listView3->addChild(cell);
        
        auto line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->ignoreAnchorPointForPosition(false);
        line->setAnchorPoint(Vec2(0, 0));
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setContentSize(Size(cellSize.width, 3));
        cell->addChild(line);
        
        Text *title = Text::create(temp3[i], ".SFUIDisplay-Semibold", 36);
        title->setTag(50);
        title->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        title->setPosition(Vec2(cellSize.width/2, cellSize.height/2));
        cell->addChild(title);
        
        if(index3 == i)
        {
            title->setTextColor(Color4B::BLACK);
            title->setFontSize(50);
        }
    }
    
    CallFunc* func3 = CallFunc::create([=]
                                       {
                                           listView3->jumpToPercentHorizontal(index3*cellSize.height);
                                       });
    runAction(Sequence::create(DelayTime::create(0.5),func3, NULL));
}

void SetCityLayer::updateViewList2()
{
    Size cellSize = Size(235, 80);
    for (int i = 0; i < cityArray[index1].size(); i++)
    {
        auto cell = Layout::create();
        cell->setTag(200+i);
        cell->setTouchEnabled(true);
        cell->setContentSize(cellSize);
        cell->addTouchEventListener(CC_CALLBACK_2(SetCityLayer::clickList2, this));
        listView2->addChild(cell);
        
        auto line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->ignoreAnchorPointForPosition(false);
        line->setAnchorPoint(Vec2(0, 0));
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setContentSize(Size(cellSize.width, 3));
        cell->addChild(line);
        
        Text *title = Text::create(cityArray[index1][i], ".SFUIDisplay-Semibold", 36);
        title->setTag(50);
        title->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        title->setPosition(Vec2(cellSize.width/2, cellSize.height/2));
        cell->addChild(title);
        
        if(i == 0)
        {
            title->setTextColor(Color4B::BLACK);
            title->setFontSize(50);
            
        }
    }
    listView2->scrollToTop(0, true);
}
void SetCityLayer::updateViewList3()
{
    Size cellSize = Size(235, 80);
    string temp = proviceArray[index1] + cityArray[index1][index2];
    
    for (int i = 0; i < districtArray[temp].size(); i++)
    {
        auto cell = Layout::create();
        cell->setTag(300+i);
        cell->setTouchEnabled(true);
        cell->setContentSize(cellSize);
        cell->addTouchEventListener(CC_CALLBACK_2(SetCityLayer::clickList3, this));
        listView3->addChild(cell);
        
        auto line = Layout::create();
        line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        line->ignoreAnchorPointForPosition(false);
        line->setAnchorPoint(Vec2(0, 0));
        line->setBackGroundColor(Color3B(0x4d, 0x4d, 0x4d));
        line->setContentSize(Size(cellSize.width, 3));
        cell->addChild(line);
        
        Text *title = Text::create(districtArray[temp][i], ".SFUIDisplay-Semibold", 36);
        title->setTag(50);
        title->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        title->setPosition(Vec2(cellSize.width/2, cellSize.height/2));
        cell->addChild(title);
        if(i == 0)
        {
            title->setTextColor(Color4B::BLACK);
            title->setFontSize(50);
            
        }
    }
    listView3->scrollToTop(0, true);
}

void SetCityLayer::clickList1(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout *layout1 = (Layout *)pSender;
        int curIndex1 = layout1->getTag() - 100;
        Text *curText = (Text*)layout1->getChildByTag(50);
        
        Layout *lastLayout1 = (Layout *)listView1->getChildByTag(index1 + 100);
        Text *lastText = (Text*)lastLayout1->getChildByTag(50);
        
        if(curIndex1 != index1)
        {
            curText->setTextColor(Color4B::BLACK);
            curText->setFontSize(50);
            
            lastText->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
            lastText->setFontSize(36);
            
            listView2->removeFromParent();
            
            listView2 = ListView::create();
            listView2->setDirection(ScrollView::Direction::VERTICAL);
            listView2->setBounceEnabled(false);
            listView2->setContentSize(Size(235, 240));
            listView2->setAnchorPoint(Vec2(0, 0));
            listView2->setPosition(Vec2(425, 250));
            listView2->setItemsMargin(0);
            listView2->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
            this->addChild(listView2);
            
            listView3->removeFromParent();
            listView3 = ListView::create();
            listView3->setDirection(ScrollView::Direction::VERTICAL);
            listView3->setBounceEnabled(false);
            listView3->setContentSize(Size(235, 240));
            listView3->setAnchorPoint(Vec2(0, 0));
            listView3->setPosition(Vec2(700, 250));
            listView3->setItemsMargin(0);
            listView3->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
            this->addChild(listView3);
            
            index1 = curIndex1;
            index2 = 0;
            index3 = 0;
            updateViewList2();
            updateViewList3();
        }
    }
}
void SetCityLayer::clickList2(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout *layout2 = (Layout *)pSender;
        int curIndex2 = layout2->getTag() - 200;
        Text *curText = (Text*)layout2->getChildByTag(50);
        
        Layout *lastLayout2 = (Layout *)listView2->getChildByTag(index2 + 200);
        Text *lastText = (Text*)lastLayout2->getChildByTag(50);
        
        if(curIndex2 != index2)
        {
            curText->setTextColor(Color4B::BLACK);
            curText->setFontSize(50);
            
            lastText->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
            lastText->setFontSize(36);
            
            listView3->removeFromParent();
            listView3 = ListView::create();
            listView3->setDirection(ScrollView::Direction::VERTICAL);
            listView3->setBounceEnabled(false);
            listView3->setContentSize(Size(235, 240));
            listView3->setAnchorPoint(Vec2(0, 0));
            listView3->setPosition(Vec2(700, 250));
            listView3->setItemsMargin(0);
            listView3->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
            this->addChild(listView3);
            
            index2 = curIndex2;
            updateViewList3();
        }
    }
}
void SetCityLayer::clickList3(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Layout *layout3 = (Layout *)pSender;
        int curIndex3 = layout3->getTag() - 300;
        Text *curText = (Text*)layout3->getChildByTag(50);
        
        Layout *lastLayout3 = (Layout *)listView3->getChildByTag(index3 + 300);
        Text *lastText = (Text*)lastLayout3->getChildByTag(50);
        
        if(curIndex3 != index3)
        {
            curText->setTextColor(Color4B::BLACK);
            curText->setFontSize(50);
            
            lastText->setTextColor(Color4B(0x4d, 0x4d, 0x4d, 255));
            lastText->setFontSize(36);
            
            index3 = curIndex3;
        }

    }
}


