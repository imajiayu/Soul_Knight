#ifndef _BIGKNIFE_H__
#define _BIGKNIFE_H__
#pragma once
#include "Weapon.h"
#include "BigKnifeController.h"
#define DAO_HERO 1
#define DAO_MONSTER 2

class BigKnife :public Weapon
{
public:
	//构造函数
	BigKnife(BigKnifeController* hitController, int attackNum, String name);

	BigKnife(int attackNum, String name);
	//射击管理器
	BigKnifeController* hitController;
};



#endif // !_BIGKNIFE_H__
