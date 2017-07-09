#include "Server_PCH.h"
#include "GameConfig.h"
#include "NetMsgHandler.h"
#include "PlayerMgr.h"
#include "GameCommand.h"
#include "GameDB.h"
#include "MsgDefine.h"
#include "Player.h"
#include "GlobalVar.h"
#include "ResourceMgr.h"
#include "GameConfig.h"
#include "DBConnection.h"

/*
 *  Created by hzd 2014-11-28
 *
 */

NetServer*			gNetServer;
GameCommand			gGameCommand;
db::DBConnection*   gDbConn = NULL;	// 

bool gIsRun		= true;
bool gIsExit	= true;

char SQL_BUFFER[ MAX_SQL_BUFFER ];

bool DBServerInit(const GameConfigInfo& rGameCfgInfo);
void RunServerExit();
void Init();
void GameUpdate( uint32 nDelay );
void ServerStart(); 

int main(int argc, const char * argv[])
{
	Init();

	ServerStart();

	RunServerExit();

	return 0;

}

void RunServerExit()
{
	uint32 nSize = 0;
	while( gIsExit)
	{
		FLOG4( "db request %d." , nSize );
		if ( !nSize )
			gIsExit = false;
	}
}


bool DBServerInit(const GameConfigInfo& rGameCfgInfo)
{
	if (gDbConn == NULL)
	{
		gDbConn = new db::DBConnection();
		std::string host(rGameCfgInfo.arrDbdateHost);
		std::string username(rGameCfgInfo.arrDbDataUsername);
		std::string password(rGameCfgInfo.arrDbDataPassword);
		std::string dbname(rGameCfgInfo.arrDbDataDbname);		
		if (!gDbConn->connect(host, username, password, dbname, 3306))
		{
			return false;
		}

		// 初始化其它数据库的数据 
	}

	return true;
}

void Init()
{
	InitNetMsg();

	g_rGameCfg.LoadRes( "server_cfg.xml" );

	const GameConfigInfo& rGameCfgInfo = g_rGameCfg.GetGameConfigInfo();

	g_rResourceMgr.LoadAllRes(rGameCfgInfo.arrConfigPath);

	if(DBServerInit(rGameCfgInfo))
	{
	
		gNetServer = NetServerMgr::Instance()->AddNetServer();
		gNetServer->SetAddress( rGameCfgInfo.arrServerHost , rGameCfgInfo.nServerPort);
		gNetServer->SetHandler( OnNetMsgEnter , OnNetMsg , OnNetMsgExit );
	
		g_rPlayerMgr.Init(*gNetServer);

		gNetServer->Start();

		FLOG4( "SERVER STARTED." );

		gGameCommand.Init();
	}
}

/*
 *  Created by hzd 2015/01/26  
 *
 */
void GameUpdate( uint32 nDelay )
{
	gNetServer->Update(nDelay);
	g_rPlayerMgr.Update(nDelay);
}

/*
 *   
 *  Created by hzd 2015/01/26  
 *
 */
void ServerStart()
{	

	timer t;
	srand( (uint32)t.elapsed() );
	uint32 nNowTime = 0,nLastTime = 0;
	int32 nCount = 0 , nRunStatus = 0;
	while ( gIsRun )
	{
		if ( nCount > 1000 )
		{
			SSleep(1);
			nCount = 0;
		}
		nCount++; 
		nRunStatus++;
		nNowTime = (uint32)t.elapsed();
		GameUpdate( nNowTime - nLastTime );
		nLastTime = nNowTime;
		if( nRunStatus > 2000000 )
		{ 
			FLOG4("Main thread is running...");
			nRunStatus = 0;
		}
	}
}



