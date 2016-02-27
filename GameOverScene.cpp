#include "GameOverScene.h"
#include "HelloWorldScene.h"


USING_NS_CC;


Scene* GameOverScene::createScene()
{	
	auto scene = Scene::createWithPhysics();
	
	auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    return scene;
}


// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithSystemFont("U got rekt", "Arial", 72);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 5));
    this->addChild(label, 1);

	auto menuItem1 = MenuItemFont::create("Restart game", CC_CALLBACK_1(GameOverScene::Restart, this));

	menuItem1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 2 / 5));

	auto *menu = Menu::create(menuItem1, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);
    
    return true;
}


void GameOverScene::Restart(cocos2d::Ref* pSender)
{
	auto scene = HelloWorld::createScene();
    
    Director::getInstance()->pushScene(scene);
}