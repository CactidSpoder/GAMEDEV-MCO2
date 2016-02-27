#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h";

class HelloWorld : public cocos2d::Layer
{
public:
    cocos2d::Sprite *background;
    cocos2d::Sprite *ball;
	cocos2d::Sprite *paddle;
	boolean leftIsPressed;
	boolean rightIsPressed;
	int brickCount;


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