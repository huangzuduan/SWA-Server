#include "Server_PCH.h"
#include "NetLoginHandler.h"
#include "NetMsgHandler.h"
#include "DBLoginHandler.h"

#include "Player.h"
#include "PlayerMgr.h"

#include "GlobalVar.h"
#include "GameDB.h"
#include "../LibProtocol/msg.pb.h"
#include "MysqlProtobufHelper.h"

extern db::DBConnection* gDbConn;

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
		std::stringstream dataSql;
		dataSql << "SELECT * FROM `account` WHERE  `username`='" << rPacket.arrUsername;
		dataSql << "' AND `password`='" << rPacket.arrPassword << "';";
		::msg::AccountInfo proto;
		int ret = doQueryProto(*gDbConn, dataSql.str(), proto);
		if (ret == 0)
		{
			if (proto.id() > 0)
			{
				FLOG4("Login successful %s", proto.username().c_str());
			}
		}

		//doQueryProto	单条查询
		//doInsertProto 单条添加
		//doReplaceProto	单条更新
		//doQueryRepeatedProto 多条查询

		//int ret = doInsertProto(*gDbConn, dataSql.str(), proto);
		//if (ret == 0)
		//{
		//	// 添加成功
		//}

		//int ret = doReplaceProto(*gDbConn, dataSql.str(), proto);
		//if (ret == 0)
		//{
		//	// 更新成功成功
		//}

		//::msg::AccountQuery protoMulti;
		//int ret = doQueryRepeatedProto(*gDbConn, dataSql.str(), *protoMulti.mutable_account_list());
		//if (ret == 0)
		//{
		//	// 多条查询
		//}
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


