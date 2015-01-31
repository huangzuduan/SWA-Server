#include "LogManager.h"
#include <boost/thread/mutex.hpp>


boost::mutex gMutex;

void Logfox( int32 nLevel , const char* str , ... )
{
    boost::mutex::scoped_lock lock( gMutex );
    if ( nLevel < LOGLEVEL0 )
        return;
    
    va_list va;
    static char sstr[255];
    static char sbuf[255];
    memset( sstr , 0 , 255 );
    memset( sbuf , 0 , 255 );
    
    va_start( va , str );
    vsprintf( sstr , str , va );
    va_end(va);
    
    printf( "log:level %d %s \n" , nLevel , sstr );
    lock.unlock();
}



