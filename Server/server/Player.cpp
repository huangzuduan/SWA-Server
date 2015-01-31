#include "Server_PCH.h"
#include "Player.h"


Player::Player(SWA::NetSocket& rSocket) :m_rSocket(rSocket),m_eStatus(EPLAYER_STATUS_INITED)
{
	memcpy(m_arrEncryptKey,EncryptDefualt,MAX_ENCRYPT_LENTH);
}

Player::~Player()
{
}

uint64	Player::UID()
{
	return m_nID;
}



void Player::SendMsg(NetMsgHead* pMsg)
{
	// ¼ÓÃÜ´¦Àí
	static char arrBuffer[MAX_SOCKET_BUFFER];
	memcpy(&arrBuffer[0], pMsg, pMsg->nSize);
	Encrypt::XorCode(pMsg->nSize,m_arrEncryptKey,&arrBuffer[sizeof(pMsg->nSize)],pMsg->nSize);
	pMsg = ( NetMsgHead*)&arrBuffer[0];

	m_rSocket.ParkMsg( pMsg );
	m_rSocket.SendMsg();
}


void Player::SetEncryptKey(char arrNewKey[MAX_ENCRYPT_LENTH])
{ 
	memcpy(m_arrEncryptKey,arrNewKey,MAX_ENCRYPT_LENTH); 
}

void Player::GetEncryptKey(char o_arrEncryptKey[MAX_ACCOUNT_LENG])
{
	memcpy(o_arrEncryptKey,m_arrEncryptKey,MAX_ENCRYPT_LENTH); 
}


void Player::SetStatus(EPlayerStatus eStatus)
{
	if (EPLAYER_STATUS_INITED == eStatus)
	{
		memcpy(m_arrEncryptKey, EncryptDefualt, MAX_ENCRYPT_LENTH);
	}
	m_eStatus = eStatus;
}



void Player::GetAccount(char o_arrAccount[MAX_ACCOUNT_LENG])
{
	memcpy(o_arrAccount,m_arrAccount,MAX_ACCOUNT_LENG);
}






void Player::Exist()
{
	m_rSocket.Disconnect();
}

void Player::Update(uint32 nDelay)
{

}

