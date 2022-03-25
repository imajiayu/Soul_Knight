#ifndef __BUFF_H__
#define __BUFF_H__

#include "cocos2d.h"
#include "Hero.h"


USING_NS_CC;
#define ADD_HP_MAX 0
#define ADD_DEFENSE_MAX 1
#define STRENGTHEN_WEAPONS 2
#define RESTORE_HP 3
#define SUCK_HP 4
#define SUCK_MP 5

class buff : public Node {
public:
	CREATE_FUNC(buff);

	virtual bool init();

	void setflag(int _flag);

	void buffEffect(Hero* hero);

	void bindPicture();

	std::string getPicture();
private:
	int flag;
	std::string picture;
};

#endif
