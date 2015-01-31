#include "Server_PCH.h"
#include "GameCommand.h"

#ifdef WIN32 
#include "conio.h"
#else

int _kbhit()
{
    struct timeval tv;
    fd_set rdfs;
    
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    FD_ZERO( &rdfs );
    FD_SET ( STDIN_FILENO , &rdfs );
    
    select( STDIN_FILENO + 1 , &rdfs , NULL , NULL , &tv );
    return FD_ISSET( STDIN_FILENO , &rdfs );
}

int _getch()
{
    return getchar();
}

#endif


using namespace SWA;

VectorCommand GameCommand::s_sCommands;

GameCommand::GameCommand()
:	m_nCommandCount( 0 )
{
	
}

GameCommand::~GameCommand()
{

}

void       GameCommand::Init()
{
    s_sCommands.push_back( KeyCommand("help" , "help show commnd list" , CommandHelp ) );
	s_sCommands.push_back( KeyCommand("online" , "player info        " , CommandPlayer ) );
}

void GameCommand::Command( float32 delay )
{
	static float32 time = 0.0f;
	time += delay;

	if ( time < 0.2f )
		return;

	time = 0.0f;

	if( _kbhit() )
	{
		int32 key = _getch();
		switch( key )
		{
#ifdef WIN32
		case 13:			// Enter
#else
        case 10:			// Linux
#endif
			{
				if( m_nCommandCount == 0 )
					break;

				ParseCommand( m_arrBuffer );

				// reset command
				m_nCommandCount = 0;
				memset( m_arrBuffer , 0 , MAX_PATH1 );
			}
			break;
		case 8:				// BackSpace
			{
				if( m_nCommandCount > 0 )
				{
					printf( "\b \b" );
					m_arrBuffer[m_nCommandCount] = 0;
					m_nCommandCount--;
				}
			}
			break;
		case 27:			// ESC
			{
				// clear command
				m_nCommandCount = 0;
				memset( m_arrBuffer , 0 , MAX_PATH1 );
				printf( "   ...[Cancel]\nCommand: " );
			}
			break;
		default:
			{
				if( m_nCommandCount >= 255 )
				{
					break;
				}
				m_arrBuffer[ m_nCommandCount ] = (char)key;
				m_arrBuffer[ m_nCommandCount + 1 ] = '\0';
				m_nCommandCount++;
				printf( "%c", key );
				
			}
			break;
		}
	}
}


bool		GameCommand::ParseCommand( char* command )
{
	VectorCommandIter iterEnd = s_sCommands.end();

	for ( VectorCommandIter iter = s_sCommands.begin() ; iter != iterEnd ; ++iter )
	{
		if ( strstr( command , iter->id.c_str() ) )
		{
			iter->callBack( command + iter->id.size() );

			return true;
		}
	}

	printf( "\nUnknown command.\n" );

	return false;
}

void GameCommand::CommandHelp( char* )
{
	printf("\n");
	VectorCommandIter iterEnd = s_sCommands.end();

	for ( VectorCommandIter iter = s_sCommands.begin() ; iter != iterEnd ; ++iter )
	{
		 FLOG4( "%-16s  %-20s%-25s" , iter->id.c_str() , "-" , iter->doc.c_str() );
	}
}

void GameCommand::CommandPlayer( char* )
{
	printf("\n");

}

