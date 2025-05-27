#pragma once
#include <iostream>
#include <graphics.h>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include "../structure/structure.h"
#pragma comment( lib, "MSIMG32.LIB" )
void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg);      //alpha png

// ����PNGͼ��ȥ͸������
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture);

// ������ y <0 �Լ�x<0���κ����
void putimagePNG(int  picture_x, int picture_y, IMAGE* picture);

/**************************************��������************************************/

//�ռ�����
void collectSunshine(int SUNSHINE_COLLECT,
	ExMessage* msg, IMAGE* imgSunshineBall,
	sunshineBall* balls);

void growPlant(int* curPlant, int& sunShine,
	PLANT* map, ExMessage* msg);

//��ȡ�û������Ϣ�����ռ����⡪����ֲֲ��
void userClick(int& sunShine,
	int PLANT_COUNT, int SUNSHINE_COLLECT,
	const int SUN_FLOWER, const int PEA_SHOOTER, const int CANNIBAL_FLOWER,
	int* curPlant, int* curX, int* curY,
	IMAGE* imgSunshineBall,
	 plant* map,  sunshineBall* balls);

//��������������
void UI();

/************************************************************************************/
/**************************************�����߼���************************************/

//��ʾ��������
void showSunshineBallsCount(char a[]);

//��ײ���
void collisionCheck(ZM* zms, BULLET* bullets, PLANT* map, const int CANNIBAL_FLOWER);

void check_CANNIBAL_FLOWER_EatZM(PLANT* map, ZM* zms, const int CANNIBAL_FLOWER);

void checkZMEatPlant(PLANT* map, ZM* zms);

void collisionCheck(ZM* zms, BULLET* bullets, PLANT* map, const int CANNIBAL_FLOWER);

//Ѳ��
void viewScence(int xMin, IMAGE imgBg, IMAGE* imgZmStand, IMAGE PrepareGrowPlants[]);

//�����»���Ч
void barsDown(int PLANT_COUNT, int imgBar_height, IMAGE imgBar, IMAGE imgBg, IMAGE* imgCards);

/************************************************************************************/
/**************************************ϵͳ������************************************/

//�ͷ��ڴ�
void free_map(PLANT* map);
void free_sunshine(SUNSHINE* balls);
void free_zm(ZM* zms);
void free_bullet(BULLET* bullets);

void ReadBinary();

/************************************************************************************/
/**************************************��Ϸ�ж���************************************/

//�浵
void judge_store(PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets);

void store_per_50s(PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets);

//�ж���Ϸ�Ƿ����
bool checkOver(int gameStatus, int WIN, int FAIL, int WIN_WIDTH, int WIN_HEIGHT,
	PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets);

inline bool fileExist(const char* name);

/************************************************************************************/
/********************************��ʼ������******************************************/

//��ʼ����ʬ
void initZM(IMAGE* imgZM);

//��ʼ��ֲ�����Ƭ+ֲ��
void initPlants(int PLANT_COUNT, IMAGE* imgCards, IMAGE* imgZhiWu[][20]);

//��Ϸ��ʼ����
void initBG(int WIN_WIDTH, int WIN_HEIGHT, IMAGE* imgBg, IMAGE* imgBar);

//��ʼ������
void initSunshine(IMAGE* imgSunshineBall);

//��ʼ����ͨ״̬���ӵ�
void initBulletNormal(IMAGE* imgBulletNormal);

//��ʼ����ը״̬���ӵ�
void initBulletBlast(IMAGE* imgBulletBlast);

//��ʼ����ʬ����
void initZMDead(IMAGE* imgZMDead);

//��ʼ����ʬ��ֲ��
void initZMEat(IMAGE* imgZMEat);

//Ƭ��Ѳ�ν�ʬ��״̬
void initZmStand(IMAGE* imgZmStand);

/*************************************************************************************/
/**************************************����*****************************************/

void createSunshine(int SUNSHINE_DOWN, int SUNSHINE_PRODUCT, int SUN_FLOWER,
	IMAGE* imgZhiWu[][20], IMAGE* imgSunshineBall,
	plant* map, sunshineBall* balls);

void createZM(int& zmCount, int WIN_WIDTH, int ZM_MAX,
	zm* zms);

//�����ӵ�--�㶹
void shoot(int WIN_WIDTH, int PEA_SHOOTER,
	IMAGE* imgZhiWu[][20], IMAGE* imgZM,
	plant* map, zm* zms, bullet* bullets);
/***********************************************************************************/