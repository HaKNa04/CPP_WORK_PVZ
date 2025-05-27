/*
������־
1.��������Ŀ������Ŀģ�壩
2.�����ز�
3.ʵ�ֿ�ʼ����Ϸ����
4.ʵ����Ϸ����
*/

#include <Windows.h>
#include <mmsystem.h>

#include <graphics.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mmsystem.h>//������Ч
#include "../draw_graphic/draw_graphic.h"

#include "../structure/structure.h"
#include "../misc_function/misc_function.h"

#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996)


#define SFML_STATIC
//#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <vector>

//������Ϸ����ĸ����
#define LAWN_ROW 3		//��Ϸ��ƺ����
#define LAWN_COL 9
#define WIN_WIDTH 900
#define WIN_HEIGHT 600
#define ZM_MAX 10		//��ʬ�������

	//ö��ֲ������
	enum {
		PEA_SHOOTER,//�㶹����
		SUN_FLOWER,//���տ�
		CANNIBAL_FLOWER,//ʳ�˻�
		PLANT_COUNT//ֲ������
	};

	//����״̬
	enum {
		SUNSHINE_DOWN,//���⽵��
		SUNSHINE_GROUND,//�������
		SUNSHINE_COLLECT,//�����ռ�
		SUNSHINE_PRODUCT//��������
	};

	//��Ϸ״̬
	enum {
		GOING,//��Ϸ����
		WIN,//��Ϸʤ��
		FAIL//��Ϸʧ��
	};

	/************************************ȫ�ֱ�����*******************************/

	int is_not_store = 0;					//�Ƿ��Ǵ浵����1�ǣ�0����
	int sunshine = 100;						//���������
	int curX, curY;							//��ǰѡ��ֲ����ƶ������е�λ��
	int curPlant;							//0:û��ѡ�У�1��ѡ����һ��ֲ��
	int killCount;							//�Ѿ�ɱ���Ľ�ʬ����
	int zmCount;							//��ʬ��������
	int gameStatus;							//��Ϸ״̬������Ӧö��

	/*****************************************************************************/


	/************************************ͼƬ��*******************************/

	IMAGE imgBg;						//��ʾ����ͼƬ
	IMAGE imgBar;						//��ʾֲ����
	IMAGE imgSunshineBall[29];			//��ʾ�������֡��
	IMAGE imgZM[22];					//��ʾ��ʬ��֡��
	IMAGE imgBulletNormal;				//�������ӵ�
	IMAGE imgBulletBlast[4];			//��ը���ӵ�
	IMAGE imgZMDead[20];				//��ʬ��������
	IMAGE imgZMEat[21];					//��ֲ�ﶯ��
	IMAGE* imgZhiWu[PLANT_COUNT][20];	//��ʾ��ÿһ����ֲ��ֲ��
	IMAGE imgCards[PLANT_COUNT];		//��ʾֲ�￨Ƭ
	IMAGE imgZmStand[11];				//Ƭ��Ѳ�ν�ʬ��״̬
	IMAGE PrepareGrowPlants[3];

	/*******************************************************************************/
//���»���
void updateWindow(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls) {
	//��Ⱦ˳�����Ҳ�Ȼ�Ḳ��

	//��Ⱦֲ��
	drawPlants(PLANT_COUNT, curPlant, curX, curY, &imgBg, &imgBar, imgCards, imgZhiWu, map);

	//��ʾ��������
	showSunshineBallsCount(sunshine);

	//��Ⱦ��ʬ
	drawZM(imgZM, imgZMDead, imgZMEat, zms);

	//��Ⱦ����
	drawSunshineBalls(imgSunshineBall, balls);

	//��Ⱦ�ӵ�
	drawBullets(&imgBulletNormal, imgBulletBlast, bullets);

	FlushBatchDraw();//ˢ��˫����
}

//��Ϸ��ʼ��
void gameInit(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls) {

	initBG( WIN_WIDTH, WIN_HEIGHT, &imgBg, &imgBar);

	initZM(imgZM);

	loadimage(&PrepareGrowPlants[0], "./res/PrepareGrowPlants/frame_0.Png");
	loadimage(&PrepareGrowPlants[1], "./res/PrepareGrowPlants/frame_1.Png");
	loadimage(&PrepareGrowPlants[2], "./res/PrepareGrowPlants/frame_2.Png");

	initPlants(PLANT_COUNT,  imgCards, imgZhiWu);

	initSunshine(imgSunshineBall);

	initBulletNormal(&imgBulletNormal);

	initBulletBlast(imgBulletBlast);

	initZMDead(imgZMDead);

	initZMEat(imgZMEat);

	initZmStand(imgZmStand);

	curPlant = 0;

	//�����������
	srand(time(NULL));

	//������������������
	set();
}

//ˢ����Ϸ�ڵĶ�̬
void updateGame(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls) {

	//�л�����֡�γɶ�̬����

	updatePlant(imgZhiWu, map);

	createSunshine(SUNSHINE_DOWN, SUNSHINE_PRODUCT, SUN_FLOWER, imgZhiWu, imgSunshineBall, map, balls);//��������
	updateSunshine(&sunshine, SUNSHINE_DOWN, SUNSHINE_GROUND, SUNSHINE_COLLECT, SUNSHINE_PRODUCT, balls);//��������״̬

	createZM(zmCount, WIN_WIDTH, ZM_MAX, zms);//������ʬ
	updateZM(killCount, gameStatus, ZM_MAX, WIN, FAIL, zms);//���½�ʬ״̬

	shoot(WIN_WIDTH, PEA_SHOOTER, imgZhiWu, imgZM, map, zms, bullets);//�����㶹�ӵ�
	updateBullets(WIN_WIDTH, bullets);//�����ӵ�״̬

	collisionCheck(zms, bullets, map, CANNIBAL_FLOWER);//��ײ���

	store_per_50s(map, zms, balls, bullets);//�浵
}

//��ʼ����
void startUI(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls) {
	if (!is_not_store) {
		UI();

		int xMin = WIN_WIDTH - imgBg.getwidth();//900-1400=-500
		viewScence(xMin, imgBg, imgZmStand, PrepareGrowPlants);

		int height = imgBar.getheight();
		barsDown(PLANT_COUNT, height, imgBar, imgBg, imgCards);
	}

	unsigned long long timer = 0;
	unsigned long long game_timer = 0;
	unsigned long long video_timer = 0;
	while (1) {
		userClick(sunshine,
			PLANT_COUNT, SUNSHINE_COLLECT, SUN_FLOWER, PEA_SHOOTER, CANNIBAL_FLOWER,
			&curPlant, &curX, &curY,imgSunshineBall, map, balls);

		//ʹ��ֲ��ҡ������
		timer = GetTickCount();
		if (timer - game_timer > 50) {
			game_timer = timer;
			updateGame(map, zms, bullets, balls);
			if (checkOver(gameStatus, WIN, FAIL, WIN_WIDTH,
				WIN_HEIGHT, map, zms, balls, bullets)) {
				break;
			}
		}
		if (timer - video_timer > 5) {
			video_timer = timer;
			updateWindow(map, zms, bullets, balls);
		}
	}
}


void already_store_game(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls) {
	FILE* fp_user = fopen("user_information.dat", "rb");
	if (!fp_user) {
		INPUT_USER_INFORMATION();
	}
	else {
		printf("�Ƿ�ʹ����һ�ε��û���Ϣ��(Y/N)\n");
		char ch;
		while (scanf(" %c", &ch)) {
			if (ch == 'Y') {
				USER* user_information = (USER*)malloc(sizeof(USER));
				if (user_information == NULL)exit(0);

				fread(user_information, sizeof(USER), 1, fp_user);
				printf("name:%s\npassWord:%s\ngender:%c\nUID:%d\n", user_information->name, user_information->passWord, user_information->gender, user_information->UID);
				printf("��ӭ������\n");
				Sleep(2000);
				fclose(fp_user);
				break;
			}
			else if (ch != 'Y' && ch != 'y' && ch != 'n' && ch != 'N') {
				printf("�����������������\n");
				fflush(stdin);
			}
			else {
				INPUT_USER_INFORMATION();
			}
		}
	}

	FILE* fp_plant = fopen("game_information_plant.dat", "rb");
	FILE* fp_zms = fopen("game_information_zms.dat", "rb");
	FILE* fp_balls = fopen("game_information_sunshine.dat", "rb");
	FILE* fp_bullets = fopen("game_information_bullets.dat", "rb");
	if (!fp_plant && !fp_zms && !fp_balls && !fp_bullets) {

		goto STARTGAME;

	}
	else {
		printf("�Ƿ������һ�ε���Ϸ��(Y/N)\n");
		char ch;
		while (scanf(" %c", &ch)) {
			if (ch == 'Y'|| ch == 'y') {
				readFile(map, zms, bullets, balls,
					fp_bullets, fp_balls, fp_zms, fp_plant, is_not_store);

				goto STARTGAME;

			}
			else if (ch != 'Y' && ch != 'y' && ch != 'n' && ch != 'N') {
				printf("������������������������\n");
				fflush(stdin);
				Sleep(3000);
			}
			else {
				
				goto STARTGAME;

			}
		}
	}

STARTGAME:
	gameInit(map, zms, bullets, balls);
	startUI(map, zms, bullets, balls);
}


int main() {
	/*********************************ͷ�ڵ�***********************************/

	PLANT* map = (PLANT*)malloc(sizeof(PLANT));
	if (map == NULL)exit(-2);
	map->next = NULL;

	SUNSHINE* balls = (SUNSHINE*)malloc(sizeof(SUNSHINE));
	if (balls == NULL)exit(-3);
	balls->next = NULL;

	ZM* zms = (ZM*)malloc(sizeof(ZM));
	if (zms == NULL)exit(-4);
	zms->next = NULL;

	BULLET* bullets = (BULLET*)malloc(sizeof(BULLET));
	if (bullets == NULL)exit(-5);
	bullets->next = NULL;

	/***********************************************************************************/
	// ������Ч������
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("res/sunshine.wav")) // ȷ���ļ�����
		return EXIT_FAILURE;

	// �洢��Ծ��Ч������
	std::vector<sf::Sound> activeSounds;

	killCount = 0;
	zmCount = 0;
	gameStatus = GOING;

	already_store_game(map, zms, bullets, balls);

	EndBatchDraw();//����˫����

	free_map(map);
	free_sunshine(balls);
	free_zm(zms);
	free_bullet(bullets);
	system("pause");
	return 0;
}