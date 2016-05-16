#ifndef _GAMEDB_H_
#define _GAMEDB_H_

#include "DbDataBase.h"

/*
 *
 *	Detail: 大部分数据，直接与mysql打交道，player里的数据，只是缓存，不用每次从mysql中读取，或player中只是缓存不太重要的数据
 *  断开连接也会保存部分需要的数据
 *   
 *  Created by hzd 2014-10-7
 *
 */


const int32 MAX_SQL_BUFFER = 512;
extern char SQL_BUFFER[ MAX_SQL_BUFFER ];

#pragma pack(push,1)

struct DBAccountLogin
{
	uint32 nID;
	char  arrUsername[32];
};


#pragma pack(pop)


#endif

