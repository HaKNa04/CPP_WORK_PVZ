#pragma once
#include <iostream>
#include <graphics.h>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include "../structure/structure.h"
#pragma comment( lib, "MSIMG32.LIB" )
void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg);      //alpha png

// 载入PNG图并去透明部分
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture);

// 适用于 y <0 以及x<0的任何情况
void putimagePNG(int  picture_x, int picture_y, IMAGE* picture);

/**************************************鼠标操作区************************************/

//收集阳光
void collectSunshine(int SUNSHINE_COLLECT,
	ExMessage* msg, IMAGE* imgSunshineBall,
	sunshineBall* balls);

void growPlant(int* curPlant, int& sunShine,
	PLANT* map, ExMessage* msg);

//读取用户鼠标信息——收集阳光——种植植物
void userClick(int& sunShine,
	int PLANT_COUNT, int SUNSHINE_COLLECT,
	const int SUN_FLOWER, const int PEA_SHOOTER, const int CANNIBAL_FLOWER,
	int* curPlant, int* curX, int* curY,
	IMAGE* imgSunshineBall,
	 plant* map,  sunshineBall* balls);

//进入界面的鼠标操作
void UI();

/************************************************************************************/
/**************************************物理逻辑区************************************/

//显示阳光数量
void showSunshineBallsCount(char a[]);

//碰撞检测
void collisionCheck(ZM* zms, BULLET* bullets, PLANT* map, const int CANNIBAL_FLOWER);

void check_CANNIBAL_FLOWER_EatZM(PLANT* map, ZM* zms, const int CANNIBAL_FLOWER);

void checkZMEatPlant(PLANT* map, ZM* zms);

void collisionCheck(ZM* zms, BULLET* bullets, PLANT* map, const int CANNIBAL_FLOWER);

//巡游
void viewScence(int xMin, IMAGE imgBg, IMAGE* imgZmStand, IMAGE PrepareGrowPlants[]);

//卡槽下滑特效
void barsDown(int PLANT_COUNT, int imgBar_height, IMAGE imgBar, IMAGE imgBg, IMAGE* imgCards);

/************************************************************************************/
/**************************************系统操作区************************************/

//释放内存
void free_map(PLANT* map);
void free_sunshine(SUNSHINE* balls);
void free_zm(ZM* zms);
void free_bullet(BULLET* bullets);

void ReadBinary();

/************************************************************************************/
/**************************************游戏判定区************************************/

//存档
void judge_store(PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets);

void store_per_50s(PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets);

//判断游戏是否结束
bool checkOver(int gameStatus, int WIN, int FAIL, int WIN_WIDTH, int WIN_HEIGHT,
	PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets);

inline bool fileExist(const char* name);

/************************************************************************************/
/********************************初始化操作******************************************/

//初始化僵尸
void initZM(IMAGE* imgZM);

//初始化植物——卡片+植物
void initPlants(int PLANT_COUNT, IMAGE* imgCards, IMAGE* imgZhiWu[][20]);

//游戏初始背景
void initBG(int WIN_WIDTH, int WIN_HEIGHT, IMAGE* imgBg, IMAGE* imgBar);

//初始化阳光
void initSunshine(IMAGE* imgSunshineBall);

//初始化普通状态的子弹
void initBulletNormal(IMAGE* imgBulletNormal);

//初始化爆炸状态的子弹
void initBulletBlast(IMAGE* imgBulletBlast);

//初始化僵尸死亡
void initZMDead(IMAGE* imgZMDead);

//初始化僵尸吃植物
void initZMEat(IMAGE* imgZMEat);

//片场巡游僵尸的状态
void initZmStand(IMAGE* imgZmStand);

/*************************************************************************************/
/**************************************创建*****************************************/

void createSunshine(int SUNSHINE_DOWN, int SUNSHINE_PRODUCT, int SUN_FLOWER,
	IMAGE* imgZhiWu[][20], IMAGE* imgSunshineBall,
	plant* map, sunshineBall* balls);

void createZM(int& zmCount, int WIN_WIDTH, int ZM_MAX,
	zm* zms);

//发射子弹--豌豆
void shoot(int WIN_WIDTH, int PEA_SHOOTER,
	IMAGE* imgZhiWu[][20], IMAGE* imgZM,
	plant* map, zm* zms, bullet* bullets);
/***********************************************************************************/