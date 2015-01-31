#ifndef SWA_BASE_DEFINE_H_
#define SWA_BASE_DEFINE_H_

#include <stdio.h>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <utility>

#ifdef WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <mswsock.h>
#include <Windows.h>
#else
#include <stdint.h>
#endif

using namespace std;
	
#ifdef WIN32
	typedef __int64                     int64;
#else
	typedef int64_t                     int64;
#endif
	typedef unsigned long long          uint64;
	typedef wchar_t						wchar;
	typedef unsigned char				uchar;
	typedef char						int8;
	typedef unsigned char				uint8;
	typedef short						int16;
	typedef unsigned short				uint16;
	typedef int							int32;
	typedef unsigned int				uint32;
	typedef unsigned int				ulong;
	typedef float						float32;
	typedef double						float64;


static const char* LTOA( long nL )
{
	static char buffer[ 64 ];
	sprintf( buffer , "%ld" , nL );
	return buffer;
}

static const char* ITOA( int32 nI )
{
	static char buffer[ 32 ];
	sprintf( buffer , "%d" , nI );
	return buffer;
}

static const char* FTOA( float32 nF )
{
	static char buffer[ 32 ];
	sprintf( buffer , "%f" , nF );
	return buffer;
}
	
#define S_SAFE_DELETE(a) if( a ){ delete a;a=NULL; }
#define S_SAFE_RELEASE(a) if( a ){ a->Release(); }
	
#define S_USE_PRINT

#define STRCMP strcmp
#define STRCPY strcpy
#define ATOF atof
#define ATOI atoi

#ifdef WIN32
#define SSleep(ms) Sleep(ms)
#else
#define SSleep(ms) usleep(ms)
#endif


/*
 *
 *	Detail: 
   %% 印出百分比符号，不转换。   
   %c 整数转成对应的 ASCII 字元。    
   %d 整数转成十进位。  
   %f 倍精确度数字转成浮点数。    
   %o 整数转成八进位。    
   %s 整数转成字符串。  
   %x 整数转成小写十六进位。    
   %X 整数转成大写十六进位。
 *   
 *  Created by hzd 2014-11-2
 *
 */
#define SPRINTF sprintf

#define RAND rand

#ifdef ASSERT
#undef ASSERT
#endif 
#define ASSERT assert

#define INSERT_FLAG(value, query_flags)	{(value) |= (query_flags);}
#define REMOVE_FLAG(value, query_flags)	{(value) &= ~(query_flags);}
#define HAS_FLAG(value, query_flags)		( ((value) & (query_flags)) != 0 ? true : false )

#include <boost/timer.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/container/set.hpp>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/std/utility.hpp>

#endif







