#ifndef __BoardSettingScene_H__
#define __BoardSettingScene_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
using namespace cocostudio;
using namespace cocos2d;
USING_NS_CC_EXT;

class BoardSettingScene :public Scene {
public:
	CREATE_FUNC(BoardSettingScene);
	static Scene* createScene();
	virtual bool init();
	
	bool getBoardSetting();
	void mode1Event(Ref* pSender, cocos2d::ui::CheckBox::EventType type);
	void mode2Event(Ref* pSender, cocos2d::ui::CheckBox::EventType type);
	void backEvent(Ref*, TouchEventType type);
private:
	bool boardSetting;
	CheckBox* mode1;
	CheckBox* mode2;
};
#endif // !__BoardSettingScene_H__

