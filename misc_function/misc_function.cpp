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

// ����PNGͼ��ȥ͸������
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
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

// ������ y <0 �Լ�x<0���κ����
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

/**************************************��������************************************/

//�ռ�����
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
			PlaySound("res/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);//����������û��Ч

			////����������ƫ����
			//float destY = 0;
			//float destX = 262;
			//float angle = atan((balls[i].y - destY) / (balls[i].x - destX));
			//balls[i].xoff = 4 * cos(angle);
			//balls[i].yoff = 4 * sin(angle);

			p_balls->p1 = p_balls->pCur;
			p_balls->p4 = vector2(162, 0);
			p_balls->t = 0;
			float distance = dis(p_balls->p1 - p_balls->p4);//���ص�֮��ľ���
			float off = 18;
			p_balls->speed = 1.0 / (distance / off);
			break;
		}
		p_balls = p_balls->next;
	}
}

void growPlant(int* curPlant, int& sunShine,
	PLANT* map, ExMessage* msg) {
	if (msg->x > 156 && msg->y > 179 && msg->y < 489) {//ֲ����Էŵĵط�
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
			new_map->y = 179 + row * 102 + 14;//ֲ����ֲ��λ��

			if (map != NULL) {
				new_map->next = map->next;
				map->next = new_map;
			}
		}
	}
}

//��ȡ�û������Ϣ�����ռ����⡪����ֲֲ��
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
				//�ж�����ֵ�Ƿ��㹻����ֲ��
				if (index == SUN_FLOWER && sunShine >= 50) {
					status = 1;
					*curPlant = index + 1;
					//ʹֲ����ʾ�ڵ��λ�ã�������ֲ��������ϴ���ʧλ�õ�Сbug
					*curX = msg.x;
					*curY = msg.y;
				}
				else if (index == PEA_SHOOTER && sunShine >= 100) {
					status = 1;
					*curPlant = index + 1;
					//ʹֲ����ʾ�ڵ��λ�ã�������ֲ��������ϴ���ʧλ�õ�Сbug
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
				//�ռ�����
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

//��������������
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
/**************************************�����߼���************************************/

//��ʾ��������
void showSunshineBallsCount(int sunshine) {
	char scoreText[8];
	sprintf_s(scoreText, sizeof(scoreText),
		"%d", sunshine);//��������ֵ��Ϊ�ַ����ͣ���ʾ����ֻ�����ַ����ͣ�
	int len = strlen(scoreText);
	int count_position = 200 - len * 5;//ʹ���������ֵ��ʾʼ�վ��С�
	outtextxy(count_position, 70, LPCTSTR(scoreText));//��ʾ��������
}

//��ײ���
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
				//break;//Ӧ��û�ò�ȷ���ȳ��
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
		if (p_zms->dead) {//��ʱ��ɾҪ������������֡
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
	checkBulletZM(zms, bullets);//�ӵ�����ʬ�ж�
	checkZMEatPlant(map, zms);//��ʬ��ֲ���ж�
	check_CANNIBAL_FLOWER_EatZM(map, zms, CANNIBAL_FLOWER);//ʳ�˻��Խ�ʬ
}

//Ѳ��
void viewScence(int xMin,
	IMAGE imgBg, IMAGE* imgZmStand, IMAGE PrepareGrowPlants[]) {

	vector2 points[9] = {
		{550,80},{530,160},{630,170},{530,200},{515,270},
		{565,370},{605,340},{705,280},{690,340}
	};
	int index[9];
	for (int i = 0; i < 9; i++) {//Ѳ��ʱ��ʬ����������ͬ
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

		//ʹ��ֲ��ҡ������
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
					&imgZmStand[index[k]]);//Ѳ��ʱ��ʬ�Ķ���ͼƬ
				if (count >= 10) {
					index[k] = (index[k] + 1) % 11;
				}
			}
			if (!user_s_house)
			{
				outtextxy(500, 400, "USER'S HOUSE");
			}
			if (left_move && !start)
				transparentimage3(NULL, 300, 200, &PrepareGrowPlants[ci]);//ֲ��ҡ��
			FlushBatchDraw();
		}
	}
}

//ֲ�￨���½�
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
/**************************************ϵͳ������************************************/

//�ͷ��ڴ�
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
		printf("�ļ���ʧ��\n");
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
/**************************************��Ϸ�ж���************************************/

//�浵
void judge_store(PLANT* map, ZM* zms, SUNSHINE* balls, BULLET* bullets) {
	printf("�Ƿ񱣴������Ϸ����Ϣ��(Y/N)\n");
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
				printf("�ļ���ʧ��\n");
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
			printf("�洢�ɹ����ڴ������´��������ټ���\n");

			fclose(fp_plant);
			fclose(fp_zms);
			fclose(fp_balls);
			fclose(fp_bullets);
			break;
		}
		else {
			printf("�����������3�����������\n");
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
			printf("�ļ���ʧ��\n");
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

//�ж���Ϸ�Ƿ����
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
/********************************��ʼ������******************************************/

//��ʼ�����ַ���м����
char name[64];

//��ʼ����ʬ
void initZM(IMAGE* imgZM) {
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		if (fileExist(name))
			loadimage(&imgZM[i], LPCTSTR(name));
		else return;
	}
	return;
}

//��ʼ��ֲ�����Ƭ+ֲ��
void initPlants(int PLANT_COUNT,
	IMAGE* imgCards, IMAGE* imgZhiWu[][20]) {

	for (int i = 0; i < PLANT_COUNT; i++) {
		//����ֲ�￨���ļ���
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		//���ж��ļ��Ƿ����
		if (fileExist(name)) {
			loadimage(&imgCards[i], LPCTSTR(name));
		}
		else {
			break;
		}

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
			//���ж�����ļ��Ƿ����
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

//��Ϸ��ʼ����
void initBG(int WIN_WIDTH, int WIN_HEIGHT,
	IMAGE* imgBg, IMAGE* imgBar) {
	//�����б�������ͼƬ
	//���ַ�����Ϊ�����ֽ��ַ�����
	sprintf_s(name, sizeof(name), "res/bg.jpg");
	if (fileExist(name))
		loadimage(imgBg, LPCTSTR(name));
	else return;
	sprintf_s(name, sizeof(name), "res/bar5.png");
	if (fileExist(name))
		loadimage(imgBar, LPCTSTR(name));
	else return;

	//������Ϸ��ͼ�δ���
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	BeginBatchDraw(); //��ʼ������ͼ
}

//��ʼ������
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

//��ʼ����ͨ״̬���ӵ�
void initBulletNormal(IMAGE* imgBulletNormal) {
	strcpy_s(name, "res/bullets/bullet_normal.png");
	if (fileExist(name)) {
		loadimage(imgBulletNormal, LPCTSTR(name));
	}
	return;
}

//��ʼ����ը״̬���ӵ�
void initBulletBlast(IMAGE* imgBulletBlast) {
	sprintf_s(name, sizeof(name), "res/bullets/bullet_blast.png");
	if (fileExist(name))
		loadimage(&imgBulletBlast[3], LPCTSTR(name));
	else return;
	for (int i = 0; i < 3; i++) {//�𲽷Ŵ�
		float k = (i + 1) * 0.2;
		loadimage(&imgBulletBlast[i]
			, LPCTSTR(name)
			, imgBulletBlast[3].getwidth() * k
			, imgBulletBlast[3].getheight() * k
			, true);
	}
	return;
}

//��ʼ����ʬ����
void initZMDead(IMAGE* imgZMDead) {
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/zm_dead/%d.png", i + 1);
		if (fileExist(name))
			loadimage(&imgZMDead[i], LPCTSTR(name));
		else break;
	}
	return;
}

//��ʼ����ʬ��ֲ��
void initZMEat(IMAGE* imgZMEat) {
	for (int i = 0; i < 21; i++) {
		sprintf_s(name, "res/zm_eat/%d.png", i + 1);
		if (fileExist(name))
			loadimage(&imgZMEat[i], LPCTSTR(name));
		else break;
	}
	return;
}

//Ƭ��Ѳ�ν�ʬ��״̬
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
/**************************************����*****************************************/

void createSunshine(int SUNSHINE_DOWN, int SUNSHINE_PRODUCT, int SUN_FLOWER,
	IMAGE* imgZhiWu[][20], IMAGE* imgSunshineBall,
	plant* map, sunshineBall* balls) {

	static int shinecount = 0;
	static int fre = 200;
	shinecount++;
	if (shinecount >= fre) {
		fre = 200 + rand() % 50;//���ˢ��ʱ��
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

				new_balls->p1 = vector2(p_map->x, p_map->y);//��ʼ������
				int w = (100) * (rand() % 2 ? 1 : -1);//ʹ�����տ�������������������
				new_balls->p4 = vector2(p_map->x + w,
					p_map->y + imgZhiWu[SUN_FLOWER][0]->getheight() -
					imgSunshineBall[0].getheight());//�յ�����
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

//�����ӵ�--�㶹
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
			&& p_map->x <= lines[p_map->row]) {//ʹ�ý�ʬ��ֲ�����ʱ������
			p_map->shootTime++;
			if (p_map->shootTime > 25) {
				p_map->shootTime = 0;
				//for (k = 0;k < bulletMax && bullets[k].used;k++);//ǧ��Ҫ�������Ѱ�Ҷ�����ѭ��
				BULLET* new_bullets = (BULLET*)malloc(sizeof(BULLET));
				if (new_bullets == NULL)exit(0);
				memset(new_bullets, 0, sizeof(BULLET));

				new_bullets->row = p_map->row;
				new_bullets->speed = 4;

				int plantX = 156 + p_map->col * 81;
				int plantY = 179 + p_map->row * 102 + 10;
				//�ӵ��ĳ�ʼλ��
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