#include "Bullet.h"
#include "SafeRoomScene.h"
#include "Monster.h"
#include "Items.h"
#include "TollgateScene.h"
#include "terrainBomb.h"
#include "terrainIce.h"
#include "terrainPoison.h"
#include "Boss.h"

//���캯��
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

	//���ó�ʼ״̬
	log("Bullet Created;");

	//��ͼƬ
	Sprite* newSprite = Sprite::create(StringUtils::format("bullet_%d.png", numBullet));
	this->bindSprite(newSprite);
	//�����˶�״̬
	this->setActive(false);

	return true;
}

//�����ӵ�
void Bullet::shoot(Point position)
{
	log("This bullet is shot!");
	//��ȡ��ʼ����λ��
	this->setPosition(position);

	//��תͼƬ�Ƕ�

	//�趨���յص�
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

	//�����ƶ�
	MoveTo* moveTo = MoveTo::create(sqrt(
		(finalX - position.x) * (finalX - position.x) +
		(finalY - position.y) * (finalY - position.y)) / speed,
		Point(finalX, finalY));
	this->runAction(moveTo);

	//����ƶ��¼�
	this->scheduleUpdate();
}
void Bullet::shootForMonster(Point position, Monster* monster) {
	log("This bullet is shot!");
	//��ȡ��ʼ����λ��
	this->setPosition(position);

	//�趨���յص�

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

	//�����ƶ�
	MoveTo* moveTo = MoveTo::create(sqrt(
		(finalX - position.x) * (finalX - position.x) +
		(finalY - position.y) * (finalY - position.y)) / speed,
		Point(finalX, finalY));
	this->runAction(moveTo);

	//����ƶ��¼�
	this->scheduleUpdate();
}


void Bullet::setActive(bool _isActive)
{
	this->isActive = _isActive;

	//���ÿɼ���
	this->setVisible(_isActive);

	this->actionSprite->setVisible(false);
}

//�ж��Ƿ񵽴�Ŀ��
bool Bullet::isArrive()
{
	//��õ�ǰ��������
	Point position = this->getPosition();

	//�ж��Ƿ񴥼��ϰ���
	Size mapSize = GlobalParameter::mapNow->getMapSize();
	Size tileSize = GlobalParameter::mapNow->getTileSize();
	// ��ȡ�����openGL����

	//��ȡ��ǰ���ڵ�ͼ���ϰ����
	TMXLayer* metaLayer = GlobalParameter::mapNow->getLayer("meta");
	TMXLayer* collisionLayer = GlobalParameter::mapNow->getLayer("collision");
	TMXLayer* collision2Layer = GlobalParameter::mapNow->getLayer("collision2");
    TMXLayer* canLayer = GlobalParameter::mapNow->getLayer("can");
	//��������ڵ�ͼ�еĸ���λ��
	Point tiledPos(position.x / tileSize.width, (mapSize.height * tileSize.height - position.y) / tileSize.height);

	if (position.x < 0 || position.y < 0)
	{
		//��֮�������������ص�
		log("What's wrong with you?");
		return true;
	}

	//��ȡ��ͼ���ӵ�Ψһ��ʾ
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


	//ͼ��ID��Ϊ�գ���ʾ���ϰ���
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

	/*�ж��Ƿ���������,���Ҹ�*/
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
	//��ͼƬ
	bulletSprite = sprite;
	this->addChild(bulletSprite);

	//�������Ŷ�����ͼƬ����
	this->actionSprite = Sprite::create("bullet_6.png");
	this->addChild(actionSprite);

	Size size = bulletSprite->getContentSize();
	this->setContentSize(size);
	//���ý�������
	this->setAnchorPoint(Point(0, 0));
}

void Bullet::update(float dt)
{
	//�ж��Ƿ񵽴�
	if (this->isArrive())
	{
		//ֹ֮ͣǰ�Ĳ���
		this->stopAllActions();
		this->unscheduleUpdate();

		//����һ���ص�����
		//����ֱ�Ӽ���
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

		//����һ���ص�����
		CallFunc* callFunc = CallFunc::create(
			[&]() {
				this->setActive(false);
			}
		);

		//��϶���
		Action* actions = Sequence::create(action, callFunc, NULL);
		//��������ִ�ж���
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

	//����һ���ص�����
	CallFunc* callFunc = CallFunc::create(
		[&]() {
		}
	);

	//��϶���
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

	//���ó�ʼ״̬
	log("Bullet Created;");

	//��ͼƬ
	Sprite* sprite = nullptr;
	if (numBullet == CIRCULE)
		sprite = Sprite::create("BossCircularBullet.png");
	else if (numBullet == SQUARE)
		sprite = Sprite::create("BossSquareBullet.png");
	this->bindSprite(sprite);

	//�����˶�״̬
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

	//����ƶ��¼�
	this->scheduleUpdate();
}
