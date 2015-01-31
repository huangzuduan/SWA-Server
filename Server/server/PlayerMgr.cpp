#include "Server_PCH.h"
#include "PlayerMgr.h"
#include "DBLoginHandler.h"


PlayerMgr::PlayerMgr()
{
}

PlayerMgr::~PlayerMgr()
{

}


void PlayerMgr::Init(NetServer& rNetServer)
{
	for ( int32 i = 0 ; i < MAX_SOCKET_CONNECT ; i++ )
	{
		SWA::NetSocket& pSocket = rNetServer.GetSocket(i);
		ASSERT(pSocket.SID() == i);
		m_arrPlayerCache[i] = new Player(pSocket);
	}
}


void PlayerMgr::Release()
{
	for ( int32 i = 0 ; i < MAX_SOCKET_CONNECT ; i++ )
	{
		delete m_arrPlayerCache[i];
	}

	delete mThis;
	mThis = NULL;
}


Player*	PlayerMgr::GetPlayer( uint64 nUid )
{
	UIDPlayerMapIter iter = m_mapUIDPlayer.find(nUid);
	if ( iter == m_mapUIDPlayer.end() )
	{
		return NULL;
	}
	return iter->second;
}


Player&	PlayerMgr::GetPlayer( SWA::NetSocket& rSocket )
{
	return *m_arrPlayerCache[rSocket.SID()];
}



void PlayerMgr::SendToAll( NetMsgHead* pMsg )
{
	UIDPlayerMapIter iter = m_mapUIDPlayer.begin();
	UIDPlayerMapIter iterend = m_mapUIDPlayer.end();

	for ( ; iter != iterend ; ++iter )
	{
		iter->second->SendMsg(pMsg);
	}
}

void PlayerMgr::Update(uint32 nDelay)
{

	UIDPlayerMapIter it = m_mapUIDPlayer.begin();
	UIDPlayerMapIter itEnd = m_mapUIDPlayer.end();
	Player* pPlayer = NULL;
	for (; it != itEnd; ++it)
	{
		pPlayer = it->second;
		pPlayer->Update(nDelay);
	}

}

















