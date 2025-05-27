#pragma once
#include <graphics.h>
#include <iostream>
#include "../structure/structure.h"
#include "../misc_function/misc_function.h"

/*************************************渲染******************************************/
//设置初始字体
void set();

//渲染植物
void drawPlants(int PLANT_COUNT, int& curPlant, int& curX, int& curY,
	IMAGE* imgBg, IMAGE* imgBar, IMAGE* imgCards, IMAGE* imgZhiWu[][20], plant* map);

//渲染僵尸
void drawZM(IMAGE* imgZM, IMAGE* imgZMDead, IMAGE* imgZMEat,zm* zms);

//渲染阳光
void drawSunshineBalls(IMAGE* imgSunshineBall,sunshineBall* balls);

//渲染子弹
void drawBullets(IMAGE* imgBulletNormal, IMAGE* imgBulletBlast,
	bullet* bullets);

/***********************************************************************************/
/*************************************更新画面**************************************/

//更新植物的状态
void updatePlant(IMAGE* imgZhiWu[][20], plant* map);

// 更新阳光的状态
void updateSunshine(int* sunshine,
	int SUNSHINE_DOWN, int SUNSHINE_GROUND, int SUNSHINE_COLLECT, int SUNSHINE_PRODUCT,
	sunshineBall* balls);

//更新僵尸的状态
void updateZM(int& killCount, int& gameStatus, int ZM_MAX, int WIN, int FAIL,
	zm* zms);

//更新子弹的状态
void updateBullets(int WIN_WIDTH,
	bullet* bullets);

/************************************************************************************/