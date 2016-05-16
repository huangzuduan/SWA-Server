
#include "NetClient.h"
#include "NetSocket.h"

/*
*
*	Detail: no
*   
*  Created by hzd 2014-8-4
*
*/

using namespace SWA;

NetClient::NetClient():m_pGameSocket(*this)
{
	
}

NetClient::~NetClient(void)
{
}


void NetClient::Start()
{
	thread t(boost::bind(&NetClient::HandleStart,this));
	this_thread::yield();
	t.swap(m_cServiceThread);
}

void NetClient::HandleStart()
{
	// Use thread group can add code to connect more servers 
	// here can SetConnect more socket connect
	SetConnect(m_pGameSocket);
	thread_group tg;
	for (uint8 i =  0 ; i < MAX_THREAD; ++i)
	{
		tg.create_thread(boost::bind( &NetClient::Run , this ) );
	}

	this_thread::yield();
	tg.join_all(); // Waitting threads to complete

}

void NetClient::SetAddress( const char* ip , uint16 port )
{
	boost::system::error_code ec;
	m_cServerAddr = tcp::endpoint( address::from_string( ip , ec ) , port );
	assert(!ec);
}

void NetClient::SetConnect(NetSocket& rSocket)
{
	rSocket.async_connect(m_cServerAddr,boost::bind(&NetClient::HandleConnect,this,boost::asio::placeholders::error,&rSocket));
}


void NetClient::HandleConnect(const boost::system::error_code& error,NetSocket* pSocket)
{
	if(error)
	{ 
		SetConnect(*pSocket);
	}else{
		FLOG4("Connected Success.................ok");
		pSocket->Run();
		FLOG4("Socket io event start.................ok");
		(m_pOnMsgConnected)(*pSocket);
	}
}


void NetClient::SetHandler( PNetServerHandler enter ,  PNetClientMsgHandler msg , PNetServerHandler exit )
{
	m_pOnMsgConnected		= enter;
	m_pOnMsgRecevied		= msg;
	m_pOnMsgDisconnect		= exit;
}


void NetClient::Run()
{
	while (true)
	{
		boost::system::error_code ec;
		try
		{
			run(ec);
			break;
		}catch( std::exception& e )
		{
		}
	}
}

void NetClient::Update()
{
	NetMsgHead* head = NULL;
	EMsgRead eReadStatus = m_pGameSocket.ReadMsg(&head);
	switch (eReadStatus)
	{
	case MSG_READ_INVALID:
		{
			(*m_pOnMsgDisconnect)(m_pGameSocket);
			
		}
		break;
	case MSG_READ_OK:
		{
			(m_pOnMsgRecevied)(head);
			m_pGameSocket.RemoveMsg( head->nSize );
			FLOG4( "OnNetMsg %d" , head->nType );
		}
		break;
	case MSG_READ_WAITTING:
	case MSG_READ_REVCING:
		break;
	}
}


