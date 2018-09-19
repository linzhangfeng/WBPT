//
//  AgreementLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/8/12.
//
//

#include "AgreementLayer.h"
#include "GameLoad.hpp"

USING_NS_CC;

Scene *AgreementLayer::createScene()
{
    Scene *scene = Scene::create();
    AgreementLayer *layer = AgreementLayer::create();
    scene->addChild(layer);
    return scene;
}

bool AgreementLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_bHaveRead = true;
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite *bg = Sprite::create("Load/agreement_bg.jpg");
    bg->setScale(Utils::getMaxScale());
    bg->setPosition(winSize / 2);
    this->addChild(bg);
    
    float textWidth = winSize.width - 200;
    Size dimSize = Size(winSize.width - 300, 0);
    Size listSize = Size(winSize.width - 200, winSize.height - 600);
    Vec2 listPos = Vec2(winSize.width / 2, winSize.height / 2 + 100);
    
    ui::Scale9Sprite *listBg = ui::Scale9Sprite::create("Load/panel_bg.9.2.png");
    listBg->setContentSize(listSize);
    listBg->setAnchorPoint(Vec2(0.5, 0.5));
    listBg->setPosition(listPos);
    this->addChild(listBg);
    
    Node *textNode = Node::create();
    textNode->setContentSize(Size(textWidth, 1));
    float tFontSize = 44;
    float cFontSize = 34;
    float posX = (textWidth - dimSize.width) / 2;
    float posY = -5;
    float padding = 20;
    float textHeight = 20;
    
    Label *agreement = Label::createWithSystemFont("《玩呗娱乐用户协议》", "Arial-BoldMT", 48);
    agreement->setAnchorPoint(Vec2(0.5, 1));
    agreement->setPosition(Vec2(textWidth / 2, posY));
    textNode->addChild(agreement);
    
    Size labelSize = agreement->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    
    Label *textLabel = Label::createWithSystemFont("一、特别提示", "Arial", tFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    
    textLabel = Label::createWithSystemFont("        玩呗娱乐同意按照本协议的规定及其不时发布的操作规则提供基于互联网的相关服务(以下称“网络服务”)，为获得网络服务，服务使用人(以下称“用户”)同意本协议的全部条款并按照页面上的提示完成全部的注册程序。用户在进行注册程序过程中点击“同意”按钮即表示用户完全接受本协议项下的全部条款。这些条款可由移动棋牌随时更新，修改后的服务协议一旦在页面上公布即有效代替原来的服务协议。用户在使用玩呗娱乐提供的各项服务之前，应仔细阅读本服务协议，如用户不同意本服务协议及/或随时对其的修改，请停止使用玩呗娱乐提供的服务,本条款是玩呗娱乐和用户之间，与苹果公司无关，但遵循苹果商店服务条款。", "Arial", cFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    
    textLabel = Label::createWithSystemFont("二、著作权声明", "Arial", tFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    textLabel = Label::createWithSystemFont("        玩呗娱乐所提供的服务的相关著作权、专利权、商标、营业秘密及其它任何所有权，权利和维护，均属玩呗娱乐所有。非经玩呗娱乐的同意，任何人或用户均不得擅自重制、传输、改作、编辑，否则应负所有法律责任。", "Arial", cFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    textLabel = Label::createWithSystemFont("三、用户的基本义务", "Arial", tFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    textLabel = Label::createWithSystemFont("（1）用户有义务妥善保管在注册服务时获得的帐号及密码谨防被盗或泄露，并为此帐号及密码登入系统后所开始的一连串行为或活动负责。因密码被盗或泄露造成的全部责任和损失均由用户本人承担，玩呗娱乐对此概不负责。\n（2）若用户发现帐号或密码遭他人非法使用或有异常使用的情形，应立即通知玩呗娱乐，并提交该帐号为本人所有的有关证明，以便申请该帐号的暂停登录和使用。因此而造成的损失玩呗娱乐不承担赔偿责任。\n（3）玩呗娱乐根据用户的通知采取措施暂停用户帐号的登录和使用的，移动棋牌应当要求用户提供并核实与其注册身份信息相一致的个人有效身份信息。\n（4）用户没有提供其个人有效身份证件或者用户提供的个人有效身份证件与所注册的身份信息不一致的，玩呗娱乐有权拒绝用户上述请求。\n（5）用户因忘记密码或密码被盗向玩呗娱乐查询密码时，须提供完全正确的注册信息（包括注册时间、用户帐号、手机号码、等信息），否则玩呗娱乐有权本着为用户保密的原则不予告知。\n（6）用户同意若发现任何非法使用用户帐号或利用安全漏洞的情况，立即通告玩呗娱乐，且须承诺在申请帐号时遵循如下规定：\n        a、用户名和中文昵称的注册及使用须尊重网络道德，遵守中华人民共和国的有关法律法规。 \n        b、用户名和中文昵称中不得含有任何威胁、恐吓、漫骂、庸俗、亵渎、色情、淫秽、非法、反动、前后矛盾、攻击性、伤害性、骚扰性、诽谤性、辱骂性的或侵害他人知识产权的文字。不得涉及政治、国家领导人及近音国家领导人，以及任何可能引起法律纠纷的文字。\n（7）若用户将帐号与他人于现实生活中进行交易或互易行为而产生的纠纷，玩呗娱乐概不负责。\n（8）用户使用在玩呗娱乐注册的用户名在玩呗娱乐中的任何行为需要遵守玩呗娱乐其他各项服务条款。\n（9）用户对移动棋牌服务管理人员所进行的询问应据实澄清，否则玩呗娱乐有权随时终止用户使用服务。", "Arial", cFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    textLabel = Label::createWithSystemFont("四、用户隐私制度", "Arial", tFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    textLabel = Label::createWithSystemFont("        玩呗娱乐绝对不会修改用户的个人资料，或编辑或透露用户在注册资料中的密码、姓名、电话、等信息及保存在玩呗娱乐中的非公开内容，除非有法律许可要求或玩呗娱乐在诚信的基础上认为透露这些信息在以下四种情况是必要的：\n        （1）遵守有关法律规定，遵从合法服务程序。\n        （2）维护玩呗娱乐的商标所有权。\n        （3）在紧急情况下竭力维护用户个人和社会大众的隐私安全。\n        （4）符合其他相关的要求。", "Arial", cFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    textLabel = Label::createWithSystemFont("五、责任描述", "Arial", tFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    textLabel = Label::createWithSystemFont("        本协议与苹果公司无关，最终解释权归玩呗娱乐公司所有。\n        电话：0755-86329809\n        地址：深圳市南山区科兴科技园B栋1单元12楼\n        邮箱：hr@ihuizhi.com", "Arial", cFontSize, dimSize, TextHAlignment::LEFT);
    textLabel->setAnchorPoint(Vec2(0, 1));
    textLabel->setPosition(Vec2(posX, posY));
    textNode->addChild(textLabel);
    
    labelSize = textLabel->getContentSize();
    posY = posY - padding - labelSize.height;
    textHeight = textHeight + padding + labelSize.height;
    
    Layout *container = Layout::create();
    container->setContentSize(Size(textWidth, textHeight));
    
    textNode->setPosition(Vec2(0, textHeight));
    container->addChild(textNode);
    
    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(listSize);
    resultList->setAnchorPoint(Vec2(0.5, 0.5));
    resultList->setPosition(listPos);
    resultList->setItemsMargin(15);
    resultList->setScrollBarEnabled(false);
    this->addChild(resultList);
    
    resultList->addChild(container);
    resultList->refreshView();
    
    Label *readLabel = Label::createWithSystemFont("我已阅读并同意以上协议", "Arial", 42);
    readLabel->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - listSize.height / 2));
    this->addChild(readLabel);
    
    CheckBox *check = CheckBox::create("Load/check_bar.png", "Load/check.png");
    check->setScale(1.35);
    check->setPosition(Vec2(readLabel->getPositionX() + readLabel->getContentSize().width/2 + 65, readLabel->getPositionY()));
    check->setSelected(m_bHaveRead);
    check->addEventListener([=](Ref* ref,CheckBox::EventType type)
                            {
                                if (type == CheckBox::EventType::SELECTED)
                                {
                                    this->m_bHaveRead = true;
                                }
                                else if (type == CheckBox::EventType::UNSELECTED)
                                {
                                    this->m_bHaveRead = false;
                                }
                            });
    this->addChild(check);
    
    ui::Button *enterBtn = ui::Button::create("Room/dialog/v_alert_dialog_btn_green.png", "Room/dialog/v_alert_dialog_btn_green_press.png");
    enterBtn->setScale(1.5);
    enterBtn->setTitleText("开始游戏");
    enterBtn->setTitleFontName("Arial");
    enterBtn->setTitleFontSize(40);
    enterBtn->setPosition(Vec2(winSize.width / 2, readLabel->getPositionY() - 115));
    enterBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        if (this->m_bHaveRead == false) {
                                            PlatformHelper::showToast("需同意以上协议才能开始游戏");
                                            return;
                                        }
                                        
                                        UserDefault::getInstance()->setBoolForKey("have_read_agreement", true);
                                        UserDefault::getInstance()->flush();
                                        ZJHModel::getInstance()->gotoGameLoad(false);
                                    });
    this->addChild(enterBtn);
    
    return true;
}


