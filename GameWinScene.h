#ifndef __GAMEWIN_SCENE_H__
#define __GAMEWIN_SCENE_H__

#include "cocos2d.h";

class GameWinScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(GameWinScene);

	cocos2d::Sprite *wat;

	void NewGame(Ref* pSender);
};

#endif // __GAMEWIN_SCENE_H__