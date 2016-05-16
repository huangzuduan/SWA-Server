#include "Client_PCH.h" 
#include "NetMsgHandler.h"
#include "ClientNetHandler.h"

extern NetServer*	gNetServer;
static NetMsgHandlerMap	 gMsgHeandlerMap;
extern ClientPlayer* gPlayer;
extern bool bExist;

bool	CheckMsg(const NetMsgHead* msg , uint16 size )
{
    if ( msg->nSize == size )
		return true;
    else
        return false;
}


void InitNetMsg()
{

	// 收到的消息处理
    static NetMsgHandler handler[] =
    {
		// 协议=>处理函数
		// 登录类协议
		{ PRO_S2C_ENCRYPT_INFO		, ClientNetHandler::RecvEncryptInfo},
		{ PRO_S2C_CHARACTER_LIST	, ClientNetHandler::RevcCharacterList },
		{ PRO_S2C_CHARACTER_INFO	, ClientNetHandler::RecvCharacterInfo },
		{ PRO_S2C_CHAT_TO_WORLD		, ClientNetHandler::RecvChatInfo	},

		// end
		{ PRO_C2S_NULL			  ,		NULL						}, // no

    };
    
    int32 i = 0;
    while ( handler[i].nType )
    {
        gMsgHeandlerMap[ handler[ i ].nType ] = handler[ i ];
        
        i++;
    }
}


void OnNetMsgEnter( NetSocket& socket )
{
	//
	ClientNetHandler::SendEncrypt();
}


void OnNetMsg(NetMsgHead* pHead )
{
	// 解密处理
	static char msgBuffer[MAX_SOCKET_BUFFER];
	memcpy(&msgBuffer[0], pHead, pHead->nSize);
	Encrypt::XorCode(pHead->nSize,gPlayer->GetEncryptKey(),&msgBuffer[sizeof(pHead->nSize)],pHead->nSize);
	pHead = ( NetMsgHead*)&msgBuffer[0];

	NetMsgHandlerMapIter iter = gMsgHeandlerMap.find( pHead->nType );
	if ( iter == gMsgHeandlerMap.end() )
	{
		FLOG4("Not find Pro:%d", pHead->nType);
		assert(0);
	}
	else
	{
		(iter->second.pFun)(pHead );
	}
}



void OnNetMsgExit( NetSocket& socket )
{
	FLOG4("=========Had Exist===============");
	bExist = true;
}


