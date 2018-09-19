#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ZJH.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void getData(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    void calculateHorizontalVertexPoints(float offsetX);
    Vec3 originalVertex(const Vec2& position,NodeGrid* _gridNodeTarget);
    void setVertex(const Vec2& position, const Vec3& vertex,NodeGrid* _gridNodeTarget);
    NodeGrid* m_pBgSprite;
    Size m_sGridSize;
    NodeGrid* m_pForeSprite;
    float mHOffsetX;
    float mHOffsetY;
    float mVOffsetX;
    float mVOffsetY;
    float offsetX;
    float time;
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
};

#endif // __HELLOWORLD_SCENE_H__