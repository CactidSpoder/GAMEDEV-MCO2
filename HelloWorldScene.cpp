#include "GameOverScene.h"
#include "GameWinScene.h"
#include "HelloWorldScene.h"
#include "StartScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{	
	auto scene = Scene::createWithPhysics();
	
	auto layer = HelloWorld::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    return scene;
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
	
	brickCount = 0;

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	leftIsPressed = false;
	rightIsPressed = false;
	
	auto label = Label::createWithSystemFont("Left Arrow: Move Left\nRight Arrow: Move Right\nP: Pause\nR: Restart", "Arial", 12);
	label->setAnchorPoint(Vec2(0, 0));
    this->addChild(label, 1);



    
	background = Sprite::create("spacey.png");
	background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	
	this->addChild(background);


	ball = Sprite::create("spherical.png");
	ball->setTag(2);
	ball->setPosition(315, 95);
	ball->setScale(0.8);
	this->addChild(ball);


	auto ballBody = PhysicsBody::createCircle(16, PhysicsMaterial( 0, 1, 1 ));
    ballBody->setCollisionBitmask(2);
    ballBody->setContactTestBitmask(true);
	ballBody->setGravityEnable(false);
	ballBody->setVelocity(Vec2(140, 140));

    ball->setPhysicsBody(ballBody);

	paddle = Sprite::create("paddle.png");
	paddle->setTag(1);
	paddle->setPosition(315, 75);
	paddle->setScale(0.4);
	this->addChild(paddle);

	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize( ), PhysicsMaterial( 1000, 0, 0 ) );
    paddleBody->setCollisionBitmask(1);
    paddleBody->setContactTestBitmask(true);
	paddleBody->setGravityEnable(false);

    paddle->setPhysicsBody(paddleBody);
	
	for(int y = 0; y < 12; y++)
	{
		if(y != 3 && y != 4 && y != 8 && y != 9)
			for(int x = 0; x < 14; x++)
			{
				auto brick = Sprite::create();
				if(y < 3)
					brick->setTexture("normBrick.png");
				else if(y > 4 && y < 8)
					brick->setTexture("yellowBrick.png");
				else if(y > 9)
					brick->setTexture("redBrick.png");

				brick->setPosition(45 * x, 606 - 21 * y);
				brick->setAnchorPoint(Vec2(0.0, 0.0));
				brick->setScale(0.6);
				brick->setTag(3);
				this->addChild(brick);

				auto brickBody = PhysicsBody::createBox(brick->getContentSize(), PhysicsMaterial(10000, 0, 0));
				brickBody->setCollisionBitmask(3);
				brickBody->setContactTestBitmask(true);
				brickBody->setGravityEnable(false);
				brick->setPhysicsBody(brickBody);

				brickCount++;
			}
	}

	auto keyPressListener = EventListenerKeyboard::create();

    keyPressListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
        switch(keyCode)
		{
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				leftIsPressed = true;
                break;

            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				rightIsPressed = true;
                break;

			case EventKeyboard::KeyCode::KEY_P:
				if(Director::getInstance()->isPaused())
					Director::getInstance()->resume();
				else
					Director::getInstance()->pause();

                break;

			case EventKeyboard::KeyCode::KEY_R:
				auto scene = HelloWorld::createScene();
    
				Director::getInstance()->pushScene(scene);

                break;
        }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyPressListener, paddle);


	auto keyReleaseListener = EventListenerKeyboard::create();

    keyReleaseListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		switch(keyCode)
		{
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				leftIsPressed = false;
                break;

            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				rightIsPressed = false;
                break;
        }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyReleaseListener, paddle);

	
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->schedule(schedule_selector(HelloWorld::update));

	this->scheduleUpdate();
    
    return true;
}


bool HelloWorld::onContactBegin(PhysicsContact &contact)
{
    auto a = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto b = (Sprite*)contact.getShapeB()->getBody()->getNode();

	
	if((a->getTag() == 2 && b->getTag() == 3) || (a->getTag() == 3 && b->getTag() == 2))
	{
		if(a->getTag() == 3)
		{
			ricochet(a);
			a->removeFromParent();
		}
		else
		{
			ricochet(b);
			b->removeFromParent();
		}

		brickCount--;
	}
	else if((a->getTag() == 1 && b->getTag() == 2) || (a->getTag() == 2 && b->getTag() == 1))
	{
		paddlePhysics();
	}
	
    return true;
}

void HelloWorld::update(float dt)
{
	if(brickCount == 0)
	{
		auto scene = GameWinScene::createScene();
    
		Director::getInstance()->pushScene(scene);
	}





	if(leftIsPressed && !rightIsPressed && paddle->getPositionX() > 68)
	{
		paddle->getPhysicsBody()->setVelocity(Vec2(-160, 0));
	}
	else if(!leftIsPressed && rightIsPressed && paddle->getPositionX() < 552)
	{
		paddle->getPhysicsBody()->setVelocity(Vec2(160, 0));
	}
	else
	{
		paddle->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}
	

	if(abs(ball->getPhysicsBody()->getVelocity().x) < 10)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x * 9, ball->getPhysicsBody()->getVelocity().y));
	}
	else if(ball->getPhysicsBody()->getVelocity().x == 0)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(70, ball->getPhysicsBody()->getVelocity().y));
	}

	if(abs(ball->getPhysicsBody()->getVelocity().y) < 10)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x, ball->getPhysicsBody()->getVelocity().y * 9));
	}
	else if(ball->getPhysicsBody()->getVelocity().y == 0)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x, 70));
	}



	ricochet();

}

void HelloWorld::ricochet()
{
	if(ball->getPositionX() <= 14)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(140, ball->getPhysicsBody()->getVelocity().y));
	}
	
	if(ball->getPositionX() >= 620)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(-140, ball->getPhysicsBody()->getVelocity().y));
	}

	if(ball->getPositionY() <= 14)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x, 140));

		auto scene = GameOverScene::createScene();
    
		Director::getInstance()->pushScene(scene);
	}
	
	if(ball->getPositionY() >= 620)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x, -140));
	}
}

void HelloWorld::ricochet(Sprite *brick)
{
	int halfWidth = brick->getBoundingBox().size.width / 2;
	int halfHeight = brick->getBoundingBox().size.height / 2;

	if(ball->getPositionX() <= brick->getPositionX() && brick->getPositionX() - halfWidth <= ball->getPositionX())
	{

		ball->getPhysicsBody()->setVelocity(Vec2(-140, ball->getPhysicsBody()->getVelocity().y));

	}

	if(ball->getPositionX() >= brick->getPositionY() && brick->getPositionX() + halfWidth >= ball->getPositionX())
	{
		ball->getPhysicsBody()->setVelocity(Vec2(140, ball->getPhysicsBody()->getVelocity().y));
	}

	if(ball->getPositionY() <= brick->getPositionY() && brick->getPositionY() - halfHeight <= ball->getPositionX())
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x, -140));
	}
	
	if(ball->getPositionY() >= brick->getPositionY() && brick->getPositionX() + halfHeight >= ball->getPositionX())
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x, 140));
	}
}

void HelloWorld::paddlePhysics()
{
	if(ball->getPositionX() <= paddle->getPositionX() - 1)
	{
		if(ball->getPositionX() < paddle->getBoundingBox().size.width / 2);

		ball->getPhysicsBody()->setVelocity(Vec2(-140, ball->getPhysicsBody()->getVelocity().y));
	}
	
	if(ball->getPositionX() >= paddle->getPositionX() + 1)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(140, ball->getPhysicsBody()->getVelocity().y));
	}
	
	if(ball->getPositionY() >= paddle->getPositionY() + 1)
	{
		ball->getPhysicsBody()->setVelocity(Vec2(ball->getPhysicsBody()->getVelocity().x, 140));
	}
}