#include "Boss.h"
#include "GlobalParameter.h"
#include "Hero.h"
#include "Items.h"
#include "cmath"
#include "ShootController.h"

Boss::Boss() :
	_moveSpeed(2),
	_up(false),
	_down(false),
	_left(false),
	_right(false),
	_collisionLayer(nullptr),
	_collision1Layer(nullptr),
	_outsideLayer(nullptr),
	_canLayer(nullptr),
	_lastPoint(-1, -1),
	_hpBar(nullptr),
	_hp(100),
	_hpNow(nullptr),
	_isAction(false)
{

}

Animate* Boss::bossAdmissionAnimation() {
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->
		addSpriteFramesWithFile("bossAdmission.plist", "bossAdmission.png");

	constexpr int iFrameCount = 4;
	SpriteFrame* frame = nullptr;

	/*animation loading*/
	Vector< SpriteFrame*> frameVector;
	for (int i = 1; i <= iFrameCount; ++i) {
		frame = spriteFrameCache->getSpriteFrameByName(
			StringUtils::format("BossAdmission%d.png", i)
		);
		frame->setOriginalSize(Size(100, 100));
		frameVector.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(frameVector);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.5f);

	 auto actionAdmission = Animate::create(animation);
	 return actionAdmission;
	
}

Animate* Boss::bossLeftAnimation() {
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->
		addSpriteFramesWithFile("bossLeft.plist", "bossLeft.png");

	SpriteFrame* boosWalkframe =
		spriteFrameCache->getSpriteFrameByName("BoosWalk.png");
	SpriteFrame* boosLeftUpArmsframe = 
		spriteFrameCache->getSpriteFrameByName("BoosLeftUpArms.png");
	SpriteFrame* boosLeftMaxUpArmsframe = 
		spriteFrameCache->getSpriteFrameByName("BoosLeftMaxUpArms.png");
	SpriteFrame* boosLeftDownArmsframe = 
		spriteFrameCache->getSpriteFrameByName("BoosLeftDownArms.png");
	/*animation loading*/
	Vector< SpriteFrame*> frameVector;
	frameVector.pushBack(boosWalkframe);
	frameVector.pushBack(boosLeftUpArmsframe);
	frameVector.pushBack(boosLeftMaxUpArmsframe);
	frameVector.pushBack(boosLeftUpArmsframe);
	frameVector.pushBack(boosLeftDownArmsframe);
	frameVector.pushBack(boosLeftUpArmsframe);
	frameVector.pushBack(boosWalkframe);

	auto animation = Animation::createWithSpriteFrames(frameVector);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.3f);

	auto actionLeft = Animate::create(animation);
	return actionLeft;

	//_actionLeft = Sequence::create(actionLeft, callfunc_selector(Boss::callFunc), nullptr);
}

Animate* Boss::bossRightAnimation() {
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->
		addSpriteFramesWithFile("bossRight.plist", "bossRight.png");

	SpriteFrame* boosWalkframe =
		spriteFrameCache->getSpriteFrameByName("BoosWalk.png");
	SpriteFrame* boosRightUpArmsframe =
		spriteFrameCache->getSpriteFrameByName("BoosRigtUpArms.png");
	SpriteFrame* boosRightMaxUpArmsframe =
		spriteFrameCache->getSpriteFrameByName("BoosRigtMaxUpArms.png");
	SpriteFrame* boosRightDownArmsframe =
		spriteFrameCache->getSpriteFrameByName("BoosRigtDownArms.png");
	/*animation loading*/
	Vector< SpriteFrame*> frameVector;
	frameVector.pushBack(boosWalkframe);
	frameVector.pushBack(boosRightUpArmsframe);
	frameVector.pushBack(boosRightMaxUpArmsframe);
	frameVector.pushBack(boosRightUpArmsframe);
	frameVector.pushBack(boosRightDownArmsframe);
	frameVector.pushBack(boosRightUpArmsframe);
	frameVector.pushBack(boosWalkframe);

	auto animation = Animation::createWithSpriteFrames(frameVector);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.3f);

	auto action = Animate::create(animation);
	return action;
	//_actionRight = Animate::create(animation);
	//_actionRight->retain();
	//_actionRight = Sequence::create(actionRight, callfunc_selector(Boss::callFunc), nullptr);
}

Animate* Boss::circuleShootAnimation() {
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->
		addSpriteFramesWithFile("bossShoot.plist", "bossShoot.png");

	SpriteFrame* boosWalkframe =
		spriteFrameCache->getSpriteFrameByName("BoosWalk.png");
	SpriteFrame* boosSmashArmsframe =
		spriteFrameCache->getSpriteFrameByName("BoosSmashArms.png");
	SpriteFrame* boosShrinkframe =
		spriteFrameCache->getSpriteFrameByName("BoosShrink.png");
	SpriteFrame* boosFlutterframe =
		spriteFrameCache->getSpriteFrameByName("BoosFlutter.png");

	/*animation loading*/
	Vector< SpriteFrame*> frameVector;
	frameVector.pushBack(boosWalkframe);
	frameVector.pushBack(boosSmashArmsframe);
	frameVector.pushBack(boosShrinkframe);
	frameVector.pushBack(boosSmashArmsframe);
	frameVector.pushBack(boosFlutterframe);

	auto animation = Animation::createWithSpriteFrames(frameVector);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.3f);

	auto actionShoot = Animate::create(animation);
	return actionShoot;
}

Animate* Boss::bossDeadAnimation() {
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->
		addSpriteFramesWithFile("bossDead.plist", "bossDead.png");

	SpriteFrame* boosWalkframe =
		spriteFrameCache->getSpriteFrameByName("BoosWalk.png");
	SpriteFrame* boosSmashArmsframe =
		spriteFrameCache->getSpriteFrameByName("BoosSmashArms.png");
	SpriteFrame* boosShrinkframe =
		spriteFrameCache->getSpriteFrameByName("BoosShrink.png");
	SpriteFrame* boosBodyframe =
		spriteFrameCache->getSpriteFrameByName("BoosBody.png");

	/*animation loading*/
	Vector< SpriteFrame*> frameVector;
	frameVector.pushBack(boosWalkframe);
	frameVector.pushBack(boosSmashArmsframe);
	frameVector.pushBack(boosShrinkframe);
	frameVector.pushBack(boosBodyframe);

	auto animation = Animation::createWithSpriteFrames(frameVector);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.3f);

	auto actionDead = Animate::create(animation);
	return actionDead;
}

void Boss::bindSprite() {
	_sprite = Sprite::create("BoosWalk.png");

	this->addChild(_sprite);
	this->setContentSize(_sprite->getContentSize());
	/*锚点*/
	this->setAnchorPoint(Point(0, 0));
	/*出生点*/
	TMXObjectGroup* bossStart = GlobalParameter::mapNow->getObjectGroup("boss");
	ValueMap StartPoint=
		bossStart->getObject("boss");
	float X = StartPoint.at("x").asFloat();
	float Y = StartPoint.at("y").asFloat();
	this->setPosition(Point(X, Y));

	/*if (!_isAction) {
		_isAction = true;
		auto callFunc = CallFunc::create([&]() {
			this->_isAction = false;
			});
		_sprite->runAction(Sequence::create(this->bossAdmissionAnimation(), callFunc, nullptr));
	}*/
}

bool Boss::init() {
	if (!Node::init()) {
		return false;
	}
	/*加载动画*/
	this->bossAdmissionAnimation();
	this->bossLeftAnimation();
	this->bossRightAnimation();
	this->circuleShootAnimation();
	this->bossDeadAnimation();

	/*进场动画*/
	this->bindSprite();

	/*读取当前所在地图的障碍物层*/
	_collisionLayer = GlobalParameter::mapNow->getLayer("collision");
	_collision1Layer = GlobalParameter::mapNow->getLayer("collision2");
	_outsideLayer = GlobalParameter::mapNow->getLayer("outside");
	_canLayer = GlobalParameter::mapNow->getLayer("can");

	/*随机初始化方向*/
	srand(unsigned(time(nullptr)));
	float ran1 = rand() % 10 / 10.0;
	float ran2 = rand() % 10 / 10.0;

	if (ran1 < 0.5) {
		setup(true);
	}
	else {
		setdown(true);
	}

	if (ran2 < 0.5) {
		setright(true);
	}
	else {
		setleft(true);
	}

	this->scheduleUpdate();

	return true;
}

void Boss::update(float dt) {
	auto posThis = this->getPosition();
	
	/*计时缓冲*/
	static int iCount1 = 0;
	static int iCount2 = 0;

	setmoveSpeed(1);

	auto attackR = 400;
	auto posHero = GlobalParameter::hero->getPosition();
	Rect attackRect = Rect(
		posThis.x - attackR / 2, posThis.y - attackR / 2, attackR, attackR
	);

	if (attackRect.containsPoint(posHero)) {
		/*攻击*/
		this->attackNear();
		/*卡住优先*/
		if (isNotMove(getlastPoint(), posThis)) {
			if (iCount2 < 5)
				++iCount2;
			else {
				iCount2 = 0;
				log("Not move1 !\n");
				this->changeDirection();
			}
		}
		else {
			if (iCount1 < 15)
				++iCount1;
			else {
				iCount1 = 0;

				setup(false);
				setdown(false);
				setleft(false);
				setright(false);
				if (posHero.x >= posThis.x) {
					setright(true);
					if (posHero.y >= posThis.y)
						setup(true);
					else
						setdown(true);
				}
				else {
					setleft(true);
					if (posHero.y >= posThis.y)
						setup(true);
					else
						setdown(true);
				}
			}
		}
	}
	else {
		/*攻击*/
		this->attackFar();
		/*卡住优先*/
		if (isNotMove(getlastPoint(), posThis)) {
			if (iCount2 < 5)
				++iCount2;
			else {
				iCount2 = 0;
				log("Not move2 !\n");
				this->changeDirection();
			}
		}
		else {
			if (iCount1 < 15)
				++iCount1;
			else {
				iCount1 = 0;

				setup(false);
				setdown(false);
				setleft(false);
				setright(false);

				float ran1 = rand() % 10 / 10.0;
				float ran2 = rand() % 10 / 10.0;

				if (ran1 < 0.5) {
					setup(true);
				}
				else {
					setdown(true);
				}
				if (ran2 < 0.5) {
					setright(true);
				}
				else {
					setleft(true);
				}
			}
		}
	}
	this->move();
}

void Boss::move() {
	if (_sprite == nullptr) {
		return;
	}

	srand(unsigned(time(nullptr)));

	Point posThis = this->getPosition();

	/*移动*/
	int dx = 0, dy = 0;

	if (getup()) {
		dy = getmoveSpeed();
		if (getleft()) {
			dx = -getmoveSpeed();
		}
		else if (getright()) {
			dx = getmoveSpeed();
		}
	}
	else if (getdown()) {
		dy = -getmoveSpeed();
		if (getleft()) {
			dx = -getmoveSpeed();
		}
		else if (getright()) {
			dx = getmoveSpeed();
		}
	}
	else {
		if (getleft()) {
			dx = -getmoveSpeed();
		}
		else if (getright()) {
			dx = getmoveSpeed();
		}
	}

	/*移动*/
	Point objPos = Point(posThis.x + dx, posThis.y + dy);

	if (isStuck(objPos))
		return;

	this->setlastPoint(posThis);
	this->setPosition(objPos);
}

bool Boss::isStuck(const Point& objPos) {
	/*层大小*/
	auto layerSize = _collisionLayer->getLayerSize();

	//判断是否触及障碍物
	Size mapSize = GlobalParameter::mapNow->getMapSize();
	Size tileSize = GlobalParameter::mapNow->getTileSize();

	/*转换为地图上的坐标系*/
	Point tilesPos(objPos.x / tileSize.width, (mapSize.height * tileSize.height - objPos.y) / tileSize.height);

	if (tilesPos.x < 0 || tilesPos.y < 0 || tilesPos.x >= layerSize.width || tilesPos.y >= layerSize.height) {
		log("bad ways1\n");

		return true;
	}

	/*获取地图格子的唯一表示*/
	int tiledGidcollsion = _collisionLayer->getTileGIDAt(tilesPos);
	int tiledGidcollsion1 = _collision1Layer->getTileGIDAt(tilesPos);
	int tiledGidOutside = _outsideLayer->getTileGIDAt(tilesPos);
	int tiledGidCan = _canLayer->getTileGIDAt(tilesPos);


	//图块ID不为空，表示是障碍物
	if (tiledGidcollsion != 0 || tiledGidcollsion1 != 0 || tiledGidOutside != 0 || tiledGidCan != 0) {

		log("bad ways2\n");

		return true;
	}

	return false;
}

void Boss::attackNear() {
	auto posHero = GlobalParameter::hero->getPosition();
	auto posThis = this->getPosition();

	if (posThis.x > posHero.x) {
		if (!_isAction) {
			_isAction = true;
			log("short hit !");
			auto callFunc = CallFunc::create([&]() {
				this->_isAction = false;
				});
		
			_sprite->runAction(Sequence::create(this->bossLeftAnimation(), callFunc, nullptr));
		}
		if (this->getBoundingBox().intersectsRect(GlobalParameter::hero->getBoundingBox())) {
			GlobalParameter::hero->getHurt(3);
		}
	}
	else {
		if (!_isAction) {
			_isAction = true;
			log("short hit !");
			auto callFunc = CallFunc::create([&]() {
				this->_isAction = false;
				});

			_sprite->runAction(Sequence::create(this->bossRightAnimation(), callFunc,nullptr));
		}
		//this->runAction(this->bossRightAnimation());
		if (this->getBoundingBox().intersectsRect(GlobalParameter::hero->getBoundingBox())) {
			GlobalParameter::hero->getHurt(3);
		}
	}
}

void Boss::attackFar() {
	if (!_isAction) {
		log("far shoot !");
		_isAction = true;
		auto callFunc = CallFunc::create([&]() {
			this->_isAction = false;
			});
		_sprite->runAction(Sequence::create(this->circuleShootAnimation(), callFunc, nullptr));
	}

	this->_sprite->runAction(this->circuleShootAnimation());
	/*发射一圈的子弹*/
	static int cnt = 0;
	if (cnt > 1000) {
		cnt = 0;
		auto s1 = ShootController::createForBoss(CIRCULE, this);
		this->addChild(s1);
		/*缓冲*/
		for (int i = 0; i < 10000; ++i);

		/*auto s2 = ShootController::createForBoss(SQUARE, this);
		this->addChild(s2);*/
	}
	else {
		++cnt;
	}
}

bool Boss::isNotMove(const Point& lastPoint, const Point& curPoint) {
	static int icount = 0;
	++icount;

	int containR = 2;
	Rect rect = Rect(
		lastPoint.x - containR / 2, lastPoint.y - containR / 2,
		containR, containR
	);

	if (rect.containsPoint(curPoint))
		if (icount > 10) {
			icount = 0;
			return true;
		}
		else
			++icount;

	return false;
}

void Boss::changeDirection() {
	setmoveSpeed(3);

	if (getup()) {
		setup(false);
		setdown(true);
	}
	else if (getdown()) {
		setup(true);
		setdown(false);
	}
	else {
		if (getright()) {
			setright(false);
			setleft(true);
		}
		else if (getleft()) {
			setright(true);
			setleft(false);
		}
		else {

			float ran1 = rand() % 10 / 10.0;
			float ran2 = rand() % 10 / 10.0;

			if (ran1 < 0.5) {
				setup(true);
			}
			else {
				setdown(true);
			}

			if (ran2 < 0.5) {
				setright(true);
			}
			else {
				setleft(true);
			}
		}
	}
}

void Boss::bossDead() {
	if (!_isAction) {
		_isAction = true;
		auto callFunc = CallFunc::create([&]() {
			this->_isAction = false;
			});
		_sprite->runAction(Sequence::create(this->bossDeadAnimation(), callFunc, nullptr));
	}

	/*items*/
	auto items = Items::createItems(GUN_TAG, this->getPosition());
	this->getParent()->addChild(items);
	GlobalParameter::_bossNow = nullptr;
	/*dead*/
	this->removeFromParentAndCleanup(true);

	GlobalParameter::portal->setVisible(true);
}

void Boss::getHurt(int hurtValue) {
	auto hpNow = this->gethp();
	hpNow -= hurtValue;
	if (hpNow < 0) {
		hpNow = 0;
	}

	this->setHp(hpNow);
	if (hpNow == 0) {
		this->bossDead();
	}
}

void Boss::setHp(int hpNow) {
	this->sethp(hpNow);
	_hpNow->setStringValue(Value(hpNow).asString());
	_hpBar->setPercent(_hp * 100 / 100);
}

