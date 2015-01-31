#include "Server_PCH.h"
#include "NetMsgHandler.h"
#include "NetChatHandler.h"
#include "NetLoginHandler.h"

#include "DBLoginHandler.h"
#include "PlayerMgr.h"
#include "GlobalVar.h"

using namespace SWA;

static NetMsgHandlerMap	 gMsgHeandlerMap;

bool CheckMsg(const NetMsgHead* pMsg , uint32 nSize )
{
	if ( pMsg->nSize == nSize )
		return true;
	else
		return false;
}


void InitNetMsg()
{

	FLOG4("PROTOCOL INIT...");

	static NetMsgHandler s_arrHandler[] =
	{
		
		// 协议=>处理函数

		{PRO_C2S_CLOSE,NetRequestClose},

		// 登录类协议
		{ PRO_C2S_ENCRYPT		,	NetLoginEncryptInfoHandler		},// 获得密钥
		{ PRO_C2S_LOGIN_ACCOUNT	,	NetLoginAccountHandler		},	//登录 
		{ PRO_C2S_LOGIN_ROLE_LIST ,	NetLoginRoleListHandler			},	// 好友列表
		{ PRO_C2S_CREATE_ROLE		,	NetLoginRegisterRoleHandler		},	// 创建角色
		{ PRO_C2S_SELECT_ROLE		,	NetLoginSelectRoleHandler		},	// 选择角色
		{ PRO_C2S_ENCRYPT			,	NetLoginEncryptInfoHandler		},

		{PRO_C2S_CLIENT_RES_LOADED,		NetLoginResourcesLoaded 		},  // 资源加载完成

		// 角色类协议

		{ PRO_C2S_CHAT_TO_WORLD,      NetChatWorldMsgHandler		},
			// end
		{ PRO_C2S_NULL			  ,		NULL			}, // no

	};

	int32 nIndex = 0;
	while ( s_arrHandler[nIndex].nType )
	{
		gMsgHeandlerMap[ s_arrHandler[ nIndex ].nType ] = s_arrHandler[ nIndex ];
		nIndex++;
	}
}


void OnNetMsgEnter( NetSocket& rSocket )
{
	Player& rPlayer = g_rPlayerMgr.GetPlayer( rSocket );
	rPlayer.SetStatus(EPLAYER_STATUS_CONNECTED);

	// 标志状态 todo

	FLOG4( "player load %s" , rSocket.GetIp().c_str() );

}


void OnNetMsg( NetSocket& rSocket , NetMsgHead* pHead )
{
	Player& rPlayer = g_rPlayerMgr.GetPlayer(rSocket );

	// 解密处理
	static char msgBuffer[MAX_SOCKET_BUFFER];
	memcpy(&msgBuffer[0], pHead, pHead->nSize);
	static char msgEncrypt[MAX_ENCRYPT_LENTH];
	rPlayer.GetEncryptKey(msgEncrypt);
	Encrypt::XorCode(pHead->nSize,msgEncrypt,&msgBuffer[sizeof(pHead->nSize)],pHead->nSize);
	pHead = ( NetMsgHead*)&msgBuffer[0];
	NetMsgHandlerMapIter iter = gMsgHeandlerMap.find( pHead->nType );
	if ( iter == gMsgHeandlerMap.end() )
	{
		FLOG4("Not find protocol:%d",pHead->nType);
		rSocket.Disconnect();
		return;
	}
	else
	{
		(iter->second.pFun)( rPlayer , pHead );
		FLOG4("OnNetMsg %d", pHead->nType);
	}
}

void OnNetMsgExit( NetSocket& rSocket )
{
	Player& rPlayer = g_rPlayerMgr.GetPlayer( rSocket );
	rPlayer.SetStatus(EPLAYER_STATUS_INITED);
}


