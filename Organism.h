#pragma once
#include"control.h"
#include<list>
#include<vector>
using std::vector;
using std::list;
using namespace std;

class World;
class Organism
{
private:
	//long int birthtime;		//出生日期
	int saveTime;
	int label;					//生物体类别标识
	long location[2];			//生物体位置
	bool life;					//是否存活
public:
	virtual ~Organism();
	virtual void starve() = 0;
	virtual int move(World& w) = 0;
	virtual void breed(World& w) = 0;
	

	void setSavetime(int);		//设置年龄
	void setLabel(int);			//设置类别标识
	void setLocation(long, long);		//设置生物体位置

	int getSavetime()const;
	int getLabel()const;
	const long* getLocation()const;

	int searchAround(long, long, World& w);         //搜寻四周
	void setLife(bool y);
	bool isdeath();

};

class Sheep:public Organism
{
public:
	Sheep();
	int move(World& w);
	void breed(World& w);
	void starve();


	void getOlder();		//年龄增加
};

class Wolf :public Organism
{
private:
	int starveTime;
public:
	Wolf();
	void starve();
	int move(World& w);
	void breed(World& w);

	
	int searchSheep(long, long, World& w);
	
	void getOlder(World& w);		//年龄增加
};

class Tree :public Organism
{
public:
	Tree();
	void starve();
	int move(World& w);
	void breed(World& w);
};
