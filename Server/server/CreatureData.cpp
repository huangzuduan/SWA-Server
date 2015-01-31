#include "CreatureData.h"
#include "logManager.h"
#include "CreatureManager.h"



fvoid       CreatureMoveData::AddTime( freal32 delta )
{
    Time += delta;
    SyncTime += delta;
}


fint32		CreatureMoveData::Move()
{
	while ( Time > MoveSpeed )
	{
		MapGridID oldPos = Pos.Grid;
		Pos.Grid = MovePath[ MovePathPos ];
		++MovePathPos;

		Direction = GetDirection( Pos.Grid , oldPos );
        
        switch ( Direction )
        {
            case SACMD_NORTH:
            case SACMD_SOUTH:
            case SACMD_WEST:
            case SACMD_EAST:
                MoveSpeed = DEFAULT_MOVE_SPEED;
                break;
            case SACMD_NORTHEAST:
            case SACMD_SOUTHWEST:
                MoveSpeed = DEFAULT_MOVE_SPEED1;
                break;
            case SACMD_NORTHWEST:
            case SACMD_SOUTHEAST:
                MoveSpeed = DEFAULT_MOVE_SPEED2;
                break;
            default:
                break;
        }

		//FLOG0( "move %d , %d , %d " , Pos.Grid.x , Pos.Grid.y , Direction );

		if ( MovePathPos == MovePathPosCount )
		{
			Time = 0.0f;

			if ( IsDirectionBuffer )
			{
				IsDirectionBuffer = F_FALSE;
				Direction = DirectionBuffer;
			}

			return 1;
		}

		Time -= MoveSpeed;

		return 0;
	}

	return -1;
}


fvoid       CreatureMoveData::UpdateSync()
{

    
}


fvoid		CreatureMoveData::Set( MapGridID* data , fint32 maxPos )
{
	MovePathPosCount = maxPos;
	MovePathPos = 1;
    
    NeedSync = F_TRUE;

	MEMCPY( MovePath , data , maxPos * sizeof( MapGridID ) );
    
    if ( SyncTime > DEFAULT_MOVE_SYNC_TIME )
    {
        UpdateSync();
        
        SyncTime = 0.0f;
    }
}


fint32		CreatureMoveData::IsPathIn( const MapGridID& pos )
{
	for ( fint32 i = 0 ; i < MovePathPosCount ; ++i )
	{
		if ( pos == MovePath[ i ] )
		{
			return i;
		}
	}

	return INVALID_ID;
}

