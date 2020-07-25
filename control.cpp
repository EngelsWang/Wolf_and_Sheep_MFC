#include "pch.h"
#include"control.h"

#include<random>
#include<iostream>
#include<ctime>
#include<fstream>
using std::uniform_real_distribution;
using namespace std;

World::World(long size, long numSheep, long numWolf)
{
	randomCreateWorld(size, numSheep, numWolf);//随机创建世界

}

World::World(const char* filename)
{
	loadWorld(filename);
}

void World::randomCreateWorld(long size, long numSheep, long numWolf)
{
	mapmodel.assign(size, vector<Organism*>(size, NULL));
	std::cout << mapmodel[1][1] << std::endl;

	vector<bool> site((size - 2) * (size - 2), false);//判断是否为空



	for (long x, y, i = 0; i < numSheep; i++)
	{
		uniform_int_distribution<long> u(0, (size - 2) * (size - 2) - 1);				// 定义一个范围为0~1的实性分布类型
		default_random_engine e(time(0) + i + numSheep);// 定义一个随机数引擎
		long rand = u(e);
		while (site[rand] == true)
		{
			rand = (rand + 1) % ((size - 2) * (size - 2));
		}
		site[rand] = true;
		x = rand / (size - 2) + 1;
		y = rand % (size - 2) + 1;

		Sheep* sheep = new Sheep;
		sheep->setLocation(x, y);

		//
		sheeps.push_back(*sheep);
		mapmodel[x][y] = &sheeps.back();
	}
	for (long x, y, i = 0; i < numWolf; i++)
	{
		uniform_int_distribution<long> u(0, (size - 2) * (size - 2) - 1);				// 定义一个范围为0~1的实性分布类型
		default_random_engine e(time(0) + i + numSheep);// 定义一个随机数引擎
		long rand = u(e);
		while (site[rand] == true)
		{
			rand = (rand + 1) % ((size - 2) * (size - 2));
		}
		site[rand] = true;
		x = rand / (size - 2) + 1;
		y = rand % (size - 2) + 1;


		Wolf* wolf = new Wolf;
		wolf->setLocation(x, y);

		//
		wolfs.push_back(*wolf);
		mapmodel[x][y] = &wolfs.back();
	}
	//设置树作为限制
	Tree* tmpTree = new Tree;
	trees.push_back(*tmpTree);
	for (long i = 0; i < size; i++)
	{
		mapmodel[size - 1][i] = tmpTree;
		mapmodel[i][size - 1] = tmpTree;
		mapmodel[0][i] = tmpTree;
		mapmodel[i][0] = tmpTree;
	}
}

void World::refreshWorld()
{
	//更新狼
	list<Wolf>::iterator it1;
	for (it1 = wolfs.begin(); it1 != wolfs.end();)
	{
		Wolf &wolf=*it1;
		if (wolf.isdeath())
		{
			list<Wolf>::iterator it2 = it1++;
			wolfs.erase(it2);
		}
		else
		{
			wolf.move(*this);
			wolf.breed(*this);
			wolf.getOlder(*this);
			it1++;
		}
	}
	//更新羊
	list<Sheep>::iterator it3;
	for (it3 = sheeps.begin(); it3 != sheeps.end();)
	{
		Sheep& sheep = *it3;
		if (sheep.isdeath())
		{
			list<Sheep>::iterator it4 = it3++;
			sheeps.erase(it4);
		}
		else
		{
			sheep.move(*this);
			//cout << "Sheep moved!" << endl;
			sheep.breed(*this);
			//cout << "Sheep breeded!" << endl;
			sheep.getOlder();
			//cout << "Sheep get older!" << endl;
			it3++;
		}
	}
}

void World::showWorld()
{
	std::cout << sheeps.size() << std::endl;
	list<Sheep>::iterator it3;
	for (it3 = sheeps.begin(); it3 != sheeps.end();it3++)
	{
		Sheep& sheep = *it3;
		std::cout << sheep.getLocation()[0] <<","<< sheep.getLocation()[1] << std::endl;
		std::cout << sheep.isdeath()<<std::endl;
	}
	std::cout << std::endl;
	list<Wolf>::iterator it4;
	for (it4 = wolfs.begin(); it4 != wolfs.end(); it4++)
	{
		Wolf& wolf = *it4;
		std::cout << wolf.getLocation()[0] << "," << wolf.getLocation()[1] << std::endl;
	}
	std::cout << wolfs.size() << std::endl;
}

long World::getSheeps() { return sheeps.size(); }
long World::getWolfs() { return wolfs.size(); }
long World::getMapsize() { return mapmodel[0].size(); }
Organism* World::getMappoint(long i, long j) { return mapmodel[i][j]; }

void World::saveWorld(const char* fileName)
{
	ofstream outFile;

	long size = getMapsize();
	long numSheeps = getSheeps();
	long numWolfs = getWolfs();
	//先大小，再成员类型
	outFile.open(fileName,ios::binary);
	outFile.write((char*)&size, sizeof(long));
	outFile.write((char*)&numSheeps, sizeof(long));
	outFile.write((char*)&numWolfs, sizeof(long));

	list<Sheep>::iterator it1;
	for (it1 = sheeps.begin(); it1 != sheeps.end(); it1++)
	{
		Sheep& sheep = *it1;
		outFile.write((char*)&sheep, sizeof(Sheep));
	}
	list<Wolf>::iterator it2;
	for (it2 = wolfs.begin(); it2 != wolfs.end(); it2++)
	{
		Wolf& wolf = *it2;
		outFile.write((char*)&wolf, sizeof(Wolf));
	}
	outFile.close();
}


void World::loadWorld(const char* fileName)
{
	ifstream inFile;
	long size;
	long numSheeps;
	long numWolfs;

	inFile.open(fileName, ios::in | ios::binary);
	inFile.read((char*)&size, sizeof(size));
	inFile.read((char*)&numSheeps, sizeof(numSheeps));
	inFile.read((char*)&numWolfs, sizeof(numWolfs));

	mapmodel.assign(size, vector<Organism*>(size, NULL));

	for (int i = 0; i < numSheeps; i++)
	{
		Sheep sheep;
		inFile.read((char*)&sheep, sizeof(sheep));
		sheeps.push_back(sheep);
		mapmodel[sheep.getLocation()[0]][sheep.getLocation()[1]] = &sheeps.back();
	}
	for (int i = 0; i < numWolfs; i++)
	{
		Wolf wolf;
		inFile.read((char*)&wolf, sizeof(wolf));
		wolfs.push_back(wolf);
		mapmodel[wolf.getLocation()[0]][wolf.getLocation()[1]] = &wolfs.back();
	}
	//设置树作为限制
	Tree* tmpTree = new Tree;
	trees.push_back(*tmpTree);
	for (int i = 0; i < size; i++)
	{
		mapmodel[size - 1][i] = tmpTree;
		mapmodel[i][size - 1] = tmpTree;
		mapmodel[0][i] = tmpTree;
		mapmodel[i][0] = tmpTree;
	}
}