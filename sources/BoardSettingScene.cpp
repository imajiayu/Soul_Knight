#include "BoardSettingScene.h"
#include "PauseLayer.h"
#include "GlobalParameter.h"
#include "SceneManager.h"

Scene* BoardSettingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BoardSettingScene::create();
    scene->addChild(layer);
    return scene;
}

bool BoardSettingScene::init() {
    if (!Scene::create())
        return false;
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* BGimage = Sprite::create("boardSettingBGimage.png");
    BGimage->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(BGimage);

    boardSetting = true;

    mode1 = CheckBox::create("Btn_mode1.png",
    "Btn_mode1_PressDown.png");
    mode1->setPosition(Point(visibleSize.width/2-250,visibleSize.height/2+50));
    this->addChild(mode1);
    mode1->addEventListener(CC_CALLBACK_2(BoardSettingScene::mode1Event, this));
    
    mode2 = CheckBox::create("Btn_mode2.png",
        "Btn_mode2_PressDown.png");
    mode2->setPosition(Point(visibleSize.width / 2+250, visibleSize.height / 2+50));
    this->addChild(mode2);
    mode2->addEventListener(CC_CALLBACK_2(BoardSettingScene::mode2Event, this));

    
	//根据以前的键位确定当前键位
	if (GlobalParameter::pressMode == 1)
	{
		mode1->setSelected(true);
		mode2->setSelected(false);
	}
	else
	{
		mode1->setSelected(false);
		mode2->setSelected(true);
	}

    auto Btn_backToStartScene = Button::create("Btn_exit.jpg","Btn_exit_PressDown.jpg");
    Btn_backToStartScene->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 250));
    this->addChild(Btn_backToStartScene);
    Btn_backToStartScene->addTouchEventListener(this, toucheventselector(BoardSettingScene::backEvent));
}

bool BoardSettingScene::getBoardSetting() {
    return boardSetting;
}

void BoardSettingScene::mode1Event(Ref* pSender, cocos2d::ui::CheckBox::EventType type) {
    switch (type) {
    case cocos2d::ui::CheckBox::EventType::SELECTED: {
		mode1->setSelected(true);
        mode2->setSelected(false);
		FILE* fp = fopen("GameData.txt", "r");
		fscanf(fp, "%d", &GlobalParameter::KnightLevel);
		fscanf(fp, "%d", &GlobalParameter::RabbiLevel);
		fscanf(fp, "%d", &GlobalParameter::specialMoney);
		fscanf(fp, "%d", &GlobalParameter::pressMode);
		fclose(fp);
		GlobalParameter::pressMode = 1;
		//写文件
		fp = fopen("GameData.txt", "w");
		fprintf(fp, "%d\n", GlobalParameter::KnightLevel);
		fprintf(fp, "%d\n", GlobalParameter::RabbiLevel);
		fprintf(fp, "%d\n", GlobalParameter::specialMoney);
		fprintf(fp, "%d\n", GlobalParameter::pressMode);
		fclose(fp);
		break;
    }
	case cocos2d::ui::CheckBox::EventType::UNSELECTED: {
		return;
	}
    }
}

void BoardSettingScene::mode2Event(Ref* pSender, cocos2d::ui::CheckBox::EventType type) {
    switch (type) {
    case cocos2d::ui::CheckBox::EventType::SELECTED: {
		mode2->setSelected(true);
        mode1->setSelected(false);
		FILE* fp = fopen("GameData.txt", "r");
		fscanf(fp, "%d", &GlobalParameter::KnightLevel);
		fscanf(fp, "%d", &GlobalParameter::RabbiLevel);
		fscanf(fp, "%d", &GlobalParameter::specialMoney);
		fscanf(fp, "%d", &GlobalParameter::pressMode);
		fclose(fp);
		GlobalParameter::pressMode = 2;
		//写文件
		fp = fopen("GameData.txt", "w");
		fprintf(fp, "%d\n", GlobalParameter::KnightLevel);
		fprintf(fp, "%d\n", GlobalParameter::RabbiLevel);
		fprintf(fp, "%d\n", GlobalParameter::specialMoney);
		fprintf(fp, "%d\n", GlobalParameter::pressMode);
		fclose(fp);
		break;
    }
	case cocos2d::ui::CheckBox::EventType::UNSELECTED: {
		return;
	}
    }
}

void BoardSettingScene::backEvent(Ref*, TouchEventType type) {
    switch (type){
    case TouchEventType::TOUCH_EVENT_ENDED: {
		if (!mode1->isSelected() && !mode2->isSelected())
			return;
		else
		{
			if (GlobalParameter::start == false) {
				SceneManager::getInstance()->changeScene(SceneManager::HeroChooseLayer);
				GlobalParameter::start = true;
			}
			//返回之前的场景
			this->removeFromParentAndCleanup(true);
		}
		break;
    }
    }
}