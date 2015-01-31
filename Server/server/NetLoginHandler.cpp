#include "Server_PCH.h"
#include "NetLoginHandler.h"
#include "NetMsgHandler.h"
#include "DBLoginHandler.h"

#include "Player.h"
#include "PlayerMgr.h"

#include "GlobalVar.h"
#include "GameDB.h"

void NetRequestClose( Player& rPlayer , NetMsgHead* pHead )
{
	rPlayer.Exist();
}

void NetLoginEncryptInfoHandler( Player& rPlayer , NetMsgHead* pHead )
{
	if (EPLAYER_STATUS_CONNECTED == rPlayer.GetStatus())
	{
		char arrRandKey[MAX_ENCRYPT_LENTH];
		Encrypt::RandKey(arrRandKey);

		{
			S2CEncryptInfo sMsg;
			memcpy(sMsg.arrEncryptKey, arrRandKey, MAX_ENCRYPT_LENTH);
			rPlayer.SendMsg(&sMsg);
		}

		// 要先发数据后才能改
		rPlayer.SetEncryptKey(arrRandKey);
		rPlayer.SetStatus(EPLAYER_STATUS_ENCRYPTED);
		
	}
}


void NetLoginAccountHandler( Player& rPlayer , NetMsgHead* pHead )
{

	if (EPLAYER_STATUS_ENCRYPTED == rPlayer.GetStatus())
	{
		const C2SLogin& rPacket = *static_cast<const C2SLogin*>(pHead);
		if(IDataBase* pDB = g_rDbDatabaseMgr.GetMainDB())
		{
			SPRINTF(SQL_BUFFER,"SELECT `id`,`username` FROM `swa_data`.`account` WHERE  `username`='%s' AND `password`='%s';", rPacket.arrUsername,rPacket.arrPassword);
			SQL_BUFFER[ MAX_SQL_BUFFER - 1 ] = '\0';
			pDB->ExecuteAsyncSQL(SQL_BUFFER,&rPlayer,DBLoginRepAccount);
		}
	}

}

void NetLoginRoleListHandler( Player& rPlayer , NetMsgHead* pHead )
{

}

/*
* 选择角色进入游戏
*/
void NetLoginSelectRoleHandler( Player& rPlayer , NetMsgHead* pHead )
{
	
	
}

void NetLoginRegisterRoleHandler( Player& rPlayer , NetMsgHead* pHead )
{
	

}

void NetLoginResourcesLoaded( Player& rPlayer , NetMsgHead* pHead )
{

	
}


