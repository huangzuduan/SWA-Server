#ifndef _CREATUREDATA_H_
#define _CREATUREDATA_H_


#include "gameDefine.h"
#include "msgDefine.h"


typedef list< fint32 > SyncCreatureList;
typedef list< fint32 >::iterator SyncCreatureListIter;

struct CreatureMoveData
{
    MapPos					Pos;
    SACreatureMoveDirection	Direction;
    
    freal32					Time;
    freal32					MoveSpeed;
    fint32					MovePathPosCount;
    fint32					MovePathPos;
    MapGridID				MovePath[ MAX_MOVE_PATH ];

    fbool					IsDirectionBuffer;
    SACreatureMoveDirection	DirectionBuffer;

    fint32					Enemy;
    
    fbool                   NeedSync;
    freal32                 SyncTime;
    
    SyncCreatureList        SyncList;
    SyncCreatureList        SyncEnter;
    SyncCreatureList        SyncExit;
    
    fint32                  GUID;
    
    CreatureMoveData()
    :	MovePathPos( 0 ) ,
        MovePathPosCount( INVALID_ID ) ,
        Time( 0.0f ) ,
        MoveSpeed( DEFAULT_MOVE_SPEED ) ,
        Direction( SACMD_SOUTH ) ,
        IsDirectionBuffer( F_FALSE ) , DirectionBuffer( SACMD_COUNT ) ,
        Enemy( 0 ) ,
        NeedSync( F_FALSE )
    {
        
    }

    fvoid				EndMove()
    {
        //logManager::Print( "x%d , y%d" , Pos.Grid.x , Pos.Grid.y );
        Enemy = 0;

        MovePathPosCount = INVALID_ID;

        Time = 0.0f;
        MovePathPos = 0;
    }

    fvoid				Clear()
    {
        Enemy = 0;

        Pos.MapID = INVALID_ID;
        Pos.Grid = MapGridID();

        Time = 0.0f;
        MoveSpeed = DEFAULT_MOVE_SPEED;
        MovePathPosCount = INVALID_ID;
        MovePathPos = 0;
        
        GUID = INVALID_ID;
        
        SyncList.clear();
        SyncEnter.clear();
        SyncExit.clear();
    }

    fvoid				AddTime( freal32 delta );

    fvoid				SetGrid( const MapGridID& grid )
    {
        Pos.Grid = grid;
    }

    fvoid               UpdateSync();
    
    fint32				Move();

    fvoid				Set( MapGridID* data , fint32 maxPos );

    fint32				IsPathIn( const MapGridID& pos );

};



#endif

