#include "pch.h"
#include"Organism.h"
#include<random>
#include<ctime>
#include<list>
#include<iostream>
using std::uniform_int_distribution;
using std::default_random_engine;
using std::iterator;
using std::cout;
using std::endl;

long t = 0;//外部变量t用作随机数种子

Organism::~Organism() {}

void Organism::setSavetime(int i)
{
	saveTime = i;
}

void Organism::setLabel(int i)
{
	label = i;
}

void Organism::setLocation(long i, long j)
{
	location[0] = i;
	location[1] = j;
}

int Organism::getSavetime()const { return saveTime; }
int Organism::getLabel()const { return label; }
const long* Organism::getLocation()const
{
	return location;
}


int Organism::searchAround(long, long, World& w)         //搜寻四周
{
	/*随机搜寻*/
	uniform_int_distribution<int> u(1, 4);				// 定义一个范围为1~4的整型分布类型
	default_random_engine e(time(0) + (t++));					// 定义一个随机数引擎
	for (int i=0; i < 4; i++)							//随机开始，顺时针搜寻
	{
		switch ((u(e)+i)%4+1)
		{
		case 1:
		{
			if (w.mapmodel[getLocation()[0]][getLocation()[1] + 1] == NULL)
				return 1;
			break;
		}
		case 2:
		{
			if (w.mapmodel[getLocation()[0] + 1][getLocation()[1]] == NULL)
				return 2;
			break;
		}
		case 3: 
		{
			if (w.mapmodel[getLocation()[0]][getLocation()[1] - 1] == NULL)
				return 3;
			break;
		}
		case 4: 
		{
			if (w.mapmodel[getLocation()[0] - 1][getLocation()[1]] == NULL)
				return 4;
			break;
		};
		default:
			break;
		}
	}
	return 0;
}

void Organism::setLife(bool y)
{
	life = y;
}

bool Organism::isdeath()
{
	return !life;
}

Sheep::Sheep()
{
	setSavetime(0);
	setLabel(1);				//羊的标识为1
	setLocation(0, 0);
	setLife(true);
}


int Sheep::move(World& w)
{
	uniform_int_distribution<int> u(1, 4);				// 定义一个范围为1~4的整型分布类型
	default_random_engine e(time(0) + (t++));					// 定义一个随机数引擎	for (int i=0; i < 4; i++)							//随机开始，顺时针搜寻
	switch (u(e))
	{
	case 1:
	{
		if (w.mapmodel[getLocation()[0]][getLocation()[1] + 1] == NULL)
		{
			w.mapmodel[getLocation()[0]][getLocation()[1] + 1] = this;
			w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
			setLocation(getLocation()[0], getLocation()[1] + 1);
		}
		break;
	}
	case 2:
	{
		if (w.mapmodel[getLocation()[0] + 1][getLocation()[1]] == NULL)
		{
			w.mapmodel[getLocation()[0] + 1][getLocation()[1]] = this;
			w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
			setLocation(getLocation()[0] + 1, getLocation()[1]);
		}
		break;
	}
	case 3:
	{
		if (w.mapmodel[getLocation()[0]][getLocation()[1] - 1] == NULL)
		{
			w.mapmodel[getLocation()[0]][getLocation()[1] - 1] = this;
			w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
			setLocation(getLocation()[0], getLocation()[1] - 1);
		}
		break;
	}
	case 4:
	{
		if (w.mapmodel[getLocation()[0] - 1][getLocation()[1]] == NULL)
		{
			w.mapmodel[getLocation()[0] - 1][getLocation()[1]] = this;
			w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
			setLocation(getLocation()[0] - 1, getLocation()[1]);
		}
		break;
	};
	default:
		break;
	}

	return u(e);										//1~4上右下左
}

void Sheep::starve() {}

void Sheep::breed(World& w)
{
	int searchFlag = searchAround(getLocation()[0], getLocation()[1], w);
	if (getSavetime() > 2 && searchFlag != 0)
	{
		Sheep* newSheep = new Sheep;
		w.sheeps.push_back(*newSheep);
		delete newSheep;
		switch (searchFlag)
		{
		case 1: 
		{
			w.sheeps.back().setLocation(getLocation()[0], getLocation()[1] + 1);
			w.mapmodel[getLocation()[0]][getLocation()[1] + 1] = &w.sheeps.back();
			cout << "New sheep:" << w.sheeps.back().getLocation()[0] << ","
				<< w.sheeps.back().getLocation()[1] << endl;
			break;
		}
		case 2: 
		{
			w.sheeps.back().setLocation(getLocation()[0] + 1, getLocation()[1]);
			w.mapmodel[getLocation()[0] + 1][getLocation()[1]] = &w.sheeps.back();
			cout << "New sheep:" << w.sheeps.back().getLocation()[0] << ","
				<< w.sheeps.back().getLocation()[1] << endl;
			break;
		}
		case 3:
		{
			w.sheeps.back().setLocation(getLocation()[0], getLocation()[1] - 1);
			w.mapmodel[getLocation()[0]][getLocation()[1] - 1] = &w.sheeps.back();
			cout << "New sheep:" << w.sheeps.back().getLocation()[0] << ","
				<< w.sheeps.back().getLocation()[1] << endl;
			break;
		}
		case 4:
		{
			w.sheeps.back().setLocation(getLocation()[0] - 1, getLocation()[1]);
			w.mapmodel[getLocation()[0] - 1][getLocation()[1]] = &w.sheeps.back();
			cout << "New sheep:" << w.sheeps.back().getLocation()[0] << ","
				<< w.sheeps.back().getLocation()[1] << endl;
			break;
		}
		default:
			break;
		}
		
	}
}

void Sheep::getOlder()
{
	setSavetime(getSavetime() + 1);
}

Wolf::Wolf()
{	
	starveTime = 0;
	setSavetime(0);
	setLabel(2);				//狼的标识为2
	setLocation(0, 0);
	setLife(true);
}

int Wolf::move(World& w)
{
	int searchFlag = searchSheep(getLocation()[0], getLocation()[1], w);
	if (searchFlag != 0)
	{
		return searchFlag;
	}
	else
	{
		uniform_int_distribution<int> u(1, 4);				// 定义一个范围为1~4的整型分布类型
		default_random_engine e(time(0) + (t++));					// 定义一个随机数引擎	for (int i=0; i < 4; i++)							//随机开始，顺时针搜寻
		switch (u(e))
		{
		case 1:
		{
			if (w.mapmodel[getLocation()[0]][getLocation()[1] + 1] == NULL)
			{
				w.mapmodel[getLocation()[0]][getLocation()[1] + 1] = this;
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				setLocation(getLocation()[0], getLocation()[1] + 1);
			}
			break;
		}
		case 2:
		{
			if (w.mapmodel[getLocation()[0] + 1][getLocation()[1]] == NULL)
			{
				w.mapmodel[getLocation()[0] + 1][getLocation()[1]] = this;
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				setLocation(getLocation()[0] + 1, getLocation()[1]);
			}
			break;
		}
		case 3:
		{
			if (w.mapmodel[getLocation()[0]][getLocation()[1] - 1] == NULL)
			{
				w.mapmodel[getLocation()[0]][getLocation()[1] - 1] = this;
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				setLocation(getLocation()[0], getLocation()[1] - 1);
			}
			break;
		}
		case 4:
		{
			if (w.mapmodel[getLocation()[0] - 1][getLocation()[1]] == NULL)
			{
				w.mapmodel[getLocation()[0] - 1][getLocation()[1]] = this;
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				setLocation(getLocation()[0] - 1, getLocation()[1]);
			}
			break;
		};
		default:
			break;
		}
		return u(e);										//1~4上右下左
	}
}

void Wolf::starve() 
{
	starveTime++;
}

void Wolf::breed(World& w)
{
	int searchFlag = searchAround(getLocation()[0], getLocation()[1], w);
	if (getSavetime() > 7 && searchFlag != 0)
	{
		Wolf* newWolf = new Wolf;
		w.wolfs.push_back(*newWolf);
		delete newWolf;
		switch (searchFlag)
		{
		case 1:
		{
			w.wolfs.back().setLocation(getLocation()[0], getLocation()[1] + 1);
			w.mapmodel[getLocation()[0]][getLocation()[1] + 1] = &w.wolfs.back();
			break;
		}
		case 2:
		{
			w.wolfs.back().setLocation(getLocation()[0] + 1, getLocation()[1]);
			w.mapmodel[getLocation()[0] + 1][getLocation()[1]] = &w.wolfs.back();
			break;
		}
		case 3:
		{
			w.wolfs.back().setLocation(getLocation()[0], getLocation()[1] - 1);
			w.mapmodel[getLocation()[0]][getLocation()[1] - 1] = &w.wolfs.back();
			break;
		}
		case 4:
		{
			w.wolfs.back().setLocation(getLocation()[0] - 1, getLocation()[1]);
			w.mapmodel[getLocation()[0] - 1][getLocation()[1]] = &w.wolfs.back();
			break;
		}
		default:
			break;
		}

	}
}

void Wolf::getOlder(World& w)
{
	starve();
	if (starveTime > 3)
	{
		w.mapmodel[getLocation()[0]][getLocation()[1]]->setLife(false);
		w.mapmodel[getLocation()[0]][getLocation()[1]]=NULL;//饥饿则删除狼											
	}
	else
	{
		setSavetime(getSavetime() + 1);
	}
}

int Wolf::searchSheep(long, long, World& w)         //搜寻四周
{
	/*随机搜寻*/
	uniform_int_distribution<int> u(1, 4);				// 定义一个范围为1~4的整型分布类型
	default_random_engine e(time(0) + (t++));					// 定义一个随机数引擎
	int rand = u(e);
	for (int i = 0; i < 4; i++)							//随机开始，顺时针搜寻
	{
		switch ((rand + i) % 4+1)
		{
		case 1:
		{
			if (w.mapmodel[getLocation()[0]][getLocation()[1] + 1]!=NULL &&
				w.mapmodel[getLocation()[0]][getLocation()[1] + 1]->getLabel() == 1)
			{
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				w.mapmodel[getLocation()[0]][getLocation()[1] + 1]->setLife(false);
				starveTime = 0;													//删除羊，调整饥饿度
				w.mapmodel[getLocation()[0]][getLocation()[1] + 1] = this;
				setLocation(getLocation()[0], getLocation()[1] + 1);
				return 1;
			}
			break;
		}
		case 2:
		{
			if (w.mapmodel[getLocation()[0] + 1][getLocation()[1]]!=NULL &&
				w.mapmodel[getLocation()[0] + 1][getLocation()[1]]->getLabel() == 1)
			{
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				w.mapmodel[getLocation()[0] + 1][getLocation()[1]]->setLife(false);
				starveTime = 0;													//删除羊，调整饥饿度
				w.mapmodel[getLocation()[0] + 1][getLocation()[1]] = this;
				setLocation(getLocation()[0] + 1, getLocation()[1]);
				return 2;
			}
			break;
		}
		case 3:
		{
			if (w.mapmodel[getLocation()[0]][getLocation()[1] - 1]!=NULL &&
				w.mapmodel[getLocation()[0]][getLocation()[1] - 1]->getLabel() == 1)
			{
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				w.mapmodel[getLocation()[0]][getLocation()[1] - 1]->setLife(false);
				starveTime = 0;													//删除羊，调整饥饿度
				w.mapmodel[getLocation()[0]][getLocation()[1] - 1] = this;
				setLocation(getLocation()[0], getLocation()[1] - 1);
				return 3;
			}
			break;
		}
		case 4:
		{
			if (w.mapmodel[getLocation()[0] - 1][getLocation()[1]]!=NULL &&
				w.mapmodel[getLocation()[0] - 1][getLocation()[1]]->getLabel() == 1)
			{
				w.mapmodel[getLocation()[0]][getLocation()[1]] = NULL;
				w.mapmodel[getLocation()[0] - 1][getLocation()[1]]->setLife(false);
				starveTime = 0;													//删除羊，调整饥饿度
				w.mapmodel[getLocation()[0] - 1][getLocation()[1]] = this;
				setLocation(getLocation()[0] - 1, getLocation()[1]);
				return 4;
			}
			break;
		};
		default:
			break;
		}
	}
	return 0;
}

Tree::Tree()
{
	setSavetime(0);
	setLabel(3);				//树的标识为3
	setLocation(0, 0);
	setLife(true);
}

int Tree::move(World& w)
{
	return 0;
}

void Tree::breed(World& w)
{

}

void Tree::starve()
{

}