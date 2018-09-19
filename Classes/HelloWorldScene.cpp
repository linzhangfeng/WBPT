#include "HelloWorldScene.h"
USING_NS_CC;

void HelloWorld::getData(float dt)
{
    mHOffsetX = 0;
    mHOffsetY = 0;
    mVOffsetX = 0;
    mVOffsetY = 0;
    offsetX += 5;
    
    time += 0.01;
    if (time >= 1){
        time -= 1;
    }
    calculateHorizontalVertexPoints(offsetX);
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

Sprite* createItem(){
    //
    //    auto bill = BillBoard::create();
    
    auto sbg = Sprite::create("Test/pp_2h.png");
    //    sbg->setRotation3D(Vec3(-90,0,0));
    //    bill->addChild(sbg);
    return sbg;
}

Vec3 HelloWorld::originalVertex(const Vec2& position,NodeGrid* _gridNodeTarget){
    Grid3D* grid = (Grid3D*)_gridNodeTarget->getGrid();
    return grid->getOriginalVertex(position);
}

void HelloWorld::setVertex(const Vec2& position,const Vec3& vertex,NodeGrid* _gridNodeTarget){
    Grid3D* grid = (Grid3D*)_gridNodeTarget->getGrid();
    grid->setVertex(position, vertex);
}

void HelloWorld::calculateHorizontalVertexPoints(float offsetX)
{
//    mHOffsetY = -offsetX / 2;
    if (false){
        float time = offsetX / 1000;
        float tt = MAX(0, time - 0.25f);
        float deltaAy = (tt * tt * 500);
        float ay = -100 - deltaAy;
        
        float deltaTheta = sqrtf(time);
        float theta = deltaTheta > 0.5f ? (float)M_PI_2*deltaTheta : (float)M_PI_2*(1-deltaTheta);
        
        float rotateByYAxis = (2-time)* M_PI;
        
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);
        
        for (int i = 0; i <= m_sGridSize.width; ++i)
        {
            for (int j = 0; j <= m_sGridSize.height; ++j)
            {
                // Get original vertex
                Vec3 p = originalVertex(Vec2(i ,j),m_pForeSprite);
                
                p.x -= m_pForeSprite->getGridRect().origin.x;
                float R = sqrtf((p.x * p.x) + ((p.y - ay) * (p.y - ay)));
                float r = R * sinTheta;
                float alpha = asinf( p.x / R );
                float beta = alpha / sinTheta;
                float cosBeta = cosf( beta );
                
                // If beta > PI then we've wrapped around the cone
                // Reduce the radius to stop these points interfering with others
                if (beta <= M_PI)
                {
                    p.x = ( r * sinf(beta));
                }
                else
                {
                    // Force X = 0 to stop wrapped
                    // points
                    p.x = 0;
                }
                
                p.y = ( R + ay - ( r * (1 - cosBeta) * sinTheta));
                
                // We scale z here to avoid the animation being
                // too much bigger than the screen due to perspective transform
                
                p.z = (r * ( 1 - cosBeta ) * cosTheta);// "100" didn't work for
                p.x = p.z * sinf(rotateByYAxis) + p.x * cosf(rotateByYAxis);
                p.z = p.z * cosf(rotateByYAxis) - p.x * sinf(rotateByYAxis);
                p.z/=7;
                //    Stop z coord from dropping beneath underlying page in a transition
                // issue #751
                if( p.z < 0.5f )
                {
                    p.z = 0.5f;
                }
                
                // Set new coords
                p.x += m_pForeSprite->getGridRect().origin.x;
                setVertex(Vec2(i, j), p,m_pForeSprite);
            }
        }
        
        for (int i = 0; i <= m_sGridSize.width; ++i)
        {
            for (int j = 0; j <= m_sGridSize.height; ++j)
            {
                // Get original vertex
                Vec3 p = originalVertex(Vec2(i ,j),m_pBgSprite);
                
                p.x -= m_pBgSprite->getGridRect().origin.x;
                float R = sqrtf((p.x * p.x) + ((p.y - ay) * (p.y - ay)));
                float r = R * sinTheta;
                float alpha = asinf( p.x / R );
                float beta = alpha / sinTheta;
                float cosBeta = cosf( beta );
                
                // If beta > PI then we've wrapped around the cone
                // Reduce the radius to stop these points interfering with others
                if (beta <= M_PI)
                {
                    p.x = ( r * sinf(beta));
                }
                else
                {
                    // Force X = 0 to stop wrapped
                    // points
                    p.x = 0;
                }
                
                p.y = ( R + ay - ( r * (1 - cosBeta) * sinTheta));
                
                // We scale z here to avoid the animation being
                // too much bigger than the screen due to perspective transform
                
                p.z = (r * ( 1 - cosBeta ) * cosTheta);// "100" didn't work for
                p.x = p.z * sinf(rotateByYAxis) + p.x * cosf(rotateByYAxis);
                p.z = p.z * cosf(rotateByYAxis) - p.x * sinf(rotateByYAxis);
                p.z/=7;
                //    Stop z coord from dropping beneath underlying page in a transition
                // issue #751
                if( p.z < 0.5f )
                {
                    p.z = 0.5f;
                }
                
                // Set new coords
                p.x += m_pBgSprite->getGridRect().origin.x;
                setVertex(Vec2(i, j), p,m_pBgSprite);
            }
        }
    }
    
    
    if (true){
        float theta = (GLfloat)(M_PI / 6.0f);
        float R = 50;
        //        float b = (187 - offsetX * 1.4f) * sinf(theta);
        float b = (m_sGridSize.width - offsetX * 1.4f) * sinf(theta);
        for (int i = 0; i <= m_sGridSize.width; ++i)
        {
            for (int j = 0; j <= m_sGridSize.height; ++j)
            {
                // Get original vertex
                ccVertex3F p = originalVertex(Vec2(i ,j),m_pForeSprite);
                
                float x = (p.y + b) / tanf(theta);
                
                float pivotX = x + (p.x - x) * cosf(theta) * cosf(theta);
                float pivotY = pivotX * tanf(theta) - b;
                
                float l = (p.x - pivotX) / sinf(theta);
                float alpha = l / R;
                if (l >= 0) {
                    if (alpha > M_PI) {
                        p.x = (GLfloat)(mHOffsetX + pivotX - R * (alpha - M_PI) * sinf(theta));
                        p.y = (GLfloat)(mHOffsetY + pivotY + R * (alpha - M_PI) * cosf(theta));
                        p.z = (GLfloat)(2 * R / 9);
                    }
                    else if (alpha <= M_PI)
                    {
                        p.x = (GLfloat)(mHOffsetX + pivotX + R * sinf(alpha) * sinf(theta));
                        p.y = (GLfloat)(mHOffsetY + pivotY - R * sinf(alpha) * cosf(theta));
                        p.z = (GLfloat)((R - R * cosf(alpha))/9);
                    }
                }
                else
                {
                    p.x += mHOffsetX;
                    p.y += mHOffsetY;
                }
                
                // Set new coords
                setVertex(Vec2(i, j), p,m_pForeSprite);
            }
        }
        
        for (int i = 0; i <= m_sGridSize.width; ++i)
        {
            for (int j = 0; j <= m_sGridSize.height; ++j)
            {
                // Get original vertex
                ccVertex3F p = originalVertex(Vec2(i ,j),m_pBgSprite);
                float x = (p.y + b) / tanf(theta);
                
                float pivotX = x + (p.x - x) * cosf(theta) * cosf(theta);
                float pivotY = pivotX * tanf(theta) - b;
                
                float l = (p.x - pivotX) / sinf(theta);
                float alpha = l / R;
                if (l >= 0) {
                    if (alpha > M_PI) {
                        p.x = (GLfloat)(mHOffsetX + pivotX - R * (alpha - M_PI) * sinf(theta));
                        p.y = (GLfloat)(mHOffsetY + pivotY + R * (alpha - M_PI) * cosf(theta));
                        p.z = (GLfloat)(2 * R / 9);
                    }
                    else if (alpha <= M_PI)
                    {
                        p.x = (GLfloat)(mHOffsetX + pivotX + R * sinf(alpha) * sinf(theta));
                        p.y = (GLfloat)(mHOffsetY + pivotY - R * sinf(alpha) * cosf(theta));
                        p.z = (GLfloat)((R - R * cosf(alpha))/9);
                    }
                }
                else
                {
                    p.x += mHOffsetX;
                    p.y += mHOffsetY;
                }
                
                setVertex(Vec2(i, j), p,m_pBgSprite);
            }
        }
    }
    
    //        float R2 = 50;
    //    float offsetX2 = offsetX;//mTouchBegin.x - pTouch->getLocation().x;
    //        float pivotX2 = 187 - offsetX2;
    //
    //
    //        for (int i = 0; i <= m_sGridSize.width; ++i)
    //        {
    //            for (int j = 0; j <= m_sGridSize.height; ++j)
    //            {
    //                // Get original vertex
    //                ccVertex3F p = originalVertex(ccp(i ,j),m_pForeSprite);
    //                float l = p.x - pivotX2;
    //                float alpha = l / R2;
    //                if (l >= 0) {
    //                    if (alpha > M_PI) {
    //                        p.x = mVOffsetX + pivotX2 - R2 * (alpha - M_PI);
    //                        p.z = 2 * R2 / 9;
    //                        p.y = p.y + mVOffsetY;
    //                    }
    //                    else if (alpha <= M_PI)
    //                    {
    //                        p.x = mVOffsetX + pivotX2 + R2 * sinf(alpha);
    //                        p.z = (R2 - R2 * cosf(alpha))/9;
    //                        p.y = p.y + mVOffsetY;
    //                    }
    //                }
    //                else
    //                {
    //                    p.x = p.x + mVOffsetX;
    //                    p.y = p.y + mVOffsetY;
    //                }
    //
    //
    //                // Set new coords
    //                setVertex(ccp(i, j), p,m_pForeSprite);
    //
    //
    //            }
    //        }
    //
    //        for (int i = 0; i <= m_sGridSize.width; ++i)
    //        {
    //            for (int j = 0; j <= m_sGridSize.height; ++j)
    //            {
    //                // Get original vertex
    //                ccVertex3F p = originalVertex(ccp(i ,j),m_pBgSprite);
    //                float l = p.x - pivotX2;
    //                float alpha = l / R2;
    //                if (l >= 0) {
    //                    if (alpha > M_PI) {
    //                        p.x = mVOffsetX + pivotX2 - R2 * (alpha - M_PI);
    //                        p.z = 2 * R2 / 9;
    //                        p.y = p.y + mVOffsetY;
    //                    }
    //                    else if (alpha <= M_PI)
    //                    {
    //                        p.x = mVOffsetX + pivotX2 + R2 * sinf(alpha);
    //                        p.z = (R2 - R2 * cosf(alpha))/9;
    //                        p.y = p.y + mVOffsetY;
    //                    }
    //                }
    //                else
    //                {
    //                    p.x = p.x + mVOffsetX;
    //                    p.y = p.y + mVOffsetY;
    //                }
    //
    //                // Set new coords
    //                setVertex(ccp(i, j), p,m_pBgSprite);
    //            }
    //        }
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    mHOffsetX = 0;
    mHOffsetY = 0;
    offsetX = 0;
    time = 0;
    
    ZJHModel::getInstance()->hallToRoomV();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //    auto bg = Sprite::create("Test/bg.jpg");
    //    bg->setPosition(640,360);
    //    this->addChild(bg);
    //
    //    int dx = 121;
    //    int bx = 640 - 2 * dx;
    //    for (int i = 0; i < 5; i ++){
    //        auto sbg = createItem();
    //        sbg->setScale(0.8);
    //        sbg->setPosition(bx + dx * i,360);
    //        this->addChild(sbg);//放于底层
    ////        auto orbit1 =OrbitCamera::create(0,0,1, 0, -60, 0, 90);
    //
    //        auto orbit1 =OrbitCamera::create(0,1,0, 0, -45, 0, 90);
    //        sbg->runAction(orbit1);
    //    }
    
    //    _camera->setPosition3D(Vec3(540, 0, 0));
    //    _camera->lookAt(Vec3(540, 700, 0));
    //
    //    this->setCameraMask(2);
    
    
    auto fg = Sprite::create("page1_back.png");
    fg->setAnchorPoint(Vec2(0.5,0.5));
    fg->setScaleX(-1);
    fg->setPosition(0,0);
    m_sGridSize = Size(100,100);//fg->getContentSize();
//
    auto bg = Sprite::create("page1.png");
    bg->setAnchorPoint(Vec2(0.5,0.5));
    bg->setPosition(0,0);
    
    m_pForeSprite = NodeGrid::create();
    m_pForeSprite->setPosition(640,360);
    m_pForeSprite->setScale(1);
    this->addChild(m_pForeSprite, 11);
    
    m_pBgSprite = NodeGrid::create();
    m_pBgSprite->setScale(1);
    m_pBgSprite->setPosition(640,360);
    this->addChild(m_pBgSprite, 10);
    
    m_pForeSprite->addChild(fg, 0);
    m_pBgSprite->addChild(bg, 0);
    
    auto result = Grid3D::create(m_sGridSize);
    result->setActive(true);
    result->setNeedDepthTestForBlit(true);
    m_pForeSprite->setGrid(result);
    
    auto result2 = Grid3D::create(m_sGridSize);
    result2->setActive(true);
    result2->setNeedDepthTestForBlit(false);
    m_pBgSprite->setGrid(result2);
//    this->setAnchorPoint(Vec2(0.5,0.5));
//    this->ignoreAnchorPointForPosition(false);
//        m_pForeSprite->runAction(PageTurn3D::create(5, m_sGridSize));
    //    schedule(schedule_selector(HelloWorld::getData), 0.0);
//    CCDirector::sharedDirector()->setDepthTest(false);
    return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    Point touchPoint = touch->getLocation();
    Point touchBeginPoint = touch->getStartLocation();
    int dx = (touchBeginPoint.x - touchPoint.x) * 1;
    if (dx >= 0){
        calculateHorizontalVertexPoints(dx);
    }
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    
}