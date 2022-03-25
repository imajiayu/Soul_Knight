#include "BigKnifeController.h"
#include "Hero.h"
#include "Monster.h"
#include "SafeRoomScene.h"
#include "terrainBomb.h"
#include "terrainIce.h"
#include "terrainPoison.h"
#include "TollgateScene.h"
#include "BigKnife.h"

bool BigKnifeController::init()
{
	//初始化状态
	registeKeyBoardEvent();

	//把用来播放动画的对象加入其中
	for (int i = 0; i < MAX_IMAGE_SPRITE_NUM; i++)
	{
		Sprite* sprite = Sprite::create("KnightImage.png");
		actionSpriteVec[i] = sprite;
		actionSpriteRun[i] = false;
		GlobalParameter::mapNow->getParent()->addChild(sprite, 11);
		sprite->setVisible(false);
	}
	//大刀，冷却时间比较慢
	this->schedule(schedule_selector(BigKnifeController::hitUpdate), 1.0f);
	this->isHit = false;
	return true;
}

void BigKnifeController::registeKeyBoardEvent()
{
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		if (GlobalParameter::pressMode == 1) {
			switch (keyCode) {
			case EventKeyboard::KeyCode::KEY_J://射击键
				if (GlobalParameter::beFrozen) {
					this->isHit = false;
				}
				else {
					this->isHit = true;
				}

				break;
			}
		}
		else {
			switch (keyCode) {
			case EventKeyboard::KeyCode::KEY_Z://射击键
				if (GlobalParameter::beFrozen) {
					this->isHit = false;
				}
				else {
					this->isHit = true;
				}

				break;
			}
		}
		
	};
	keyBoardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		if (GlobalParameter::pressMode == 1) {
			switch (keyCode) {
			case EventKeyboard::KeyCode::KEY_J:
				this->isHit = false;
				break;
			}
		}
		else {
			switch (keyCode) {
			case EventKeyboard::KeyCode::KEY_Z:
				this->isHit = false;
				break;
			}
		}
		
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
}

void BigKnifeController::hitUpdate(float dt)
{
	if (isHit)
	{
		//从spriteVec中取出一个没有被使用的
		Sprite* actionSprite = nullptr;

		static int index;

		for (index = 0; index < MAX_IMAGE_SPRITE_NUM; index++)
		{
			//当前未播放
			if (actionSpriteRun[index] == false)
			{
				actionSprite = actionSpriteVec[index];
				actionSpriteRun[index] = true;
				break;
			}
		}

		Point hitPosition = GlobalParameter::hero->getPosition();
		//根据当前位置设定初始坐标
		if (GlobalParameter::rightSide)
		{
			if (GlobalParameter::upSide)
			{
				//右上
				hitPosition.x += 45;
				hitPosition.y += 15;
			}
			else if (GlobalParameter::downSide)
			{
				//右下
				hitPosition.x += 55;
				hitPosition.y -= 45;
			}
			else
			{
				//仅朝右
				hitPosition.x += 55;
				hitPosition.y -= 15;
			}
		}
		else if (GlobalParameter::leftSide)
		{
			if (GlobalParameter::upSide)
			{
				//左上
				hitPosition.x -= 45;
				hitPosition.y += 15;
			}
			else if (GlobalParameter::downSide)
			{
				//左下
				hitPosition.x -= 55;
				hitPosition.y -= 45;
			}
			else
			{
				//仅朝左
				hitPosition.x -= 55;
				hitPosition.y -= 15;
			}
		}
		else
		{
			if (GlobalParameter::upSide)
			{
				//仅朝上
				if (GlobalParameter::imageRightSide)
				{
					hitPosition.x += 20;
					hitPosition.y += 25;
				}
				else
				{
					hitPosition.x -= 20;
					hitPosition.y += 25;
				}
			}
			else
			{
				//仅朝下
				if (GlobalParameter::imageRightSide)
				{
					hitPosition.x += 20;
					hitPosition.y -= 55;
				}
				else
				{
					hitPosition.x -= 20;
					hitPosition.y -= 55;
				}
			}
		}

		actionSprite->setPosition(hitPosition);

		//尝试直接加载
		SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
		frameCache->addSpriteFramesWithFile("BigKnifeHit.plist", "BigKnifeHit.png");

		int iFrameNum = 16;
		SpriteFrame* frame = NULL;
		Vector<SpriteFrame*> frameVec;

		for (int i = 1; i <= iFrameNum; i++)
		{
			frame = frameCache->getSpriteFrameByName(StringUtils::format("BigKnife_%02d.png", i));
			frameVec.pushBack(frame);
		}

		Animation* animation = Animation::createWithSpriteFrames(frameVec);
		animation->setLoops(1);
		animation->setDelayPerUnit(0.05f);
		Animate* action = Animate::create(animation);


		//回调函数
		CallFunc* callFunc = CallFunc::create(
			[&]() {
				actionSpriteRun[index] = false;
				actionSpriteVec[index]->setVisible(false);
			}
		);

		//组合动作
		Action* actions = Sequence::create(action, callFunc, NULL);

		//动作精灵执行动作
		actionSprite->setVisible(true);
		actionSprite->runAction(actions);


		this->removeTile(hitPosition);

	
		if (TollgateScene::_monsterManager != nullptr) {
			Vector<Monster*>& monsterList = TollgateScene::_monsterManager->getMonsterList();

			for(int i=0;i<monsterList.size();++i)
				
					if (dynamic_cast<BigKnife*>(this->getParent())->getTag() == DAO_HERO
						&& GlobalParameter::hero->getBoundingBox().intersectsRect(monsterList.at(i)->getBoundingBox())) {
						monsterList.at(i)->getHurt(GlobalParameter::curAttack);
						log("Monster was hurt !\n");
					}
				
			
		}
	}
}

BigKnifeController* BigKnifeController::createForMonster() {
	BigKnifeController* bigKnifeController = new BigKnifeController();
	if (bigKnifeController && bigKnifeController->initForMonsters()) {
		bigKnifeController->retain();
	}
	else {
		CC_SAFE_DELETE(bigKnifeController);
	}

	return bigKnifeController;

}

bool BigKnifeController::initForMonsters() {
	//把用来播放动画的对象加入其中
	for (int i = 0; i < MAX_IMAGE_SPRITE_NUM; i++)
	{
		Sprite* sprite = Sprite::create("KnightImage.png");
		actionSpriteVec[i] = sprite;
		actionSpriteRun[i] = false;
		GlobalParameter::mapNow->getParent()->addChild(sprite, 11);
		sprite->setVisible(false);
	}

	this->schedule(schedule_selector(BigKnifeController::hitUpdateForMonster), 2.0f);
	this->isHit = false;
	return true;
}

void BigKnifeController::setIsHit(bool isHit) {
	this->isHit = isHit;
}


void BigKnifeController::hitUpdateForMonster(float dt)
{
	if (isHit)
	{
		//从spriteVec中取出一个没有被使用的
		Sprite* actionSprite = nullptr;

		static int index;

		for (index = 0; index < MAX_IMAGE_SPRITE_NUM; index++)
		{
			//当前未播放
			if (actionSpriteRun[index] == false)
			{
				actionSprite = actionSpriteVec[index];
				actionSpriteRun[index] = true;
				break;
			}
		}

		Point hitPosition = dynamic_cast<Monster*>(this->getParent()->getParent())->getPosition();
		//根据当前位置设定初始坐标
		if (dynamic_cast<Monster*>(this->getParent()->getParent())->getright())
		{
			if (dynamic_cast<Monster*>(this->getParent()->getParent())->getup())
			{
				//右上
				hitPosition.x += 45;
				hitPosition.y += 15;
			}
			else if (dynamic_cast<Monster*>(this->getParent()->getParent())->getdown())
			{
				//右下
				hitPosition.x += 55;
				hitPosition.y -= 45;
			}
			else
			{
				//仅朝右
				hitPosition.x += 55;

			}
		}
		else if (dynamic_cast<Monster*>(this->getParent()->getParent())->getleft())
		{
			if (dynamic_cast<Monster*>(this->getParent()->getParent())->getup())
			{
				//左上
				hitPosition.x -= 45;
				hitPosition.y += 15;
			}
			else if (dynamic_cast<Monster*>(this->getParent()->getParent())->getdown())
			{
				//左下
				hitPosition.x -= 55;
				hitPosition.y -= 45;
			}
			else
			{
				//仅朝左
				hitPosition.x -= 55;

			}
		}


		actionSprite->setPosition(hitPosition);



		//尝试直接加载
		SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
		frameCache->addSpriteFramesWithFile("BigKnifeHit.plist", "BigKnifeHit.png");

		int iFrameNum = 16;
		SpriteFrame* frame = NULL;
		Vector<SpriteFrame*> frameVec;

		for (int i = 1; i <= iFrameNum; i++)
		{
			frame = frameCache->getSpriteFrameByName(StringUtils::format("BigKnife_%02d.png", i));
			frameVec.pushBack(frame);
		}

		Animation* animation = Animation::createWithSpriteFrames(frameVec);
		animation->setLoops(1);
		animation->setDelayPerUnit(0.05f);
		Animate* action = Animate::create(animation);


		//回调函数
		CallFunc* callFunc = CallFunc::create(
			[&]() {
				actionSpriteRun[index] = false;
				actionSpriteVec[index]->setVisible(false);
			}
		);

		//组合动作
		Action* actions = Sequence::create(action, callFunc, NULL);

		//动作精灵执行动作
		actionSprite->setVisible(true);
		actionSprite->runAction(actions);

		if (dynamic_cast<Knife*>(this->getParent())->getTag() == DAO_MONSTER
			&& dynamic_cast<Monster*>(this->getParent()->getParent())->getBoundingBox().intersectsRect(GlobalParameter::hero->getBoundingBox())) {
			GlobalParameter::hero->getHurt(1);
			log("Hero was hurt !\n");

		}
	}
}
void BigKnifeController::removeTile(Point hitPosition) {
	//判断是否触及障碍物
	Size mapSize = GlobalParameter::mapNow->getMapSize();
	Size tileSize = GlobalParameter::mapNow->getTileSize();
	TMXLayer* canLayer = GlobalParameter::mapNow->getLayer("can");
	TMXLayer* metaLayer = GlobalParameter::mapNow->getLayer("meta");
	TMXLayer* collisionLayer = GlobalParameter::mapNow->getLayer("collision");
	TMXLayer* collision2Layer = GlobalParameter::mapNow->getLayer("collision2");
	//获得坐标在地图中的格子位置
	Point tiledPos(hitPosition.x / tileSize.width, (mapSize.height * tileSize.height - hitPosition.y) / tileSize.height);

	if (hitPosition.x < 0 || hitPosition.y < 0)
	{
		//迷之报错，在这里隐藏掉
		log("What's wrong with you?");
		return;
	}
	int tiledGid = metaLayer->getTileGIDAt(tiledPos);
	int tiledGidCan = canLayer->getTileGIDAt(tiledPos);
	if (tiledGidCan != 0) {

		Value propertiesCan = GlobalParameter::mapNow->getPropertiesForGID(tiledGidCan);
		if (propertiesCan.isNull()) {
			return;
		}
		ValueMap propMapCan = propertiesCan.asValueMap();
		if (propMapCan.find("Poison") != propMapCan.end()) {
			Value prop = propMapCan.at("Poison");

			if (prop.asString().compare("true") == 0) {

				this->createPoison(hitPosition);
				metaLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				canLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				if (collisionLayer->getTileGIDAt(tiledPos)) {
					collisionLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				else {
					collision2Layer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				log("ok, poison!");
				return;
			}
		}
		if (propMapCan.find("Hot") != propMapCan.end()) {
			Value prop = propMapCan.at("Hot");
			if (prop.asString().compare("true") == 0) {

				this->createBoom(hitPosition);

				metaLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				canLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				if (collisionLayer->getTileGIDAt(tiledPos)) {
					collisionLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				else {
					collision2Layer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				log("ok, boom!");
				return;;
			}

		}

		if (propMapCan.find("Cold") != propMapCan.end()) {
			Value prop = propMapCan.at("Cold");

			if (prop.asString().compare("true") == 0) {

				this->createIce(hitPosition);
				metaLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				canLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				if (collisionLayer->getTileGIDAt(tiledPos)) {
					collisionLayer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				else {
					collision2Layer->removeTileAt(Vec2(int(tiledPos.x), int(tiledPos.y)));
				}
				log("ok, ice!");
				return;
			}

		}
		return;
	}


	//图块ID不为空，表示是障碍物
	if (tiledGid != 0)
	{
		Value properties = GlobalParameter::mapNow->getPropertiesForGID(tiledGid);
		if (properties.isNull()) {
			return;
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
		}
		return;
	}
}
void BigKnifeController::createBoom(Point position) {
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("BreathImage.plist", "BreathImage.png");
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
void BigKnifeController::createPoison(Point position) {
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
void BigKnifeController::createIce(Point position) {
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