#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "GlobalVariables.h"
#include "cocos2d.h"
#include "Box2D\Box2D.h"

extern GlobalVariables global;

class HelloWorld : public cocos2d::Layer
{
public:
    cocos2d::Sprite *background;
    cocos2d::Sprite *ball;
	cocos2d::Sprite *paddle;
	boolean leftIsPressed;
	boolean rightIsPressed;

    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(HelloWorld);

	void HelloWorld::update(float dt);
	
	void ricochet();

	void ricochet(cocos2d::Sprite *brick);

	void paddlePhysics();


private:
	cocos2d::PhysicsWorld *sceneWorld;

    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };
    
    bool onContactBegin( cocos2d::PhysicsContact &contact );
};

#endif // __HELLOWORLD_SCENE_H__
