#include "structure.h"
#include <cmath>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//�ӷ�
vector2 operator +(vector2 x, vector2 y)
{
	return vector2(x.x + y.x, x.y + y.y);
}
//����
vector2 operator -(vector2 x, vector2 y) 
{
	return vector2(x.x - y.x, x.y - y.y);
}
// �˷�
vector2 operator *(vector2 x, vector2 y) 
{
	return vector2(x.x * y.x - x.y * y.y, x.y * y.x + x.x * y.y);
}
// �˷�
vector2 operator *(vector2 y, float x) 
{
	return vector2(x * y.x, x * y.y);
}
vector2 operator *(float x, vector2 y)
{
	return vector2(x * y.x, x * y.y);
}

//���
long long vector2::cross(vector2 x, vector2 y)
{ 
	return x.y * y.x - x.x * y.y; 
}

//������ ���
long long vector2::dot(vector2 x, vector2 y)
{ 
	return x.x * y.x + x.y * y.y;
}

//�����������
long long vector2::dv(long long a, long long b)
{//ע������������ 
	return b < 0 ? dv(-a, -b)
		: (a < 0 ? -dv(-a, b)
			: (a + b / 2) / b);
}

//ģ��ƽ��
long long vector2::len(vector2 x) 
{ 
	return x.x * x.x + x.y * x.y; 
}

//ģ��
long long dis(vector2 x)
{ 
	return sqrt(x.x * x.x + x.y * x.y); 
}

//��������
vector2 operator /(vector2 x, vector2 y) 
{
	long long l = vector2::len(y);
	return vector2(vector2::dv(vector2::dot(x, y), l), vector2::dv(vector2::cross(x, y), l));
}

//����Ĥ
vector2 operator %(vector2 x, vector2 y) 
{ 
	return x - ((x / y) * y); 
}

//����GCD 
vector2 vector2::gcd(vector2 x, vector2 y) 
{
	return len(y) ? vector2::gcd(y, x % y) : x; 
}

//����������
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

//�ж����������Ƿ���ȷ
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

//�ж������Ƿ�ȫ
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

//�����û���
char* readIn_userName() {
	char* name = (char*)malloc(sizeof(char) * 22);
	strExit(name);
	while (1) {
		printf("�������û���(ֻ����Ӣ��,���ܳ�����ʮ����ĸ)\n");
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
			printf("������������������������\n");
			Sleep(3000);
			fflush(stdin);//���������
			system("cls");
		}
	}
	return name;
}

//�����û�����
char* readIn_passWord() {
	char* passWord = (char*)malloc(sizeof(char) * 20);
	strExit(passWord);
	while (1) {
		printf("����������(������������ַ�����Сд��ĸ����������������������8��18֮��)\n");
		char* str1 = (char*)malloc(sizeof(char) * 50);
		strExit(passWord);
		if (str1 == NULL) {
			continue;
		}
		scanf_s("%s", str1, sizeof(char) * 50);
		if (passWord_safe(str1)) {
			printf("��������һ��\n");
			char* str2 = (char*)malloc(sizeof(char) * 50);
			if (str2 == NULL) {
				continue;
			}
			scanf_s("%s", str2, sizeof(char) * 50);
			if (strcmp(str1, str2) != 0) {
				printf("�������벻ͬ�����������������������\n");
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
			printf("�������벻��ȫ�������������������\n");
			free(str1);
			Sleep(3000);
			fflush(stdin);
			system("cls");
		}
	}
	return passWord;
}

//�����û��Ա�
char readIn_gender() {
	char gender;
	while (1) {
		printf("�����������Ա�(M/F)\n");
		char ch;
		scanf_s(" %c", &ch);
		if (ch == 'F' || ch == 'M') {
			gender = ch;
			fflush(stdin);
			system("cls");
			break;
		}
		else {
			printf("����������������������������\n");
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
		printf("�ļ���ʧ��\n");
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

//�����û���Ϣ
void INPUT_USER_INFORMATION() {
	USER* head = (USER*)malloc(sizeof(USER));
	if (head == NULL) {
		printf("����ʧ��\n");
		exit(0);
	}
	head->next = NULL;

	strcpy(head->name, readIn_userName());
	strcpy(head->passWord, readIn_passWord());

	head->gender = readIn_gender();
	printf("��ɫ�����ɹ���������ϢΪ:\n");
	printf("����%s\n", head->name);
	printf("����%s\n", head->passWord);
	printf("�Ա�%c\n", head->gender);
	printf("�Ƿ�ȷ������(Y/N)\n");
	fflush(stdin);
	char ch;
	while (scanf(" %c", &ch)) {
		if (ch == 'Y')break;
		else if (ch != 'Y' && ch != 'N') {
			printf("������������������������\n");
			Sleep(3000);
		}

	}

	srand(time(NULL));
	head->UID = (rand()) % 100000000;

	writeInFile_binary(head);

	printf("��ӭ���ĵ������װ���%s\n", head->name);
	Sleep(2000);
	printf("����UIDΪ%d\n", head->UID);
	Sleep(2000);
	printf("��Ϸ�������������ף����Ϸ���\n");
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
	printf("��Ϸ�����ɹ�����ӭ������\n");
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