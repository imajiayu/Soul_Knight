#ifndef _BOSS_H_
#define  _BOSS_H_

#include"cocos2d.h"
USING_NS_CC;

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

//#define CIRCULE 1
//#define SQUARE 2
//#define CIRCULENUM 16
//#define SQUARENUM 12
//#define PI 3.1415926535898
//#define SPEED 2
//#define R 10000

class ShootController;

class Boss :public Node {
public:
	/*��������*/
	static Animate* bossAdmissionAnimation();
	/*��ս����*/
	Animate* bossLeftAnimation();
	Animate* bossRightAnimation();
	/*�ӵ�����*/
	Animate* circuleShootAnimation();
	/*����*/
	Animate* bossDeadAnimation();
	/*�󶨾���*/
	void bindSprite();
	/*create func*/
	CREATE_FUNC(Boss);
	Boss();
	virtual bool init();
	virtual void update(float dt) override;
	/*�ƶ�������*/
	void move();
	bool isNotMove(const Point& lastPoint, const Point& curPoint);
	void changeDirection();
	void attackNear();
	void attackFar();
	/*����*/
	void setHp(int hpNow);
	void getHurt(int hurtValue);
	void bossDead();
private:
	bool isStuck(const Point& objPos);
	/*�ϰ���*/
	TMXLayer* _collisionLayer;
	TMXLayer* _collision1Layer;
	TMXLayer* _outsideLayer;
	TMXLayer* _canLayer;
	/*����*/
	Sprite* _sprite;
	Sprite* _spriteAdmission;
	/*����*/
	bool _isAction;

	CC_SYNTHESIZE(int, _hp, hp);
public:
	LoadingBar* _hpBar;
	TextAtlas* _hpNow;
	Widget* HPUI;

	/*����*/
	CC_SYNTHESIZE(bool, _up, up);
	CC_SYNTHESIZE(bool, _down, down);
	CC_SYNTHESIZE(bool, _left, left);
	CC_SYNTHESIZE(bool, _right, right);
	/*�ƶ��ٶ�*/
	CC_SYNTHESIZE(int, _moveSpeed, moveSpeed);
	/*�ϴ��Ƿ��ƶ�*/
	CC_SYNTHESIZE(Point, _lastPoint, lastPoint);
};

#endif