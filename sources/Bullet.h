#ifndef _BULLET_H__
#define _BULLET_H__

#include "GlobalParameter.h"
#include "Boss.h"
class Monster;
#define CIRCULE 99
#define SQUARE 98
#define CIRCULENUM 16
#define SQUARENUM 12
#define PI 3.1415926535898
#define SPEED 50
#define R 10000
class Boss;

class Bullet :public Node
{
private:

	//子弹状态
	bool isActive;
	//图片对象
	Sprite* bulletSprite;
	//动画对象
	Sprite* actionSprite;
	//子弹速度
	int speed = 500;

	//最终位置
	int finalX;
	int finalY;

	//子弹编号
	int numBullet;
public:
	//构造函数
	Bullet();//

	Bullet(int numBullet);
	virtual bool init();
	//发射子弹
	void shoot(Point position);
	void shootForMonster(Point position,Monster* monster );
	//设置子弹状态
	void setActive(bool _isActive);
	//获得子弹状态
	bool getActive();
	//判定是否到达目标
	bool isArrive();
	//绑定图像
	void bindSprite(Sprite *sprite);
	//更新函数
	void update(float dt);
	//获得绑定的图像
	Sprite* getBindSprite();

	//地形碰撞出现;
	void createBoom(Point poisition);
	void createPoison(Point position);
	void createIce(Point position);

public:
	/*Boss*/
	Bullet(int tag, int);
	bool initForBoss();
	void shootBossBullet(const Point& pos, Boss* boss);

	
};
#endif // !_BULLET_H__
