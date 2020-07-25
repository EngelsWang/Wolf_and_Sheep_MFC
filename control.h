#pragma once
#include"Organism.h"
#include"TestOneDlg.h"
#include<vector>
#include<list>
using std::vector;
using std::list;

class Organism;
class Wolf;
class Sheep;
class Tree;
class World
{
private:
	//Timestep time;
	vector<vector<Organism*>> mapmodel;			//生物体指针空间矩阵
	list<Wolf> wolfs;					//狼链表
	list<Sheep> sheeps;					//羊链表
	list<Tree> trees;
public:
	World(long size = 10, long numSheep = 20, long numWolf = 2);
	World(const char*);
	void randomCreateWorld(long size, long numSheep, long numWolf);					//多方式创建世界
	void showWorld();					//显示
	void refreshWorld();				//刷新

	long getSheeps();
	long getWolfs();
	long getMapsize();
	Organism* getMappoint(long i, long j);

	void saveWorld(const char*);
	void loadWorld(const char* );

	friend class Organism;
	friend class Wolf;
	friend class Sheep;
	friend class Tree;

	friend bool CTestOneDlg::writeFile();
	friend bool CTestOneDlg::loadFile();
};


