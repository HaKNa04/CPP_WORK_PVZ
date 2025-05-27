#pragma once//防止头文件重复编译
#include "../structure/structure.h"
#include <iostream>

class vector2 {
public:
	vector2(float _x = 0, float _y = 0) :x(_x), y(_y) {}
	vector2(int* data) :x(data[0]), y(data[1]) {}
	double x, y;

	// 运算符重载
	friend vector2 operator+(vector2 x, vector2 y);
	friend vector2 operator-(vector2 x, vector2 y);
	friend vector2 operator*(vector2 x, vector2 y);
	friend vector2 operator*(vector2, float);
	friend vector2 operator*(float, vector2);
	friend vector2 operator/(vector2 x, vector2 y);

	//向量模
	friend vector2 operator %(vector2 x, vector2 y);

	//叉积
	static long long cross(vector2 x, vector2 y);
	//数量积 点积
	static long long dot(vector2 x, vector2 y);
	//四舍五入除法
	static long long dv(long long a, long long b);
	//模长平方
	static long long len(vector2 x);
	//向量GCD 
	vector2 gcd(vector2 x, vector2 y);
};
//模长
long long dis(vector2 x);
//贝塞尔曲线
vector2 calcBezierPoint(float t, vector2 p0, vector2 p1, vector2 p2, vector2 p3);

//判断字符串是否分配内存
inline void strExit(char* str) {
	if (str == NULL) {
		printf("启动失败\n");
		exit(0);
	}
	else {
		return;
	}
}

// 游戏对象基类（抽象类）
class GameObject :public vector2{
public:
	GameObject(float x = 0, float y = 0) : x(x), y(y), frameIndex(0) {}
	virtual ~GameObject() = default;

	double x, y;
	int frameIndex;

	// 纯虚函数：更新逻辑和渲染
	virtual void update() = 0;
	virtual void draw() const = 0;

	// 公共方法
	int getX() const { return x; }
	int getY() const { return y; }
	void setPosition(int nx, int ny) { x = nx; y = ny; }
};

/*******************************结构体区****************************************/

//存储用户信息
typedef class USER_INFORMATION {
public:
	char name[20];					//只能是英文
	char passWord[20];				//密码必须包含多种字符
	char gender;					//性别M/F
	int UID;						//入游编号
	USER_INFORMATION* next;
}USER;

//种植植物并加以渲染使之动起来
typedef class plant :public GameObject {
public:
	int type;						//0:没有植物
	int sunShineCost;				//种植植物消耗的阳光

	bool catched;					//是否被捕
	int deadTime;					//死亡计数器

	int timer;						//0对应起点，1对应终点

	int shootTime;					//发射间隔

	int row, col;					//植物所在行列

	plant* next;
}PLANT;


//阳光的产生，停留，消失或飞到计数位置
typedef class sunshineBall :public GameObject {
public:
	int destY;						//飘落过程的目标位置的y坐标
	int timer;						//让阳光在到达目的位置时停一会

	//	float xoff;						//阳光飞到计数位置时的x坐标变化
	//	float yoff;						//阳光飞到计数位置时的y坐标变化

	float t;						// 贝塞尔曲线时间点0，1
	vector2 p1, p2, p3, p4;
	vector2 pCur;					//当前时刻阳光的位置
	float speed;					//
	int status;						//状态

	sunshineBall* next;
}SUNSHINE;

//僵尸的产生与前进
typedef class zm :public GameObject {
public:
	int speed;						//僵尸的速度
	int row;						//僵尸产生的行号
	int blood;						//僵尸的血量
	bool dead;						//是否死亡
	bool eating;					//是否正在吃植物

	struct zm* next;
}ZM;

//子弹的运动位置
typedef class bullet :public GameObject {
public:
	int row;						//子弹攻击的行号
	int speed;						//子弹飞行速度
	bool blast;						//是否爆炸

	bullet* next;
}BULLET;

//读入用户信息
void INPUT_USER_INFORMATION();

//读进文件里
void readFile(PLANT* map, ZM* zms, BULLET* bullets, SUNSHINE* balls,
	FILE* fp_bullets, FILE* fp_balls, FILE* fp_zms, FILE* fp_plant, int& is_not_store);

/********************************************************************************/

