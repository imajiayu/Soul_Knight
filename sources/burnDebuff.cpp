#include "burnDebuff.h"
#include "GlobalParameter.h"

bool burnDebuff::init() {
	this->deBuffTime = 100;
	this->buff_per = 1;
	this->scheduleUpdate();
	return true;
}
void burnDebuff::Buffeffect(Hero* hero) {
	GlobalParameter::isBurn = true;
	hero->getHurt(buff_per);
}
void burnDebuff::update(float dt) {
	if (!deBuffTime) {
		GlobalParameter::isBurn = false;
		this->removeFromParentAndCleanup(true);
		return;
	}
	if (deBuffTime % 30 == 0) {
		Buffeffect(GlobalParameter::hero);
		log("burn buff !!");
	}
	deBuffTime--;
}