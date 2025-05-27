#include "draw_graphic.h"

/*************************************渲染******************************************/
//设置字体
void set() {
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;

    // Replace the following line:  
    strcpy_s(f.lfFaceName,"Segoe UI Black");  

    // With this corrected code:  
    //wcscpy_s(f.lfFaceName, L"Segoe UI Black");

	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿效果
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);
}

//渲染植物
void drawPlants(int PLANT_COUNT, int& curPlant, int& curX, int& curY,
	IMAGE* imgBg, IMAGE* imgBar, IMAGE* imgCards, IMAGE* imgZhiWu[][20],
	plant* map) {
	transparentimage3(NULL, -100, 0, imgBg);
	transparentimage3(NULL, 160, 0, imgBar);
	for (int i = 0;i < PLANT_COUNT;i++) {
		int x = 238 + i * 65;
		int y = 6;
		transparentimage3(NULL, x, y, &imgCards[i]);
	}

	PLANT* p_map = map->next;
	while (p_map) {
		int zhiWuType = p_map->type - 1;
		int index = p_map->frameIndex;
		transparentimage3(NULL, p_map->x, p_map->y, imgZhiWu[zhiWuType][index]);
		p_map = p_map->next;
	}

	//渲染 拖动过程中的植物
	//移动到此处使得植物移动过程中是在以种植植物上方
	if (curPlant > 0) {
		IMAGE* img = imgZhiWu[curPlant - 1][0];
		transparentimage3(NULL, curX - img->getwidth() / 2, curY - img->getheight() / 2, img);
	}
}

//渲染僵尸
void drawZM(IMAGE* imgZM, IMAGE* imgZMDead, IMAGE* imgZMEat,
	zm* zms) {
	ZM* p_zms = zms->next;
	while(p_zms){
			IMAGE* img = NULL;
			if (p_zms->dead)img = imgZMDead;
			else if (p_zms->eating)img = imgZMEat;
			else img = imgZM;
			img += (p_zms->frameIndex);//用指针的偏移切换帧数
			transparentimage3(NULL, p_zms->x, p_zms->y - img->getheight(), img);
		p_zms = p_zms->next;
	}
}

//渲染阳光
void drawSunshineBalls(IMAGE* imgSunshineBall,
	sunshineBall* balls) {
	SUNSHINE* p_balls = balls->next;
	while(p_balls) {
			IMAGE* img = &imgSunshineBall[p_balls->frameIndex];
			transparentimage3(NULL, p_balls->pCur.x, p_balls->pCur.y, img);
		p_balls = p_balls->next;
	}
}

//渲染子弹--豌豆
void drawBullets(IMAGE* imgBulletNormal, IMAGE* imgBulletBlast,
	bullet* bullets) {
	BULLET* p_bullets = bullets->next;
	while (p_bullets) {
		if (p_bullets->blast) {
			IMAGE* img = &imgBulletBlast[p_bullets->frameIndex];
			transparentimage3(NULL, p_bullets->x, p_bullets->y, img);
		}
		else {
			transparentimage3(NULL, p_bullets->x, p_bullets->y, imgBulletNormal);
		}
		p_bullets = p_bullets->next;
	}
}

/***********************************************************************************/
/*************************************更新画面**************************************/

void updatePlant(IMAGE* imgZhiWu[][20], plant* map) {
	PLANT* p_map = map->next;
	while (p_map) {
		p_map->frameIndex++;
		int zhiWuType = p_map->type - 1;
		int index = p_map->frameIndex;
		if (imgZhiWu[zhiWuType][index] == NULL) {
			p_map->frameIndex = 0;
		}
		p_map = p_map->next;
	}
}

void updateSunshine(int* sunshine, int SUNSHINE_DOWN, int SUNSHINE_GROUND, int SUNSHINE_COLLECT,
	int SUNSHINE_PRODUCT, sunshineBall* balls) {
	SUNSHINE* p_balls = balls->next;
	SUNSHINE* q_balls = balls;
	while (p_balls) {
		p_balls->frameIndex = (p_balls->frameIndex + 1) % 29;
		if (p_balls->status == SUNSHINE_DOWN) {
			sunshineBall* sun = p_balls;
			sun->t += sun->speed;
			sun->pCur = sun->p1 + sun->t * (sun->p4 - sun->p1);
			if (sun->t >= 1) {
				sun->status = SUNSHINE_GROUND;
				sun->timer = 0;
			}
		}
		else if (p_balls->status == SUNSHINE_GROUND) {
			p_balls->timer++;
			if (p_balls->timer > 100) {
				q_balls->next = p_balls->next;
				free(p_balls);
				p_balls = q_balls->next;
				continue;
			}
		}
		else if (p_balls->status == SUNSHINE_COLLECT) {
			sunshineBall* sun = p_balls;
			sun->t += sun->speed;
			sun->pCur = sun->p1 + sun->t * (sun->p4 - sun->p1);
			if (sun->t > 1) {
				q_balls->next = p_balls->next;
				free(p_balls);
				p_balls = q_balls->next;
				sun->t = 0;
				(*sunshine) += 25;
				continue;
			}
		}
		else if (p_balls->status == SUNSHINE_PRODUCT) {
			sunshineBall* sun = p_balls;
			sun->t += sun->speed;
			sun->pCur = calcBezierPoint(sun->t, sun->p1, sun->p2, sun->p3, sun->p4);
			if (sun->t > 1) {
				sun->status = SUNSHINE_GROUND;
				sun->timer = 0;
			}
		}
		/*balls[i].frameIndex = (balls[i].frameIndex + 1) % 29;
		if (balls[i].timer == 0) {
			balls[i].y += 2;

		}

		if (balls[i].y >= balls[i].destY) {
			balls[i].timer++;
			if (balls[i].timer > 100) {
				balls[i].used = false;
			}
		}*/
		//}
		//else if (balls[i].xoff) {
		//	//设置阳光球偏移量
		//	float destY = 0;
		//	float destX = 262;
		//	float angle = atan((balls[i].y - destY) / (balls[i].x - destX));
		//	balls[i].xoff = 50 * cos(angle);
		//	balls[i].yoff = 50 * sin(angle);

		//	balls[i].x -= balls[i].xoff;
		//	balls[i].y -= balls[i].yoff;
		//	if (balls[i].y < 0 || balls[i].x < 262) {
		//		balls[i].xoff = 0;
		//		balls[i].yoff = 0;
		//		*sunshine += 25;
		//	}
		//}
		q_balls = p_balls;
		p_balls = p_balls->next;
	}
}

void updateZM(int& killCount, int& gameStatus, int ZM_MAX, int WIN, int FAIL,
	zm* zms) {
	ZM* p_zms;
	static int count = 0;
	count++;
	if (count > 2) {
		count = 0;
		//更新僵尸的位置
		p_zms = zms->next;
		while (p_zms)
		{
			p_zms->x -= p_zms->speed;
			if (p_zms->x < 60) {
				gameStatus = FAIL;
			}
			p_zms = p_zms->next;
		}
	}

	static int count2 = 0;
	count2++;
	if (count2 > 2) {
		count2 = 0;
		p_zms = zms->next;
		ZM* q_zms = zms;
		while (p_zms) {
			if (p_zms->dead) {
				(p_zms->frameIndex)++;
				if (p_zms->frameIndex >= 20) {
					q_zms->next = p_zms->next;
					free(p_zms);
					p_zms = q_zms->next;

					killCount++;
					if (killCount >= ZM_MAX) {
						gameStatus = WIN;
					}
					continue;
				}
			}
			else if (p_zms->eating) {
				p_zms->frameIndex = (p_zms->frameIndex + 1) % 21;
			}
			else {
				p_zms->frameIndex = (p_zms->frameIndex + 1) % 22;
			}
			q_zms = p_zms;
			p_zms = p_zms->next;
		}
	}
}

void updateBullets(int WIN_WIDTH, bullet* bullets) {
	BULLET* p_bullets = bullets->next;
	BULLET* q_bullets = bullets;
	while (p_bullets) {
		p_bullets->x += p_bullets->speed;
		if (p_bullets->x > WIN_WIDTH) {//子弹飞出屏幕
			q_bullets->next = p_bullets->next;
			free(p_bullets);
			p_bullets = q_bullets->next;
			continue;
		}
		if (p_bullets->blast) {
			p_bullets->frameIndex++;
			if (p_bullets->frameIndex >= 4) {//子弹爆炸结束
				q_bullets->next = p_bullets->next;
				free(p_bullets);
				p_bullets = q_bullets->next;
				continue;
			}
		}
		q_bullets = p_bullets;
		p_bullets = p_bullets->next;
	}
}

/************************************************************************************/