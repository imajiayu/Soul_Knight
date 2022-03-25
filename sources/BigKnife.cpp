#include "BigKnife.h"
BigKnife::BigKnife(BigKnifeController* hitController, int attackNum, String name) :Weapon(attackNum, name)
{
	/*tag*/
	this->setTag(DAO_HERO);

	this->hitController = hitController;

	this->addChild(hitController);

	this->weaponSprite->setPosition(20, -20);

	//¶ÁÈ¡Í¼Ïñ
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("BigKnife.plist", "BigKnife.png");

	int iFrameNum = 8;
	SpriteFrame* frame = NULL;

	for (int i = 0; i < iFrameNum; i++)
	{
		frame = frameCache->getSpriteFrameByName(StringUtils::format("BigKnife%d.png", i));
		weaponSpriteVec.pushBack(frame);
	}

	//ÉèÖÃ³õÊ¼Í¼Ïñ
	this->weaponSprite->setSpriteFrame(*(weaponSpriteVec.begin()));
}

BigKnife::BigKnife(int attackNum, String name) :Weapon(attackNum, name) {

	/*tag*/
	this->setTag(DAO_MONSTER);

	this->weaponSprite->setPosition(20, -20);

	//¶ÁÈ¡Í¼Ïñ
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("BigKnife.plist", "BigKnife.png");

	int iFrameNum = 8;
	SpriteFrame* frame = NULL;

	for (int i = 0; i < iFrameNum; i++)
	{
		frame = frameCache->getSpriteFrameByName(StringUtils::format("BigKnife%d.png", i));
		weaponSpriteVec.pushBack(frame);
	}

	//ÉèÖÃ³õÊ¼Í¼Ïñ
	this->weaponSprite->setSpriteFrame(*(weaponSpriteVec.begin()));
}
