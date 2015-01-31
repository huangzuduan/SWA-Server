#include "probability.h"
#include "mathBase.h"
#include <time.h>

using namespace SWA;

Probability::Probability(void)
{
}


Probability::~Probability(void)
{
}

/*
 * 随机抽取N个数组（结果不重复）
 * source 源数组
 * target 输出数组
 */

void Probability::getRandArray(const vector<int32>& source, vector<int32>& target, int32 tnum)
{
	target.clear();
	vector<int32> list(source);

	int32 snum = source.size();
	int32 i = 0;
	int32 rand = 0;
	srand((int32)time(NULL));
	for(;i<tnum;i++)
	{
		rand = MathBase::GetRandomInt( i , snum );
		target.push_back(list[rand]); 
		swap(list[i], list[rand]);
	}
}
/*
 * 随机抽取N个数组（结果不重复）
 * source 源数组
 * target 输出数组
 */
void Probability::getRandArray(const int32 source[], int32 snum,  int32 target[], int32 tnum)
{
//	const sint32 snum = sizeof(source) / sizeof(source[0]);
//	const sint32 tnum = (sint32)(sizeof(source) / sizeof(sint32));
	int32 *list = new int32[snum];
	memcpy(list, source,  sizeof(int32) * snum);

	int32 i = 0;
	int32 rand = 0;
	srand((int32)time(NULL));
	for(;i<tnum;i++)
	{
		rand = MathBase::GetRandomInt( i , snum );
		target[i] = list[rand];
		swap(list[i], list[rand]);
	}
	delete []list;
}

/*
 * 随机抽取N个数组（结果有重复）
 * source 源数组
 * snum 源数组大小
 * target 输出数组
 * tnum 输出数量
 */
void Probability::getRandArray2(const int32 source[], int32 snum, int32 target[], int32 tnum)
{
	int32 i = 1;
	int32 nRand = 0;
	srand(time(NULL));
	for(; i < tnum; i++)
	{
		nRand = MathBase::GetRandomInt( 0 , snum-1 );
		target[i] = source[nRand];
	}
}

//获取几分之的几率
bool Probability::selectByOdds(int32 upNum,int32 downNum)
{
	if (downNum < 1) return false;
	if (upNum < 1) return false;
	if (upNum > downNum - 1) return true;
	int32 m_rand =  MathBase::GetRandomInt( 1 , downNum );
	return (m_rand <= upNum);
}
