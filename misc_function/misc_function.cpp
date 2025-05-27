#include <graphics.h>
#include "misc_function.h"
#include "../structure/structure.h"
#pragma warning(disable:4996)

void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg)      //alpha png
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

// 载入PNG图并去透明部分
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}

// 适用于 y <0 以及x<0的任何情况
void putimagePNG(int x, int y, IMAGE* picture) {

	IMAGE imgTmp, imgTmp2, imgTmp3;
	int winWidth = getwidth();
	int winHeight = getheight();
	if (y < 0) {
		SetWorkingImage(picture);
		getimage(&imgTmp, 0, -y,
			picture->getwidth(), picture->getheight() + y);
		SetWorkingImage();
		y = 0;
		picture = &imgTmp;
	}
	else if (y >= getheight() || x >= getwidth()) {
		return;
	}
	else if (y + picture->getheight() > winHeight) {
		SetWorkingImage(picture);
		getimage(&imgTmp, x, y, picture->getwidth(), winHeight - y);
		SetWorkingImage();
		picture = &imgTmp;
	}

	if (x < 0) {
		SetWorkingImage(picture);
		getimage(&imgTmp2, -x, 0, picture->getwidth() + x, picture->getheight());
		SetWorkingImage();
		x = 0;
		picture = &imgTmp2;
	}

	if (x > winWidth - picture->getwidth()) {
		SetWorkingImage(picture);
		getimage(&imgTmp3, 0, 0, winWidth - x, picture->getheight());
		SetWorkingImage();
		picture = &imgTmp3;
	}


	_putimagePNG(x, y, picture);
}

/**************************************鼠标操作区************************************/

//收集阳光
void collectSunshine(int SUNSHINE_COLLECT,
	ExMessage* msg, IMAGE* imgSunshineBall,
	 sunshineBall* balls) {
	SUNSHINE* p_balls = balls->next;
	int w = imgSunshineBall[0].getwidth();
	int h = imgSunshineBall[0].getheight();
	while (p_balls) {
		int x = p_balls->pCur.x;
		int y = p_balls->pCur.y;

		if (msg->x > x && msg->y > y
			&& msg->x < x + w
			&& msg->y < y + h) {
			p_balls->status = SUNSHINE_COLLECT;
			PlaySound("res/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);//解决点击过快没音效

			////设置阳光球偏移量
			//float destY = 0;
			//float destX = 262;
			//float angle = atan((balls[i].y - destY) / (balls[i].x - destX));
			//balls[i].xoff = 4 * cos(angle);
			//balls[i].yoff = 4 * sin(angle);

			p_balls->p1 = p_balls->pCur;
			p_balls->p4 = vector2(162, 0);
			p_balls->t = 0;
			float distance = dis(p_balls->p1 - p_balls->p4);//于重点之间的距离
			float off = 18;
			p_balls->speed = 1.0 / (distance / off);
			break;
		}
		p_balls = p_balls->next;
	}
}

void growPlant(int* curPlant, int& sunShine,
	PLANT* map, ExMessage* msg) {
	if (msg->x > 156 && msg->y > 179 && msg->y < 489) {//植物可以放的地方
		PLANT* p_map = map;
		int row = (msg->y - 179) / 103;
		int col = (msg->x - 156) / 81;
		int status = 1;
		while (p_map) {
			if (p_map->row == row && p_map->col == col) {
				status = 0;
				break;
			}
			p_map = p_map->next;
		}
		if (status) {
			PLANT* new_map = (PLANT*)malloc(sizeof(PLANT));
			if (new_map == NULL)exit(0);
			memset(new_map, 0, sizeof(PLANT));

			new_map->type = *curPlant;
			switch (*curPlant - 1) {
			case 0:sunShine -= 100;
				break;
			case 1:sunShine -= 50;
				break;
			case 2:sunShine -= 150;
				break;
			default:
				break;
			}

			new_map->row = row;
			new_map->col = col;

			new_map->x = 156 + col * 81;
			new_map->y = 179 + row * 102 + 14;//植物种植的位置

			if (map != NULL) {
				new_map->next = map->next;
				map->next = new_map;
			}
		}
	}
}

//读取用户鼠标信息——收集阳光——种植植物
void userClick(int& sunShine, const int PLANT_COUNT, int SUNSHINE_COLLECT,
	const int SUN_FLOWER, const int PEA_SHOOTER, const int CANNIBAL_FLOWER,
	int* curPlant, int* curX, int* curY, IMAGE* imgSunshineBall,
	 plant* map,  sunshineBall* balls) {

	ExMessage msg;
	static int status = 0;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x > 238 && msg.x < 238 + 65 * PLANT_COUNT && msg.y < 96) {

				int index = (msg.x - 238) / 65;
				//判断阳光值是否足够购买植物
				if (index == SUN_FLOWER && sunShine >= 50) {
					status = 1;
					*curPlant = index + 1;
					//使植物显示在点击位置，避免了植物出现在上次消失位置的小bug
					*curX = msg.x;
					*curY = msg.y;
				}
				else if (index == PEA_SHOOTER && sunShine >= 100) {
					status = 1;
					*curPlant = index + 1;
					//使植物显示在点击位置，避免了植物出现在上次消失位置的小bug
					*curX = msg.x;
					*curY = msg.y;
				}
				else if (index == CANNIBAL_FLOWER && sunShine >= 150) {
					status = 1;
					*curPlant = index + 1;
					*curX = msg.x;
					*curY = msg.y;
				}
			}
			else {
				//收集阳光
				collectSunshine(SUNSHINE_COLLECT, &msg, imgSunshineBall, balls);
			}
		}
		else if (msg.message == WM_MOUSEMOVE && status == 1) {
			*curX = msg.x;
			*curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP && status == 1) {
			growPlant(curPlant, sunShine, map, &msg);
			status = 0;
			*curPlant = 0;
		}
	}
}

//进入界面的鼠标操作
void UI() {
	IMAGE imgBg, imgMenu1, imgMenu2;
	loadimage(&imgBg, "res/menu.png");
	loadimage(&imgMenu1, "res/menu1.png");
	loadimage(&imgMenu2, "res/menu2.png");

	int flag = 0;
	while (1) {


		transparentimage3(NULL, 0, 0, &imgBg);
		transparentimage3(NULL, 474, 75, flag ? &imgMenu1 : &imgMenu2);

		ExMessage msg;
		if (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN &&
				msg.x > 474 && msg.x < 474 + 300 &&
				msg.y>75 && msg.y < 75 + 140) {
				flag = 1;

			}
			else if (msg.message == WM_LBUTTONUP && flag) {
				FlushBatchDraw();
				break;
			}
		}
		FlushBatchDraw();
	}
}

/************************************************************************************/
/**************************************物理逻辑区************************************/

//显示阳光数量
void showSunshineBallsCount(int sunshine) {
	char scoreText[8];
	sprintf_s(scoreText, sizeof(scoreText),
		"%d", sunshine);//将整型数值改为字符类型（显示类型只能是字符类型）
	int len = strlen(scoreText);
	int count_position = 200 - len * 5;//使得阳光的数值显示始终居中。
	outtextxy(count_position, 70, LPCTSTR(scoreText));//显示出阳光数
}

//碰撞检测
void checkBulletZM(ZM* zms, BULLET* bullets) {
	BULLET* p_bullets = bullets->next;
	while (p_bullets) {
		if (p_bullets->blast) {
			p_bullets = p_bullets->next;
			continue;
		}
		ZM* p_zms = zms->next;
		while (p_zms) {
			int x1 = p_zms->x + 80;
			int x2 = p_zms->x + 110;
			int x = p_bullets->x;
			if (p_zms->dead == false
				&& p_bullets->row == p_zms->row
				&& x > x1 && x < x2) {
				p_zms->blood -= 10;
				p_bullets->blast = true;
				p_bullets->speed = 0;

				if (p_zms->blood <= 0) {
					p_zms->dead = true;
					p_zms->speed = 0;
					p_zms->frameIndex = 0;
				}
				//break;//应该没用不确定先瞅瞅
			}
			p_zms = p_zms->next;
		}
		p_bullets = p_bullets->next;
	}
}

void check_CANNIBAL_FLOWER_EatZM(PLANT* map, ZM* zms, const int CANNIBAL_FLOWER) {
	PLANT* p_map = map->next;
	while (p_map) {
		if (p_map->type == CANNIBAL_FLOWER + 1) {
			int row = p_map->row;
			ZM* p_zms = zms->next;
			while (p_zms) {
				if (row == p_zms->row && p_zms->dead == false) {
					int PlantX = 156 + p_map->col * 81;
					int x1 = PlantX + 10;
					int x2 = PlantX + 60;
					int x3 = p_zms->x + 80;
					if (x3 > x1 && x3 < x2) {
						p_zms->dead = true;
						p_zms->speed = 0;
						p_zms->frameIndex = 0;
					}
				}
				p_zms = p_zms->next;
			}
		}
		p_map = p_map->next;
	}
	return;
}

void checkZMEatPlant(PLANT* map, ZM* zms) {
	ZM* p_zms = zms->next;
	while (p_zms) {
		if (p_zms->dead) {//暂时不删要播放死亡动画帧
			p_zms = p_zms->next;
			continue;
		}
		int row = p_zms->row;
		PLANT* p_map = map->next;
		PLANT* q_map = map;

		while (p_map) {
			if (p_map->row == row) {
				int PlantX = 156 + p_map->col * 81;
				int x1 = PlantX + 10;
				int x2 = PlantX + 60;
				int x3 = p_zms->x + 80;
				if (x3 > x1 && x3 < x2) {
					if (p_map->catched) {
						p_map->deadTime++;
						if (p_map->deadTime++ >= 100) {
							q_map->next = p_map->next;
							free(p_map);
							p_map = q_map->next;
							p_zms->eating = false;
							p_zms->frameIndex = 0;
							p_zms->speed = 2;
							continue;
						}
					}
					else {
						p_map->catched = true;
						p_zms->eating = true;
						p_zms->speed = 0;
						p_zms->frameIndex = 0;
					}
				}
			}
			q_map = p_map;
			p_map = p_map->next;
		}
		p_zms = p_zms->next;
	}
}

void collisionCheck(ZM* zms, BULLET* bullets, PLANT* map, const int CANNIBAL_FLOWER) {
	checkBulletZM(zms, bullets);//子弹碰僵尸判断
	checkZMEatPlant(map, zms);//僵尸吃植物判断
	check_CANNIBAL_FLOWER_EatZM(map, zms, CANNIBAL_FLOWER);//食人花吃僵尸
}

//巡游
void viewScence(int xMin,
	IMAGE imgBg, IMAGE* imgZmStand, IMAGE PrepareGrowPlants[]) {

	vector2 points[9] = {
		{550,80},{530,160},{630,170},{530,200},{515,270},
		{565,370},{605,340},{705,280},{690,340}
	};
	int index[9];
	for (int i = 0; i < 9; i++) {//巡游时僵尸抖动动作不同
		index[i] = rand() % 11;
	}
	int count = 0;

	int x = 0;

	bool user_s_house = false;
	bool right_move = false;
	bool stop = false;
	int stop_count = 0;
	bool left_move = false;
	bool start = false;
	int c = 0;
	int ci = 0;

	unsigned long long timer = 0;
	unsigned long long start_time = GetTickCount();
	unsigned long long game_timer = 0;
	unsigned long long video_timer = 0;
	while (1) {

		//使得植物摇摆起来
		timer = GetTickCount();

		if (timer - game_timer >= 50) {
			game_timer = timer;

			for (int k = 0; k < 9; k++) {
				index[k] = (index[k] + 1) % 11;
			}

		}

		if (timer - video_timer > 5) {

			if (!user_s_house)
			{
				if (stop_count <= 200)
					stop_count++;
				else
				{
					user_s_house = true;
					stop_count = 0;
				}
			}
			else if (user_s_house && !right_move)
			{
				if (x >= xMin)
					x = x - 2;
				else
					right_move = true;
			}
			else if (right_move && !stop) {
				if (stop_count <= 200)
					stop_count++;
				else
				{
					stop = true;
					stop_count = 0;
				}
			}
			else if (stop && !left_move) {
				if (x <= -100)
					x = x + 2;
				else
					left_move = true;
			}
			else if (left_move && !start)
			{
				if (c <= 60)
					c = c + 1;
				else
				{
					c = 0;
					ci = ci + 1;
				}
				if (ci >= 3)
				{
					ci = 0;
					start = true;
				}
			}
			else if (start)
			{
				break;
			}


			video_timer = timer;
			transparentimage3(NULL, x, 0, &imgBg);
			for (int k = 0; k < 9; k++) {
				transparentimage3(NULL, points[k].x - xMin + x,
					points[k].y,
					&imgZmStand[index[k]]);//巡游时僵尸的抖动图片
				if (count >= 10) {
					index[k] = (index[k] + 1) % 11;
				}
			}
			if (!user_s_house)
			{
				outtextxy(500, 400, "USER'S HOUSE");
			}
			if (left_move && !start)
				transparentimage3(NULL, 300, 200, &PrepareGrowPlants[ci]);//植物摇摆
			FlushBatchDraw();
		}
	}
}

//植物卡槽下降
void barsDown(int PLANT_COUNT, int imgBar_height,
	IMAGE imgBar, IMAGE imgBg, IMAGE* imgCards) {

	for (int y = -imgBar_height; y <= 0; y++) {

		putimage(-100, 0, &imgBg);
		putimage(160, y, &imgBar);

		for (int i = 0; i < PLANT_COUNT; i++) {
			int x = 238 + i * 65;
			y = 6 + y;
			transparentimage3(NULL, x, y, &imgCards[i]);
		}

		FlushBatchDraw();
		Sleep(20);
	}
}

/************************************************************************************/
/**************************************系统操作区************************************/

//释放内存
void free_map(PLANT* map)
{
	PLANT* p_map;
	map = map->next;
	while (map)
	{
		p_map = map->next;
		free(map);
		map = p_map;
	}
}
void free_sunshine(SUNSHINE* balls)
{
	SUNSHINE* p_balls;
	balls = balls->next;
	while (balls)
	{
		p_balls = balls->next;
		free(balls);
		balls = p_balls;
	}
}
void free_zm(ZM* zms)
{
	ZM* p_zms;
	zms = zms->next;
	while (zms)
	{
		p_zms = zms->next;
		free(zms);
		zms = p_zms;
	}
}
void free_bullet(BULLET* bullets)
{
	BULLET* p_bullets;
	bullets = bullets->next;
	while (bullets)
	{
		p_bullets = bullets->next;
		free(bullets);
		bullets = p_bullets;
	}
}


void ReadBinary() {
	FILE* fp = fopen("user_information.dat", "rb");
	if (!fp) {
		printf("文件打开失败\n");
		return;
	}
	USER* user_information = (USER*)malloc(sizeof(USER));
	if (user_information == NULL)exit(0);

	fread(user_information, sizeof(USER), 1, fp);
	printf("name:%s\npassWord:%s\ngender:%c\nUID:%d\n", user_information->name, user_information->passWord, user_information->gender, user_information->UID);

	fclose(fp);
	free(user_information);
}

/************************************************************************************/
/**************************************游戏判定区************************************/

//存档
void judge_store(PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets) {
	printf("是否保存这次游戏的信息？(Y/N)\n");
	char ch;
	while (scanf(" %c", &ch)) {
		if (ch == 'N') {
			break;
		}
		else if (ch == 'Y') {
			FILE* fp_plant = fopen("game_information_plant.dat", "wb+");
			FILE* fp_zms = fopen("game_information_zms.dat", "wb+");
			FILE* fp_balls = fopen("game_information_sunshine.dat", "wb+");
			FILE* fp_bullets = fopen("game_information_bullets.dat", "wb+");
			if (!fp_plant && !fp_zms && !fp_balls && !fp_bullets) {
				printf("文件打开失败\n");
				return;
			}
			PLANT* p_map = map->next;
			while (p_map) {
				fwrite(p_map, sizeof(PLANT), 1, fp_plant);
				p_map = p_map->next;
			}
			ZM* p_zms = zms->next;
			while (p_zms) {
				fwrite(p_zms, sizeof(ZM), 1, fp_zms);
				p_zms = p_zms->next;
			}
			SUNSHINE* p_balls = balls->next;
			while (p_balls) {
				fwrite(p_balls, sizeof(SUNSHINE), 1, fp_balls);
				p_balls = p_balls->next;
			}
			BULLET* p_bullets = bullets->next;
			while (p_bullets) {
				fwrite(p_bullets, sizeof(BULLET), 1, fp_bullets);
				p_bullets = p_bullets->next;
			}
			printf("存储成功，期待您的下次启动，再见。\n");

			fclose(fp_plant);
			fclose(fp_zms);
			fclose(fp_balls);
			fclose(fp_bullets);
			break;
		}
		else {
			printf("输入错误，请在3秒后重新输入\n");
			Sleep(3000);
			fflush(stdin);
			system("cls");
		}
	}
}

void store_per_50s(PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets) {
	static int judge = 0;
	judge++;
	if (judge >= 50) {
		judge = 0;
		FILE* fp_plant = fopen("game_information_plant.dat", "wb+");
		FILE* fp_zms = fopen("game_information_zms.dat", "wb+");
		FILE* fp_balls = fopen("game_information_sunshine.dat", "wb+");
		FILE* fp_bullets = fopen("game_information_bullets.dat", "wb+");
		if (!fp_plant && !fp_zms && !fp_balls && !fp_bullets) {
			printf("文件打开失败\n");
			return;
		}
		PLANT* p_map = map->next;
		while (p_map) {
			fwrite(p_map, sizeof(PLANT), 1, fp_plant);
			p_map = p_map->next;
		}
		ZM* p_zms = zms->next;
		while (p_zms) {
			fwrite(p_zms, sizeof(ZM), 1, fp_zms);
			p_zms = p_zms->next;
		}
		SUNSHINE* p_balls = balls->next;
		while (p_balls) {
			fwrite(p_balls, sizeof(SUNSHINE), 1, fp_balls);
			p_balls = p_balls->next;
		}
		BULLET* p_bullets = bullets->next;
		while (p_bullets) {
			fwrite(p_bullets, sizeof(BULLET), 1, fp_bullets);
			p_bullets = p_bullets->next;
		}

		fclose(fp_plant);
		fclose(fp_zms);
		fclose(fp_balls);
		fclose(fp_bullets);
	}
}

//判断游戏是否结束
bool checkOver(int gameStatus, int WIN, int FAIL, int WIN_WIDTH, int WIN_HEIGHT,
	PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets) {

	bool ret = false;
	if (gameStatus == WIN) {
		Sleep(2000);
		ReadBinary();
		judge_store(map, zms, balls, bullets);

		loadimage(0, "res/gameWin.png", WIN_WIDTH, WIN_HEIGHT, true);
		mciSendString("play res/win.mp3", 0, 0, 0);
		ret = true;
	}
	else if (gameStatus == FAIL) {
		Sleep(2000);
		ReadBinary();
		judge_store(map, zms, balls, bullets);

		loadimage(0, "res/gameFail.png", WIN_WIDTH, WIN_HEIGHT, true);
		mciSendString("play res/lose.mp3", 0, 0, 0);
		ret = true;
	}
	FlushBatchDraw();
	return ret;
}

inline bool fileExist(const char* name) {
	FILE* fp;
	fopen_s(&fp, name, "r");
	if (fp) {
		fclose(fp);
	}
	return fp != NULL;
}

/************************************************************************************/
/********************************初始化操作******************************************/

//初始化存地址的中间变量
char name[64];

//初始化僵尸
void initZM(IMAGE* imgZM) {
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		if (fileExist(name))
			loadimage(&imgZM[i], LPCTSTR(name));
		else return;
	}
	return;
}

//初始化植物——卡片+植物
void initPlants(int PLANT_COUNT,
	IMAGE* imgCards, IMAGE* imgZhiWu[][20]) {

	for (int i = 0; i < PLANT_COUNT; i++) {
		//生成植物卡牌文件名
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		//先判断文件是否存在
		if (fileExist(name)) {
			loadimage(&imgCards[i], LPCTSTR(name));
		}
		else {
			break;
		}

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
			//先判断这个文件是否存在
			if (fileExist(name)) {
				imgZhiWu[i][j] = new IMAGE;
				loadimage(imgZhiWu[i][j], LPCTSTR(name));
			}
			else {
				break;
			}
		}
	}
	return;
}

//游戏初始背景
void initBG(int WIN_WIDTH, int WIN_HEIGHT,
	IMAGE* imgBg, IMAGE* imgBar) {
	//加载有背景背景图片
	//把字符集改为“多字节字符集”
	sprintf_s(name, sizeof(name), "res/bg.jpg");
	if (fileExist(name))
		loadimage(imgBg, LPCTSTR(name));
	else return;
	sprintf_s(name, sizeof(name), "res/bar5.png");
	if (fileExist(name))
		loadimage(imgBar, LPCTSTR(name));
	else return;

	//创建游戏的图形窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	BeginBatchDraw(); //开始批量绘图
}

//初始化阳光
void initSunshine(IMAGE* imgSunshineBall) {
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		if (name) {
			loadimage(&imgSunshineBall[i], LPCTSTR(name));
		}
		else {
			return;
		}
	}
	return;
}

//初始化普通状态的子弹
void initBulletNormal(IMAGE* imgBulletNormal) {
	strcpy_s(name, "res/bullets/bullet_normal.png");
	if (fileExist(name)) {
		loadimage(imgBulletNormal, LPCTSTR(name));
	}
	return;
}

//初始化爆炸状态的子弹
void initBulletBlast(IMAGE* imgBulletBlast) {
	sprintf_s(name, sizeof(name), "res/bullets/bullet_blast.png");
	if (fileExist(name))
		loadimage(&imgBulletBlast[3], LPCTSTR(name));
	else return;
	for (int i = 0; i < 3; i++) {//逐步放大
		float k = (i + 1) * 0.2;
		loadimage(&imgBulletBlast[i]
			, LPCTSTR(name)
			, imgBulletBlast[3].getwidth() * k
			, imgBulletBlast[3].getheight() * k
			, true);
	}
	return;
}

//初始化僵尸死亡
void initZMDead(IMAGE* imgZMDead) {
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/zm_dead/%d.png", i + 1);
		if (fileExist(name))
			loadimage(&imgZMDead[i], LPCTSTR(name));
		else break;
	}
	return;
}

//初始化僵尸吃植物
void initZMEat(IMAGE* imgZMEat) {
	for (int i = 0; i < 21; i++) {
		sprintf_s(name, "res/zm_eat/%d.png", i + 1);
		if (fileExist(name))
			loadimage(&imgZMEat[i], LPCTSTR(name));
		else break;
	}
	return;
}

//片场巡游僵尸的状态
void initZmStand(IMAGE* imgZmStand) {
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, sizeof(name), "res/zm_stand/%d.png", i + 1);
		if (fileExist(name))
			loadimage(&imgZmStand[i], LPCTSTR(name));
		else break;
	}
	return;
}

/*************************************************************************************/
/**************************************创建*****************************************/

void createSunshine(int SUNSHINE_DOWN, int SUNSHINE_PRODUCT, int SUN_FLOWER,
	IMAGE* imgZhiWu[][20], IMAGE* imgSunshineBall,
	plant* map, sunshineBall* balls) {

	static int shinecount = 0;
	static int fre = 200;
	shinecount++;
	if (shinecount >= fre) {
		fre = 200 + rand() % 50;//随机刷新时间
		shinecount = 0;

		SUNSHINE* new_balls = (SUNSHINE*)malloc(sizeof(SUNSHINE));
		if (new_balls == NULL)exit(0);
		memset(new_balls, 0, sizeof(SUNSHINE));

		new_balls->status = SUNSHINE_DOWN;
		new_balls->p1 = vector2(160 + rand() % (900 - 360), 60);
		new_balls->p4 = vector2(new_balls->p1.x, 200 + (rand() % 4) * 90);
		int off = 2;
		float distance = new_balls->p4.y - new_balls->p1.y;
		new_balls->speed = 1.0 / (distance / off);
		new_balls->next = balls->next;
		balls->next = new_balls;
	}
	PLANT* p_map = map->next;
	while (p_map) {
		if (p_map->type == SUN_FLOWER + 1) {
			p_map->timer++;
			if (p_map->timer > 200) {
				p_map->timer = 0;

				SUNSHINE* new_balls = (SUNSHINE*)malloc(sizeof(SUNSHINE));
				if (new_balls == NULL)exit(0);
				memset(new_balls, 0, sizeof(SUNSHINE));

				new_balls->p1 = vector2(p_map->x, p_map->y);//起始点坐标
				int w = (100) * (rand() % 2 ? 1 : -1);//使得向日葵产生阳光在左右两边
				new_balls->p4 = vector2(p_map->x + w,
					p_map->y + imgZhiWu[SUN_FLOWER][0]->getheight() -
					imgSunshineBall[0].getheight());//终点坐标
				new_balls->p2 = vector2(new_balls->p1.x + w * 0.3, new_balls->p1.y - 100);
				new_balls->p3 = vector2(new_balls->p1.x + w * 0.7, new_balls->p1.y - 100);
				new_balls->status = SUNSHINE_PRODUCT;

				new_balls->speed = 0.05;

				new_balls->next = balls->next;
				balls->next = new_balls;
			}
		}
		p_map = p_map->next;
	}
}

void createZM(int& zmCount, int WIN_WIDTH, int ZM_MAX,
	zm* zms) {
	if (zmCount >= ZM_MAX) {
		return;
	}

	static int zmFre = 200;
	static int zmcount = 0;
	zmcount++;
	if (zmcount > zmFre) {
		zmcount = 0;
		zmFre = rand() % 30 + 200;
		ZM* new_zm = (ZM*)malloc(sizeof(ZM));
		if (new_zm == NULL)exit(0);
		memset(new_zm, 0, sizeof(ZM));

		new_zm->x = WIN_WIDTH;
		new_zm->row = rand() % 3;
		new_zm->y = 172 + (1 + new_zm->row) * 100;
		new_zm->speed = 2;
		new_zm->blood = 100;

		new_zm->next = zms->next;
		zms->next = new_zm;
		zmCount++;
	}
}

//发射子弹--豌豆
void shoot(int WIN_WIDTH, int PEA_SHOOTER,
	IMAGE* imgZhiWu[][20], IMAGE* imgZM,
	plant* map, zm* zms, bullet* bullets) {
	int lines[3] = { 0 };
	int dangerX = WIN_WIDTH - imgZM[0].getwidth() + 75;
	ZM* p_zms = zms->next;
	while (p_zms) {
		if (p_zms->x < dangerX) {
			lines[p_zms->row] = p_zms->x;
		}
		p_zms = p_zms->next;
	}
	PLANT* p_map = map->next;
	while (p_map) {
		if (p_map->type == PEA_SHOOTER + 1
			&& p_map->x <= lines[p_map->row]) {//使得僵尸在植物后面时不攻击
			p_map->shootTime++;
			if (p_map->shootTime > 25) {
				p_map->shootTime = 0;
				//for (k = 0;k < bulletMax && bullets[k].used;k++);//千万不要搞错这是寻找而不是循环
				BULLET* new_bullets = (BULLET*)malloc(sizeof(BULLET));
				if (new_bullets == NULL)exit(0);
				memset(new_bullets, 0, sizeof(BULLET));

				new_bullets->row = p_map->row;
				new_bullets->speed = 4;

				int plantX = 156 + p_map->col * 81;
				int plantY = 179 + p_map->row * 102 + 10;
				//子弹的初始位置
				new_bullets->x = plantX + imgZhiWu[p_map->type - 1][0]->getwidth() - 10;
				new_bullets->y = plantY + 7;

				new_bullets->next = bullets->next;
				bullets->next = new_bullets;
			}
		}
		p_map = p_map->next;
	}
}

/***********************************************************************************/