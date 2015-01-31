//
//  LogManager.h
//  test
//
//  Created by fox on 13-1-9.
//
//

#ifndef SWA_LOG_MANAGER_H_
#define SWA_LOG_MANAGER_H_

#include "BaseDefine.h"

enum logLevel
{
	LOGLEVEL0 = 0,
	LOGLEVEL1,
	LOGLEVEL2,
	LOGLEVEL3,
	LOGLEVEL4
};

void Logfox( int32 level , const char* str , ... );

#ifdef S_USE_PRINT
#ifdef WIN32
#define FLOG( nLevel , fmt , arg ,... ) Logfox( nLevel , fmt , ##arg __VA_ARGS__ );
#define FLOG0( fmt , arg ,... ) Logfox( LOGLEVEL0 , fmt , ##arg __VA_ARGS__ );
#define FLOG1( fmt , arg ,... ) Logfox( LOGLEVEL1 , fmt , ##arg __VA_ARGS__ );
#define FLOG2( fmt , arg ,... ) Logfox( LOGLEVEL2 , fmt , ##arg __VA_ARGS__ );
#define FLOG3( fmt , arg ,... ) Logfox( LOGLEVEL3 , fmt , ##arg __VA_ARGS__ );
#define FLOG4( fmt , ... ) Logfox( LOGLEVEL4 , fmt , __VA_ARGS__ );

#else
#define FLOG( nLevel , fmt , a... ) Logfox( nLevel , fmt , ##a );

#define FLOG0( fmt , a... ) Logfox( LOGLEVEL0 , fmt , ##a );
#define FLOG1( fmt , a... ) Logfox( LOGLEVEL1 , fmt , ##a );
#define FLOG2( fmt , a... ) Logfox( LOGLEVEL2 , fmt , ##a );
#define FLOG3( fmt , a... ) Logfox( LOGLEVEL3 , fmt , ##a );
#define FLOG4( fmt , a... ) Logfox( LOGLEVEL4 , fmt , ##a );
#endif

#else
#ifdef WIN32
#define FLOG( nLevel , fmt , arg ,... ) logFile( nLevel , fmt  "\n%s %s() line:%d" , ##arg __VA_ARGS__ , __FILE__ , __FUNCTION__ , __LINE__ );
#else
#define FLOG( nLevel , fmt , a... ) logFile( nLevel , fmt  "\n%s %s() line:%d" , ##a , __FILE__ , __FUNCTION__ , __LINE__ );
#endif

#endif



#endif
