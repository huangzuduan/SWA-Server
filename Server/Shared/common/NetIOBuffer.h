//
//  IButton.h
//  test
//
//  Created by fox on 12-11-19.
//  Copyright (c) 2012Äê __MyCompanyName__. All rights reserved.
//

#ifndef SWA_NETIOBUFFER_H_
#define SWA_NETIOBUFFER_H_

#include "NetConfig.h"

namespace SWA
{
    class NetIOBuffer
    {
    public:
        
		NetIOBuffer();
		~NetIOBuffer();

        void   InitBuffer( uint32 nMax );
        void   ReleaseBuffer();
    
        bool   Write( char* c , uint32 nLen );
        void   Read( void** b , uint32 nLen );
        uint32 ReadRemove( void* pMsg );
        void   RemoveBuffer( uint32 nLen );
        
        uint32 GetLen();
        uint32 GetSpace();
		void*  GetBuffer();
		void*  GetStart();
		void*  GetBufferEnd();
		void   ClearBuffer();
		void   MoveBuffer();
        
    protected:
        mutex       m_cMutex;
        uint32     m_nBegin;
        uint32     m_nLen;
        char*      m_pBuffer;
        uint32		m_nMax;
    };
    

}


#endif



