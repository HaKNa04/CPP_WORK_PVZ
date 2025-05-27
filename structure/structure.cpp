#include "structure.h"
#include <cmath>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//加法
vector2 operator +(vector2 x, vector2 y)
{
	return vector2(x.x + y.x, x.y + y.y);
}
//减法
vector2 operator -(vector2 x, vector2 y) 
{
	return vector2(x.x - y.x, x.y - y.y);
}
// 乘法
vector2 operator *(vector2 x, vector2 y) 
{
	return vector2(x.x * y.x - x.y * y.y, x.y * y.x + x.x * y.y);
}
// 乘法
vector2 operator *(vector2 y, float x) 
{
	return vector2(x * y.x, x * y.y);
}
vector2 operator *(float x, vector2 y)
{
	return vector2(x * y.x, x * y.y);
}

//叉积
long long vector2::cross(vector2 x, vector2 y)
{ 
	return x.y * y.x - x.x * y.y; 
}

//数量积 点积
long long vector2::dot(vector2 x, vector2 y)
{ 
	return x.x * y.x + x.y * y.y;
}

//四舍五入除法
long long vector2::dv(long long a, long long b)
{//注意重名！！！ 
	return b < 0 ? dv(-a, -b)
		: (a < 0 ? -dv(-a, b)
			: (a + b / 2) / b);
}

//模长平方
long long vector2::len(vector2 x) 
{ 
	return x.x * x.x + x.y * x.y; 
}

//模长
long long dis(vector2 x)
{ 
	return sqrt(x.x * x.x + x.y * x.y); 
}

//向量除法
vector2 operator /(vector2 x, vector2 y) 
{
	long long l = vector2::len(y);
	return vector2(vector2::dv(vector2::dot(x, y), l), vector2::dv(vector2::cross(x, y), l));
}

//向量膜
vector2 operator %(vector2 x, vector2 y) 
{ 
	return x - ((x / y) * y); 
}

//向量GCD 
vector2 vector2::gcd(vector2 x, vector2 y) 
{
	return len(y) ? vector2::gcd(y, x % y) : x; 
}

//贝塞尔曲线
vector2 calcBezierPoint(float t, vector2 p0, vector2 p1, vector2 p2, vector2 p3)
{
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	vector2 p = uuu * p0;
	p = p + 3 * uu * t * p1;
	p = p + 3 * u * tt * p2;
	p = p + ttt * p3;

	return p;
}

//判断名字输入是否正确
bool name_right(char* name) {
	int len = strlen(name);
	if (len > 20)return false;
	for (int i = 0; name[i] != '\0'; i++) {
		if (name[i] > 'z' || (name[i] > 'Z' && name[i] < 'a') || name[i] < 'a') {
			return false;
		}
	}
	return true;
}

//判断密码是否安全
bool passWord_safe(char* passWord) {
	int len = strlen(passWord);
	int  lowercase_letter = 0, capital_letter = 0, number = 0, other = 0;
	for (int i = 0; passWord[i] != '\0'; i++)
	{
		if (passWord[i] >= 'a' && passWord[i] <= 'z')
			lowercase_letter = 1;
		else if (passWord[i] >= 'A' && passWord[i] <= 'Z')
			capital_letter = 1;
		else if (passWord[i] >= '0' && passWord[i] <= '9')
			number = 1;
		else
			other = 1;
	}
	if (lowercase_letter + capital_letter + number + other >= 3 && len >= 8 && len <= 16)
		return true;
	else
		return false;
}

//读入用户名
char* readIn_userName() {
	char* name = (char*)malloc(sizeof(char) * 22);
	strExit(name);
	while (1) {
		printf("请输入用户名(只能是英文,不能超过二十个字母)\n");
		char* str = (char*)malloc(sizeof(char) * 50);
		strExit(str);
		if (str == NULL) {
			continue;
		}
		scanf_s("%s", str, sizeof(str));
		if (name_right(str)) {
			strcpy(name, str);
			system("cls");
			fflush(stdin);
			break;
		}
		else {
			printf("输入错误请在三秒后重新输入\n");
			Sleep(3000);
			fflush(stdin);//清除缓冲区
			system("cls");
		}
	}
	return name;
}

//读入用户密码
char* readIn_passWord() {
	char* passWord = (char*)malloc(sizeof(char) * 20);
	strExit(passWord);
	while (1) {
		printf("请设置密码(必须包含特殊字符、大小写字母、数字四者其三，长度在8到18之间)\n");
		char* str1 = (char*)malloc(sizeof(char) * 50);
		strExit(passWord);
		if (str1 == NULL) {
			continue;
		}
		scanf_s("%s", str1, sizeof(char) * 50);
		if (passWord_safe(str1)) {
			printf("请再输入一次\n");
			char* str2 = (char*)malloc(sizeof(char) * 50);
			if (str2 == NULL) {
				continue;
			}
			scanf_s("%s", str2, sizeof(char) * 50);
			if (strcmp(str1, str2) != 0) {
				printf("两次输入不同，请在三秒后重新输入密码\n");
				free(str1); free(str2);
				Sleep(3000);
				fflush(stdin);
				system("cls");
				continue;
			}
			else {
				strcpy(passWord, str1);
				free(str1); free(str2);
				fflush(stdin);
				system("cls");
				break;
			}
		}
		else {
			printf("您的密码不安全，请在三秒后重新输入\n");
			free(str1);
			Sleep(3000);
			fflush(stdin);
			system("cls");
		}
	}
	return passWord;
}

//读入用户性别
char readIn_gender() {
	char gender;
	while (1) {
		printf("请输入您的性别(M/F)\n");
		char ch;
		scanf_s(" %c", &ch);
		if (ch == 'F' || ch == 'M') {
			gender = ch;
			fflush(stdin);
			system("cls");
			break;
		}
		else {
			printf("您的输入错误，请在三秒后重新输入\n");
			Sleep(3000);
			fflush(stdin);
			system("cls");
		}
	}
	return gender;
}

void writeInFile_binary(USER* head) {
	FILE* fp = fopen("user_information.dat", "wb+");
	if (!fp) {
		printf("文件打开失败\n");
		return;
	}

	USER* p = head;
	while (p) {
		fwrite(p, sizeof(USER), 1, fp);
		p = p->next;
	}

	fclose(fp);
	return;
}

//创建用户信息
void INPUT_USER_INFORMATION() {
	USER* head = (USER*)malloc(sizeof(USER));
	if (head == NULL) {
		printf("启动失败\n");
		exit(0);
	}
	head->next = NULL;

	strcpy(head->name, readIn_userName());
	strcpy(head->passWord, readIn_passWord());

	head->gender = readIn_gender();
	printf("角色创建成功，您的信息为:\n");
	printf("名称%s\n", head->name);
	printf("密码%s\n", head->passWord);
	printf("性别%c\n", head->gender);
	printf("是否确认无误？(Y/N)\n");
	fflush(stdin);
	char ch;
	while (scanf(" %c", &ch)) {
		if (ch == 'Y')break;
		else if (ch != 'Y' && ch != 'N') {
			printf("输入错误请在三秒后重新输入\n");
			Sleep(3000);
		}

	}

	srand(time(NULL));
	head->UID = (rand()) % 100000000;

	writeInFile_binary(head);

	printf("欢迎您的到来，亲爱的%s\n", head->name);
	Sleep(2000);
	printf("您的UID为%d\n", head->UID);
	Sleep(2000);
	printf("游戏将在五秒后开启，祝你游戏愉快\n");
	Sleep(5000);
	system("cls");

	free(head);
}

void readFile(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls,
	FILE* fp_bullets, FILE* fp_balls, FILE* fp_zms, FILE* fp_plant, int& is_not_store) {

	while (fp_plant) {
		PLANT* new_map = (PLANT*)malloc(sizeof(PLANT));
		if (new_map == NULL)exit(0);
		new_map->next = NULL;
		if (fread(new_map, sizeof(PLANT), 1, fp_plant) != 1)break;
		new_map->next = map->next;
		map->next = new_map;
	}
	while (fp_zms) {
		ZM* new_zm = (ZM*)malloc(sizeof(ZM));
		if (new_zm == NULL)exit(0);
		new_zm->next = NULL;
		if (fread(new_zm, sizeof(ZM), 1, fp_zms) != 1)break;
		new_zm->next = zms->next;
		zms->next = new_zm;
	}
	while (fp_balls) {
		SUNSHINE* new_balls = (SUNSHINE*)malloc(sizeof(SUNSHINE));
		if (new_balls == NULL)exit(0);
		new_balls->next = NULL;
		if (fread(new_balls, sizeof(SUNSHINE), 1, fp_balls) != 1)break;
		new_balls->next = balls->next;
		balls->next = new_balls;
	}
	while (fp_bullets) {
		BULLET* new_bullets = (BULLET*)malloc(sizeof(BULLET));
		if (new_bullets == NULL)exit(0);
		new_bullets->next = NULL;
		if (fread(new_bullets, sizeof(BULLET), 1, fp_bullets) != 1)break;
		new_bullets->next = bullets->next;
		bullets->next = new_bullets;
	}
	printf("游戏读档成功，欢迎回来！\n");
	is_not_store = 1;
	Sleep(2000);
	system("cls");
	if (fp_plant)
		fclose(fp_plant);
	if (fp_zms)
		fclose(fp_zms);
	if (fp_balls)
		fclose(fp_balls);
	if (fp_bullets)
		fclose(fp_bullets);
	return;
}