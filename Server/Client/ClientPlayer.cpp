#include "Client_PCH.h"
#include "ClientPlayer.h"

ClientPlayer::ClientPlayer(void)
{
	memcpy(mEncryptKey,EncryptDefualt,MAX_ENCRYPT_LENTH);

}


ClientPlayer::~ClientPlayer(void)
{
}

void ClientPlayer::SendMsg()
{
	mSocket->SendMsg();
}


void ClientPlayer::PackMsg( NetMsgHead* msg )
{
	// ¼ÓÃÜ´¦Àí
	static char msgBuffer[MAX_SOCKET_BUFFER];
	memcpy(&msgBuffer[0], msg, msg->nSize);
	Encrypt::XorCode(msg->nSize,mEncryptKey,&msgBuffer[sizeof(msg->nSize)],msg->nSize);
	msg = ( NetMsgHead*)&msgBuffer[0];
	mSocket->ParkMsg( msg );
}

void ClientPlayer::SetSocket( NetSocket& rSocket )
{
	mSocket = &rSocket;
}

NetSocket* ClientPlayer::GetSocket()
{
	return mSocket;
}
