#include"CheatingScene.h"
#include"GlobalParameter.h"
#include "Hero.h"

Scene* CheatingScene::createScene() {
	auto scene = Scene::create();
	auto layer =CheatingScene::create();
	scene->addChild(layer);
	return scene;
}

bool CheatingScene::init() {
	if (!Scene::create())
		return false;
	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("CheatingScene_1.ExportJson");
	this->addChild(UI);

	CheckBox* checkBox_hp = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(UI, "CheckBox_hp"));
	checkBox_hp->addEventListener(CC_CALLBACK_2(CheatingScene::hpEvent, this));

	CheckBox* checkBox_mp = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(UI, "CheckBox_mp"));
	checkBox_mp->addEventListener(CC_CALLBACK_2(CheatingScene::mpEvent, this));

	CheckBox* checkBox_coin = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(UI, "CheckBox_coin"));
	checkBox_coin->addEventListener(CC_CALLBACK_2(CheatingScene::coinEvent, this));

	Button* Btn_back = dynamic_cast<Button*>(Helper::seekWidgetByName(UI, "Button_back"));
	Btn_back->addClickEventListener(CC_CALLBACK_1(CheatingScene::backEvent, this));

	//用来提示玩家的一条消息
	Text* label = Text::create();
	label->setText("Attention:the bug only applies in \nthis turn of the game in case it may \nbreak the interst of the game!");
	label->setFontSize(20);
	this->addChild(label);
	label->setPosition(Point(200, 100));

	//赋初值
	isInfiniteHp = GlobalParameter::hpInite;
	checkBox_hp->setSelected(isInfiniteHp);
	isInfiniteMp = GlobalParameter::mpInite;
	checkBox_mp->setSelected(isInfiniteMp);
	isInfiniteCoin = GlobalParameter::moneyInite;
	checkBox_coin->setSelected(isInfiniteCoin);
	

	if (GlobalParameter::heroID == 1) {
		FILE* fp = fopen("KnightData.txt", "r");
		for (int i = 1; i < GlobalParameter::KnightLevel; i++)
		{
			fscanf(fp, "%d,%d,%d", &hpmax, &defensemax, &mpmax);
		}

		//需要此行数据
		fscanf(fp, "%d,%d,%d", &hpmax, &defensemax, &mpmax);
		fclose(fp);
	}
	else {
		FILE* fp = fopen("RabbiData.txt", "r");
		for (int i = 1; i < GlobalParameter::RabbiLevel; i++)
		{
			fscanf(fp, "%d,%d,%d", &hpmax, &defensemax, &mpmax);
		}

		//需要此行数据
		fscanf(fp, "%d,%d,%d", &hpmax, &defensemax, &mpmax);
		fclose(fp);
	}
	return true;

}

bool CheatingScene::getInfiniteHp() {
	return isInfiniteHp;
}

bool CheatingScene::getInfiniteMp() {
	return isInfiniteMp;
}

bool CheatingScene::getInfiniteCoin() {
	return isInfiniteCoin;
}

void CheatingScene::hpEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type) {
	switch(type) {
	case cocos2d::ui::CheckBox::EventType::SELECTED: {
		isInfiniteHp = true;
		GlobalParameter::hpInite = true;
		GlobalParameter::hero->setHpMax(9999);
		GlobalParameter::hero->setHp(9999);
		break;
	}
	case cocos2d::ui::CheckBox::EventType::UNSELECTED: {
		isInfiniteHp = false;
		GlobalParameter::hpInite = false;
		GlobalParameter::hero->setHpMax(this->hpmax);
		GlobalParameter::hero->setHp(this->hpmax);
		GlobalParameter::canShootMp = true;

		break;
	}
	}
}

void CheatingScene::mpEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type) {
	switch (type) {
	case cocos2d::ui::CheckBox::EventType::SELECTED: {
		isInfiniteMp = true;
		GlobalParameter::mpInite = true;
		GlobalParameter::hero->setMpMax(9999);
		GlobalParameter::hero->setMp(9999);
	}break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED: {
		isInfiniteMp = false;
		GlobalParameter::mpInite = false;
		GlobalParameter::hero->setMpMax(this->mpmax);
		GlobalParameter::hero->setMp(this->mpmax);
	}break;
	}
}

void CheatingScene::coinEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type) {
	switch (type) {
	case cocos2d::ui::CheckBox::EventType::SELECTED: {
		isInfiniteCoin = true;
		GlobalParameter::moneyInite = true;
		GlobalParameter::normalMoney = 9999;
	}break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED: {
		isInfiniteCoin = false;
		GlobalParameter::moneyInite = false;
	}break;
	}
}

void CheatingScene::backEvent(Ref*) {
	//返回暂停界面
	this->removeFromParentAndCleanup(true);
}