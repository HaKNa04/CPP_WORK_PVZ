#pragma once
#include <graphics.h>
#include <iostream>
#include "../structure/structure.h"
#include "../misc_function/misc_function.h"

/*************************************��Ⱦ******************************************/
//���ó�ʼ����
void set();

//��Ⱦֲ��
void drawPlants(int PLANT_COUNT, int& curPlant, int& curX, int& curY,
	IMAGE* imgBg, IMAGE* imgBar, IMAGE* imgCards, IMAGE* imgZhiWu[][20], plant* map);

//��Ⱦ��ʬ
void drawZM(IMAGE* imgZM, IMAGE* imgZMDead, IMAGE* imgZMEat,zm* zms);

//��Ⱦ����
void drawSunshineBalls(IMAGE* imgSunshineBall,sunshineBall* balls);

//��Ⱦ�ӵ�
void drawBullets(IMAGE* imgBulletNormal, IMAGE* imgBulletBlast,
	bullet* bullets);

/***********************************************************************************/
/*************************************���»���**************************************/

//����ֲ���״̬
void updatePlant(IMAGE* imgZhiWu[][20], plant* map);

// ���������״̬
void updateSunshine(int* sunshine,
	int SUNSHINE_DOWN, int SUNSHINE_GROUND, int SUNSHINE_COLLECT, int SUNSHINE_PRODUCT,
	sunshineBall* balls);

//���½�ʬ��״̬
void updateZM(int& killCount, int& gameStatus, int ZM_MAX, int WIN, int FAIL,
	zm* zms);

//�����ӵ���״̬
void updateBullets(int WIN_WIDTH,
	bullet* bullets);

/************************************************************************************/