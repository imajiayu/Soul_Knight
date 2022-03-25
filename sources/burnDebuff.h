#ifndef __BURNDEBUFF_H__
#define __BURNDEBUFF_H__

#include "deBuff.h"

class burnDebuff :public deBuff {
public:
	CREATE_FUNC(burnDebuff);
	virtual bool init();
	virtual void Buffeffect(Hero* hero);
	void update(float dt);
private:
	int buff_per;  //buff…À∫¶;
};




#endif
