#include "Bullet.h"
#include "SafeRoomScene.h"
#include "Monster.h"
#include "Items.h"
#include "TollgateScene.h"
#include "terrainBomb.h"
#include "terrainIce.h"
#include "terrainPoison.h"
#include "Boss.h"

//构造函数
Bullet::Bullet():numBullet(1){

	this->init();
}

Bullet::Bullet(int numBullet) :numBullet(numBullet)
{

	this->init();
}

bool Bullet::init()
{
	if (!Node::init())
		return false;

	//设置初始状态
	log("Bullet Created;");

	//绑定图片
	Sprite* newSprite = Sprite::create(StringUtils::format("bullet_%d.png", numBullet));
	this->bindSprite(newSprite);
	//设置运动状态
	this->setActive(false);

	return true;
}

//发射子弹
void Bullet::shoot(Point position)
{
	log("This bullet is shot!");
	//获取初始发射位置
	this->setPosition(position);

	//旋转图片角度

	//设定最终地点
	if (GlobalParameter::rightSide)
		finalX = position.x + 10000;
	else if (GlobalParameter::leftSide)
		finalX = position.x - 10000;
	else
		finalX = position.x;

	if (GlobalParameter::upSide)
		finalY = position.y + 10000;
	else if (GlobalParameter::downSide)
		finalY = position.y - 10000;
	else
		finalY = position.y;

	//持续移动
	MoveTo* moveTo = MoveTo::create(sqrt(
		(finalX - position.x) * (finalX - position.x) +
		(finalY - position.y) * (finalY - position.y)) / speed,
		Point(finalX, finalY));
	this->runAction(moveTo);

	//添加移动事件
	this->scheduleUpdate();
}
void Bullet::shootForMonster(Point position, Monster* monster) {
	log("This bullet is shot!");
	//获取初始发射位置
	this->setPosition(position);

	//设定最终地点

	if (monster->getright())
		finalX = position.x + 10000;
	else if (monster->getleft())
		finalX = position.x - 10000;
	else
		finalX = position.x;

	if (monster->getup())
		finalY = position.y + 10000;
	else if (monster->getdown())
		finalY = position.y - 10000;
	else
		finalY = position.y;

	//持续移动
	MoveTo* moveTo = MoveTo::create(sqrt(
		(finalX - position.x) * (finalX - position.x) +
		(finalY - position.y) * (finalY - position.y)) / speed,
		Point(finalX, finalY));
	this->runAction(moveTo);

	//添加移动事件
	this->scheduleUpdate();
}


void Bullet::setActive(bool _isActive)
{
	this->isActive = _isActive;

	//设置可见性
	this->setVisible(_isActive);

	this->actionSprite->setVisible(false);
}

//判断是否到达目标
bool Bullet::isArrive()
{
	//获得当前物体坐标
	Point position = this->getPosition();

	//判断是否触及障碍物
	Size mapSize = GlobalParameter::mapNow->getMapSize();
	Size tileSize = GlobalParameter::mapNow->getTileSize();
	// 获取点击的openGL坐标

	//读取当前所在地图的障碍物层
	TMXLayer* metaLayer = GlobalParameter::mapNow->getLayer("meta");
	TMXLayer* collisionLayer = GlobalParameter::mapNow->getLayer("collision");
	TMXLayer* collision2Layer = GlobalParameter::mapNow->getLayer("collision2");
    TMXLayer* canLayer = GlobalParameter::mapNow->getLayer("can");
	//获得坐标在地图中的格子位置
	Point tiledPos(position.x / tileSize.width, (mapSize.height * tileSize.height - position.y) / tileSize.height);

	if (position.x < 0 || position.y < 0)
	{
		//迷之报错，在这里隐藏掉
		log("What's wrong with you?");
		return true;
	}

	//获取地图格子的唯一表示
	int tiledGid = metaLayer->getTileGIDAt(tiledPos);

	int tiledGidCan = canLayer->getTileGIDAt(tiledPos);
	if (tiledGidCan!=0) {

		Value propertiesCan = GlobalParameter::mapNow->getPropertiesForGID(tiledGidCan);
		if (propertiesCan.isNull()) {
			return true;
		}
		ValueMap propMapCan = propertiesCan.asValueMap();
		if (propMapCan.find("Poison") != propMapCan.end()) {
			Value prop = propMapCan.at("Poison");

			if (prop.asString().compare("true") == 0) {

				this->createPoison(position);
				metaLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				canLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				if (collisionLayer->getTileGIDAt(tiledPos)) {
					collisionLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				else {
					collision2Layer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				log("ok, poison!");
				return true;
			}
		}
		if (propMapCan.find("Hot") != propMapCan.end()) {
			Value prop = propMapCan.at("Hot");
			if (prop.asString().compare("true") == 0) {

				this->createBoom(position);

				metaLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				canLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				if (collisionLayer->getTileGIDAt(tiledPos)) {
					collisionLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				else {
					collision2Layer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				log("ok, boom!");
				return true;
			}

		}

		if (propMapCan.find("Cold") != propMapCan.end()) {
			Value prop = propMapCan.at("Cold");

			if (prop.asString().compare("true") == 0) {

				this->createIce(position);
				metaLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				canLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				if (collisionLayer->getTileGIDAt(tiledPos)) {
					collisionLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				else {
					collision2Layer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				log("ok, ice!");
				return true;
			}

		}
		return true;
	}


	//图块ID不为空，表示是障碍物
	if (tiledGid != 0)
	{
		Value properties = GlobalParameter::mapNow->getPropertiesForGID(tiledGid);
		if (properties.isNull()) {
			return true;
		}
		ValueMap propMap = properties.asValueMap();

		if (propMap.find("Eat") != propMap.end()) {
			Value prop = propMap.at("Eat");

			if (prop.asString().compare("true") == 0) {
				metaLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				if (collisionLayer->getTileGIDAt(tiledPos)) {
					collisionLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				else {
					collision2Layer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				log("ok!");
			}
			else {
				return false;
			}
		}
		return true;
	}

	/*判断是否碰到敌人,别乱改*/
	if (TollgateScene::_monsterManager != nullptr) {
		Vector<Monster*>& monsterList = TollgateScene::_monsterManager->getMonsterList();
		for (int i = 0; i < monsterList.size(); ++i)

			if (HERO_BULLET == this->getTag() && this->getBoundingBox().intersectsRect(monsterList.at(i)->getBoundingBox())) {
				log("monseter was  hurt !\n");
				monsterList.at(i)->getHurt(GlobalParameter::curAttack);
				return true;
			}
	}

	if (HERO_BULLET == this->getTag() && GlobalParameter::_bossNow && this->getBoundingBox().intersectsRect(GlobalParameter::_bossNow->getBoundingBox())) {
		log("boss was hurt !");
		GlobalParameter::_bossNow->getHurt(GlobalParameter::curAttack);
		return true;
	}

	if (MONSTER_BULLET == this->getTag() && this->getBoundingBox().intersectsRect(GlobalParameter::hero->getBoundingBox())) {
		log("hero was hurt !\n");
		GlobalParameter::hero->getHurt(1);
		return true;
	}

	return false;
}

void Bullet::bindSprite(Sprite* sprite) {
	//绑定图片
	bulletSprite = sprite;
	this->addChild(bulletSprite);

	//用来播放动画的图片精灵
	this->actionSprite = Sprite::create("bullet_6.png");
	this->addChild(actionSprite);

	Size size = bulletSprite->getContentSize();
	this->setContentSize(size);
	//设置焦点坐标
	this->setAnchorPoint(Point(0, 0));
}

void Bullet::update(float dt)
{
	//判断是否到达
	if (this->isArrive())
	{
		//停止之前的操作
		this->stopAllActions();
		this->unscheduleUpdate();

		//创建一个回调函数
		//尝试直接加载
		SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
		frameCache->addSpriteFramesWithFile("Bullet1.plist", "Bullet1.png");

		int iFrameNum = 9;
		SpriteFrame* frame = NULL;
		Vector<SpriteFrame*> frameVec;

		for (int i = 1; i <= iFrameNum; i++)
		{
			frame = frameCache->getSpriteFrameByName(StringUtils::format("Bullet1 (%d).png", i));
			frameVec.pushBack(frame);
		}

		Animation* animation = Animation::createWithSpriteFrames(frameVec);
		animation->setLoops(1);
		animation->setDelayPerUnit(0.1f);

		Animate* action = Animate::create(animation);

		//创建一个回调函数
		CallFunc* callFunc = CallFunc::create(
			[&]() {
				this->setActive(false);
			}
		);

		//组合动作
		Action* actions = Sequence::create(action, callFunc, NULL);
		//动作精灵执行动作
		this->actionSprite->setVisible(true);
		this->actionSprite->runAction(actions);

	}
}

bool Bullet::getActive()
{
	return isActive;
}

Sprite* Bullet::getBindSprite()
{
	return this->bulletSprite;
}

void Bullet::createBoom(Point position) {
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("BreathImage.plist","BreathImage.png");
	int iFrameNum = 15;
	SpriteFrame* frame = NULL;
	Vector<SpriteFrame*> frameVec;

	for (int i = 1; i <= iFrameNum; i++)
	{
		std::string se = StringUtils::format("Breath_%02d.png", i);
		frame = frameCache->getSpriteFrameByName(StringUtils::format("Breath_%02d.png", i));
		frameVec.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameVec);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.1f);

	Animate* action = Animate::create(animation);

	//创建一个回调函数
	CallFunc* callFunc = CallFunc::create(
		[&]() {
		}
	);

	//组合动作
	Action* actions = Sequence::create(action, callFunc, NULL);

	auto boomLayer = terrainBomb::create();
	Sprite* sprite = Sprite::create();
//	sprite->setAnchorPoint(Point(0, 0));
	boomLayer->bindSprite(sprite);
	boomLayer->setCenterPoint(Point(position.x, position.y));
	boomLayer->setPosition(Point(position.x, position.y));
	sprite->runAction(actions);
//	sprite->setVisible(false);
	GlobalParameter::curMapLayer->addChild(boomLayer, 3);
}
void Bullet::createPoison(Point position) {
	auto poisonLayer = terrainPoison::create();
	Sprite* sprite = Sprite::create("poisonFog.png");
//	sprite->setAnchorPoint(Point(0, 0));
	poisonLayer->bindSprite(sprite);
	poisonLayer->setCenterPoint(Point(position.x, position.y));
	poisonLayer->setPosition(Point(position.x, position.y));

	FadeIn* fadeIn = FadeIn::create(1.0f);
	sprite->runAction(fadeIn);
	GlobalParameter::curMapLayer->addChild(poisonLayer, 3);
}
void Bullet::createIce(Point position) {
	auto iceLayer = terrainIce::create();
	Sprite* sprite = Sprite::create("iceFog.png");
//	sprite->setAnchorPoint(Point(0, 0));
	iceLayer->bindSprite(sprite);
	iceLayer->setCenterPoint(Point(position.x, position.y));
	iceLayer->setPosition(Point(position.x, position.y));

	FadeIn* fadeIn = FadeIn::create(1.0f);
	sprite->runAction(fadeIn);
	GlobalParameter::curMapLayer->addChild(iceLayer, 3);
}

//Boss
Bullet::Bullet(int tag, int) {
	this->numBullet = tag;
	this->setTag(MONSTER_BULLET);
	this->initForBoss();

}

bool Bullet::initForBoss() {
	if (!Node::init())
		return false;

	//设置初始状态
	log("Bullet Created;");

	//绑定图片
	Sprite* sprite = nullptr;
	if (numBullet == CIRCULE)
		sprite = Sprite::create("BossCircularBullet.png");
	else if (numBullet == SQUARE)
		sprite = Sprite::create("BossSquareBullet.png");
	this->bindSprite(sprite);

	//设置运动状态
	this->setActive(true);


	return true;
}

void Bullet::shootBossBullet(const Point& objpos, Boss* boss) {
	this->setPosition(boss->getPosition());
	log("boss bullet shoot !");
	MoveTo* moveTo = MoveTo::create(
		R / SPEED,
		objpos);

	//bulletSprite
	this->runAction(moveTo);

	//添加移动事件
	this->scheduleUpdate();
}
