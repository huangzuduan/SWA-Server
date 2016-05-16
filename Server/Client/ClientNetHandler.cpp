#include "Client_PCH.h"
#include "ClientNetHandler.h"
#include "NetMsgHandler.h"
#include "ClientPlayer.h"

using namespace SWA;

extern	ClientPlayer* gPlayer;


ClientNetHandler::ClientNetHandler(void)
{
}


ClientNetHandler::~ClientNetHandler(void)
{
}

void ClientNetHandler::SendRequestClose()
{

}

void ClientNetHandler::SendEncrypt()
{
	C2SEncryptInfo sMsg;
	gPlayer->PackMsg(&sMsg);
	gPlayer->SendMsg();
}

void ClientNetHandler::RecvEncryptInfo(NetMsgHead* pHead)
{
	const S2CEncryptInfo& rPacket = *static_cast<const S2CEncryptInfo*>(pHead);
	gPlayer->SetEncryptKey(rPacket.arrEncryptKey);
	SendLogin();
}

void ClientNetHandler::SendLogin()
{
	// 使用默认账号与密码
	C2SLogin sMsg;
	strcpy(sMsg.arrUsername,"hzd");
	strcpy(sMsg.arrPassword,"hzd123");
	gPlayer->PackMsg(&sMsg);
	gPlayer->SendMsg();
}

void ClientNetHandler::SendCharacterList()
{

}

void ClientNetHandler::SendChar2World()
{

}
void ClientNetHandler::SendRegister()
{

}

void ClientNetHandler::SendLoadResed()
{

}


////////////////////////////接收//////////////////////////////////////////

void ClientNetHandler::RevcCharacterList(NetMsgHead* head)
{
	
}


void ClientNetHandler::RevcLoginResult(NetMsgHead* head)
{

}

void ClientNetHandler::RecvSelectResult(NetMsgHead* head)
{

}

void ClientNetHandler::RecvRegisterResult(NetMsgHead* head)
{

}

void ClientNetHandler::RecvCharacterInfo(NetMsgHead* head)
{

}

void ClientNetHandler::RecvChatInfo(NetMsgHead* head)
{


}

