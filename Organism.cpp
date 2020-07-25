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

long t = 0;//�ⲿ����t�������������

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


int Organism::searchAround(long, long, World& w)         //��Ѱ����
{
	/*�����Ѱ*/
	uniform_int_distribution<int> u(1, 4);				// ����һ����ΧΪ1~4�����ͷֲ�����
	default_random_engine e(time(0) + (t++));					// ����һ�����������
	for (int i=0; i < 4; i++)							//�����ʼ��˳ʱ����Ѱ
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
	setLabel(1);				//��ı�ʶΪ1
	setLocation(0, 0);
	setLife(true);
}


int Sheep::move(World& w)
{
	uniform_int_distribution<int> u(1, 4);				// ����һ����ΧΪ1~4�����ͷֲ�����
	default_random_engine e(time(0) + (t++));					// ����һ�����������	for (int i=0; i < 4; i++)							//�����ʼ��˳ʱ����Ѱ
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

	return u(e);										//1~4��������
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
	setLabel(2);				//�ǵı�ʶΪ2
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
		uniform_int_distribution<int> u(1, 4);				// ����һ����ΧΪ1~4�����ͷֲ�����
		default_random_engine e(time(0) + (t++));					// ����һ�����������	for (int i=0; i < 4; i++)							//�����ʼ��˳ʱ����Ѱ
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
		return u(e);										//1~4��������
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
		w.mapmodel[getLocation()[0]][getLocation()[1]]=NULL;//������ɾ����											
	}
	else
	{
		setSavetime(getSavetime() + 1);
	}
}

int Wolf::searchSheep(long, long, World& w)         //��Ѱ����
{
	/*�����Ѱ*/
	uniform_int_distribution<int> u(1, 4);				// ����һ����ΧΪ1~4�����ͷֲ�����
	default_random_engine e(time(0) + (t++));					// ����һ�����������
	int rand = u(e);
	for (int i = 0; i < 4; i++)							//�����ʼ��˳ʱ����Ѱ
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
				starveTime = 0;													//ɾ���򣬵���������
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
				starveTime = 0;													//ɾ���򣬵���������
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
				starveTime = 0;													//ɾ���򣬵���������
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
				starveTime = 0;													//ɾ���򣬵���������
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
	setLabel(3);				//���ı�ʶΪ3
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