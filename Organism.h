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
	//long int birthtime;		//��������
	int saveTime;
	int label;					//����������ʶ
	long location[2];			//������λ��
	bool life;					//�Ƿ���
public:
	virtual ~Organism();
	virtual void starve() = 0;
	virtual int move(World& w) = 0;
	virtual void breed(World& w) = 0;
	

	void setSavetime(int);		//��������
	void setLabel(int);			//��������ʶ
	void setLocation(long, long);		//����������λ��

	int getSavetime()const;
	int getLabel()const;
	const long* getLocation()const;

	int searchAround(long, long, World& w);         //��Ѱ����
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


	void getOlder();		//��������
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
	
	void getOlder(World& w);		//��������
};

class Tree :public Organism
{
public:
	Tree();
	void starve();
	int move(World& w);
	void breed(World& w);
};
