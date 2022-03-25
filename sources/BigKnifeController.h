#ifndef _BigKnife_CONTROLLER_H__
#define _BigKnife_CONTROLLER_H__
#include "GameController.h"
#include "GlobalParameter.h"
#define MAX_IMAGE_SPRITE_NUM 10

class BigKnifeController :public GameController
{
public:
	CREATE_FUNC(BigKnifeController);
	virtual bool init();
	void hitUpdate(float dt);
	void hitUpdateForMonster(float dt);

	bool isHit;
	//用来播放动画的image对象
	Sprite* actionSpriteVec[MAX_IMAGE_SPRITE_NUM];
	//存储image对象是否正在被使用
	bool actionSpriteRun[MAX_IMAGE_SPRITE_NUM];

	static BigKnifeController* createForMonster();
	bool initForMonsters();
	void setIsHit(bool isShot);
	
private:
	void registeKeyBoardEvent();
	void removeTile(Point hitPosition);
	void createPoison(Point position);
	void createBoom(Point position);
	void createIce(Point position);

};


#endif // !_BigKnife_CONTROLLER_H__
