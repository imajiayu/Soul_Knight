#include "buff.h"
#include "GlobalParameter.h"
#include "hero.h"

bool buff::init() {
	this->flag = 0;
	this->picture = "";
	return true;
}
void buff::setflag(int _flag) {
	this->flag = _flag;
}
void buff::buffEffect(Hero* hero) {
	switch (this->flag) {
	case ADD_HP_MAX:
		hero->sethpMax(hero->gethpMax() + 2);
		int curHP;
		curHP = hero->gethp();
		curHP += 2;
		if (curHP >= hero->gethpMax()) {
			curHP = hero->gethpMax();
		}
		hero->sethp(curHP);
		hero->setHp(hero->gethp());
		hero->setHpMax(hero->gethpMax());
		break;
	case ADD_DEFENSE_MAX:
		hero->setdefenseMax(hero->getdefenseMax() + 2);
		int curDefense;
		curDefense = hero->getdefense();
		curDefense += 2;
		if (curDefense >= hero->getdefenseMax()) {
			curDefense = hero->getdefenseMax();
		}
		hero->setdefense(curDefense);
		hero->setDefense(hero->getdefense());
		hero->setDefenseMax(hero->getdefenseMax());
		break;
	case STRENGTHEN_WEAPONS:
		GlobalParameter::strengthen_weapon = true;
		break;
	case RESTORE_HP:
		int curHP_2;
		curHP_2 = hero->gethp();
		curHP_2 += 3;
		if (curHP_2 >= hero->gethpMax()) {
			curHP_2 = hero->gethpMax();
		}
		hero->sethp(curHP_2);
		hero->setHp(hero->gethp());
		break;
	case SUCK_HP:
		GlobalParameter::suckHP = true;
		break;
	case SUCK_MP:
		GlobalParameter::suckMP = true;
		break;
	}
}
void buff::bindPicture() {
	switch (flag) {
	case ADD_HP_MAX:
		this->picture = "add_hp_max.png";
		break;
	case ADD_DEFENSE_MAX:
		this->picture = "add_defense_max.png";
		break;
	case STRENGTHEN_WEAPONS:
		this->picture = "strengthen_weapons.png";
		break;
	case RESTORE_HP:
		this->picture = "restore_hp.png";
		break;
	case SUCK_HP:
		this->picture = "suck_hp.png";
		break;
	case SUCK_MP:
		this->picture = "suck_mp.png";
		break;
	}

}
std::string buff::getPicture() {
	return this->picture;
}