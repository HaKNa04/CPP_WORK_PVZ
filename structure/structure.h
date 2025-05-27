#pragma once//��ֹͷ�ļ��ظ�����
#include "../structure/structure.h"
#include <iostream>

class vector2 {
public:
	vector2(float _x = 0, float _y = 0) :x(_x), y(_y) {}
	vector2(int* data) :x(data[0]), y(data[1]) {}
	double x, y;

	// ���������
	friend vector2 operator+(vector2 x, vector2 y);
	friend vector2 operator-(vector2 x, vector2 y);
	friend vector2 operator*(vector2 x, vector2 y);
	friend vector2 operator*(vector2, float);
	friend vector2 operator*(float, vector2);
	friend vector2 operator/(vector2 x, vector2 y);

	//����ģ
	friend vector2 operator %(vector2 x, vector2 y);

	//���
	static long long cross(vector2 x, vector2 y);
	//������ ���
	static long long dot(vector2 x, vector2 y);
	//�����������
	static long long dv(long long a, long long b);
	//ģ��ƽ��
	static long long len(vector2 x);
	//����GCD 
	vector2 gcd(vector2 x, vector2 y);
};
//ģ��
long long dis(vector2 x);
//����������
vector2 calcBezierPoint(float t, vector2 p0, vector2 p1, vector2 p2, vector2 p3);

//�ж��ַ����Ƿ�����ڴ�
inline void strExit(char* str) {
	if (str == NULL) {
		printf("����ʧ��\n");
		exit(0);
	}
	else {
		return;
	}
}

// ��Ϸ������ࣨ�����ࣩ
class GameObject :public vector2{
public:
	GameObject(float x = 0, float y = 0) : x(x), y(y), frameIndex(0) {}
	virtual ~GameObject() = default;

	double x, y;
	int frameIndex;

	// ���麯���������߼�����Ⱦ
	virtual void update() = 0;
	virtual void draw() const = 0;

	// ��������
	int getX() const { return x; }
	int getY() const { return y; }
	void setPosition(int nx, int ny) { x = nx; y = ny; }
};

/*******************************�ṹ����****************************************/

//�洢�û���Ϣ
typedef class USER_INFORMATION {
public:
	char name[20];					//ֻ����Ӣ��
	char passWord[20];				//���������������ַ�
	char gender;					//�Ա�M/F
	int UID;						//���α��
	USER_INFORMATION* next;
}USER;

//��ֲֲ�ﲢ������Ⱦʹ֮������
typedef class plant :public GameObject {
public:
	int type;						//0:û��ֲ��
	int sunShineCost;				//��ֲֲ�����ĵ�����

	bool catched;					//�Ƿ񱻲�
	int deadTime;					//����������

	int timer;						//0��Ӧ��㣬1��Ӧ�յ�

	int shootTime;					//������

	int row, col;					//ֲ����������

	plant* next;
}PLANT;


//����Ĳ�����ͣ������ʧ��ɵ�����λ��
typedef class sunshineBall :public GameObject {
public:
	int destY;						//Ʈ����̵�Ŀ��λ�õ�y����
	int timer;						//�������ڵ���Ŀ��λ��ʱͣһ��

	//	float xoff;						//����ɵ�����λ��ʱ��x����仯
	//	float yoff;						//����ɵ�����λ��ʱ��y����仯

	float t;						// ����������ʱ���0��1
	vector2 p1, p2, p3, p4;
	vector2 pCur;					//��ǰʱ�������λ��
	float speed;					//
	int status;						//״̬

	sunshineBall* next;
}SUNSHINE;

//��ʬ�Ĳ�����ǰ��
typedef class zm :public GameObject {
public:
	int speed;						//��ʬ���ٶ�
	int row;						//��ʬ�������к�
	int blood;						//��ʬ��Ѫ��
	bool dead;						//�Ƿ�����
	bool eating;					//�Ƿ����ڳ�ֲ��

	struct zm* next;
}ZM;

//�ӵ����˶�λ��
typedef class bullet :public GameObject {
public:
	int row;						//�ӵ��������к�
	int speed;						//�ӵ������ٶ�
	bool blast;						//�Ƿ�ը

	bullet* next;
}BULLET;

//�����û���Ϣ
void INPUT_USER_INFORMATION();

//�����ļ���
void readFile(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls,
	FILE* fp_bullets, FILE* fp_balls, FILE* fp_zms, FILE* fp_plant, int& is_not_store);

/********************************************************************************/

