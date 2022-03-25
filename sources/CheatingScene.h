#ifndef __CheatingScene_H__
#define __CheatingScene_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
using namespace cocostudio;
using namespace cocos2d;
USING_NS_CC_EXT;

class CheatingScene :public Scene {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CheatingScene);

	bool getInfiniteHp();
	bool getInfiniteMp();
	bool getInfiniteCoin();

	void hpEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type);
	void mpEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type);
	void coinEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type);
	void backEvent(Ref*);
private:
	bool isInfiniteHp;
	bool isInfiniteMp;
	bool isInfiniteCoin;

	int hpmax;
	int mpmax;
	int defensemax;
};
#endif // !__CheatingScene_H__
#pragma once