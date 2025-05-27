/*
开发日志
1.创建新项目（空项目模板）
2.导入素材
3.实现开始的游戏场景
4.实现游戏顶部
*/

#include <Windows.h>
#include <mmsystem.h>

#include <graphics.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mmsystem.h>//播放音效
#include "../draw_graphic/draw_graphic.h"

#include "../structure/structure.h"
#include "../misc_function/misc_function.h"

#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996)


#define SFML_STATIC
//#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <vector>

//设置游戏界面的高与宽
#define LAWN_ROW 3		//游戏草坪行数
#define LAWN_COL 9
#define WIN_WIDTH 900
#define WIN_HEIGHT 600
#define ZM_MAX 10		//僵尸最多数量

	//枚举植物类型
	enum {
		PEA_SHOOTER,//豌豆射手
		SUN_FLOWER,//向日葵
		CANNIBAL_FLOWER,//食人花
		PLANT_COUNT//植物数量
	};

	//阳光状态
	enum {
		SUNSHINE_DOWN,//阳光降落
		SUNSHINE_GROUND,//阳光落地
		SUNSHINE_COLLECT,//阳光收集
		SUNSHINE_PRODUCT//阳光生产
	};

	//游戏状态
	enum {
		GOING,//游戏进行
		WIN,//游戏胜利
		FAIL//游戏失败
	};

	/************************************全局变量区*******************************/

	int is_not_store = 0;					//是否是存档内容1是，0不是
	int sunshine = 100;						//阳光的数量
	int curX, curY;							//当前选中植物，在移动过程中的位置
	int curPlant;							//0:没有选中，1：选择了一个植物
	int killCount;							//已经杀掉的僵尸个数
	int zmCount;							//僵尸出现数量
	int gameStatus;							//游戏状态——对应枚举

	/*****************************************************************************/


	/************************************图片区*******************************/

	IMAGE imgBg;						//表示背景图片
	IMAGE imgBar;						//表示植物栏
	IMAGE imgSunshineBall[29];			//表示阳光球的帧号
	IMAGE imgZM[22];					//表示僵尸的帧号
	IMAGE imgBulletNormal;				//正常的子弹
	IMAGE imgBulletBlast[4];			//爆炸的子弹
	IMAGE imgZMDead[20];				//僵尸死亡动画
	IMAGE imgZMEat[21];					//吃植物动画
	IMAGE* imgZhiWu[PLANT_COUNT][20];	//表示在每一行种植的植物
	IMAGE imgCards[PLANT_COUNT];		//表示植物卡片
	IMAGE imgZmStand[11];				//片场巡游僵尸的状态
	IMAGE PrepareGrowPlants[3];

	/*******************************************************************************/
//更新画面
void updateWindow(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls) {
	//渲染顺序不能乱不然会覆盖

	//渲染植物
	drawPlants(PLANT_COUNT, curPlant, curX, curY, &imgBg, &imgBar, imgCards, imgZhiWu, map);

	//显示阳光数量
	showSunshineBallsCount(sunshine);

	//渲染僵尸
	drawZM(imgZM, imgZMDead, imgZMEat, zms);

	//渲染阳光
	drawSunshineBalls(imgSunshineBall, balls);

	//渲染子弹
	drawBullets(&imgBulletNormal, imgBulletBlast, bullets);

	FlushBatchDraw();//刷新双缓冲
}

//游戏初始化
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

	//配置随机种子
	srand(time(NULL));

	//设置字体如阳光数量
	set();
}

//刷新游戏内的动态
void updateGame(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls) {

	//切换动画帧形成动态画面

	updatePlant(imgZhiWu, map);

	createSunshine(SUNSHINE_DOWN, SUNSHINE_PRODUCT, SUN_FLOWER, imgZhiWu, imgSunshineBall, map, balls);//创建阳光
	updateSunshine(&sunshine, SUNSHINE_DOWN, SUNSHINE_GROUND, SUNSHINE_COLLECT, SUNSHINE_PRODUCT, balls);//更新阳光状态

	createZM(zmCount, WIN_WIDTH, ZM_MAX, zms);//创建僵尸
	updateZM(killCount, gameStatus, ZM_MAX, WIN, FAIL, zms);//更新僵尸状态

	shoot(WIN_WIDTH, PEA_SHOOTER, imgZhiWu, imgZM, map, zms, bullets);//发射豌豆子弹
	updateBullets(WIN_WIDTH, bullets);//更新子弹状态

	collisionCheck(zms, bullets, map, CANNIBAL_FLOWER);//碰撞检测

	store_per_50s(map, zms, balls, bullets);//存档
}

//初始界面
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

		//使得植物摇摆起来
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
		printf("是否使用上一次的用户信息？(Y/N)\n");
		char ch;
		while (scanf(" %c", &ch)) {
			if (ch == 'Y') {
				USER* user_information = (USER*)malloc(sizeof(USER));
				if (user_information == NULL)exit(0);

				fread(user_information, sizeof(USER), 1, fp_user);
				printf("name:%s\npassWord:%s\ngender:%c\nUID:%d\n", user_information->name, user_information->passWord, user_information->gender, user_information->UID);
				printf("欢迎回来！\n");
				Sleep(2000);
				fclose(fp_user);
				break;
			}
			else if (ch != 'Y' && ch != 'y' && ch != 'n' && ch != 'N') {
				printf("输入错误请重新输入\n");
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
		printf("是否继续上一次的游戏？(Y/N)\n");
		char ch;
		while (scanf(" %c", &ch)) {
			if (ch == 'Y'|| ch == 'y') {
				readFile(map, zms, bullets, balls,
					fp_bullets, fp_balls, fp_zms, fp_plant, is_not_store);

				goto STARTGAME;

			}
			else if (ch != 'Y' && ch != 'y' && ch != 'n' && ch != 'N') {
				printf("输入错误请在三秒后重新输入\n");
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
	/*********************************头节点***********************************/

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
	// 加载音效缓冲区
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("res/sunshine.wav")) // 确保文件存在
		return EXIT_FAILURE;

	// 存储活跃音效的容器
	std::vector<sf::Sound> activeSounds;

	killCount = 0;
	zmCount = 0;
	gameStatus = GOING;

	already_store_game(map, zms, bullets, balls);

	EndBatchDraw();//结束双缓冲

	free_map(map);
	free_sunshine(balls);
	free_zm(zms);
	free_bullet(bullets);
	system("pause");
	return 0;
}