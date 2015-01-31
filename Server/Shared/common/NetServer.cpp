//
//  server.cpp
//  test
//
//  Created by fox on 12-11-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "NetServer.h"

namespace SWA
{

	NetServer::NetServer()
		:   m_cAcceptor( *this )
	{
		static int32 s_nIncreaseNetServerID = 0;
		m_nServerID = ++s_nIncreaseNetServerID;
		for ( int i = 0 ; i < MAX_SOCKET_CONNECT ; i++ ) 
		{
			m_arrSocket[ i ] = new NetSocket( *this );
		}
	}


	NetServer::~NetServer()
	{
	}

	int32 NetServer::ID()
	{
		return m_nServerID;
	}


	void NetServer::SetAccept( NetSocket& rSocket )
	{
		m_cAcceptor.async_accept( rSocket ,
			boost::bind( &NetServer::HandleAccept , this , boost::asio::placeholders::error ,
			&rSocket ) );
	}


	void NetServer::SetAddress( const char* pIp , uint16 nPort )
	{
		boost::system::error_code ec;
		m_cServerAddr = tcp::endpoint( address::from_string( pIp , ec ) , nPort );
		assert(!ec);
	}

	void NetServer::SetHandler( PNetServerHandler pEnter  ,PNetServerMsgHandler pOnMsg , PNetServerHandler pExit )
	{
		m_pOnEnter = pEnter;
		m_pOnExit = pExit;
		m_pOnMsg = pOnMsg;
	}

	void NetServer::Run()
	{
		while ( true )
		{
			boost::system::error_code ec;
			try
			{
				run( ec );
				break;
			}
			catch ( std::exception& e )
			{
			}
		}
	}


	void NetServer::Start()
	{
		// Start socket use a new thread , that can loop
		thread t( boost::bind( &NetServer::HandleStart , this ) );
		this_thread::yield();		// Temporarily give up a CPU time , to other threads
		t.swap( m_cServiceThread );	// Swaps the state of the object with that of mServiceThread
	}


	void NetServer::HandleStart()
	{
		boost::system::error_code ec;
		m_cAcceptor.open( m_cServerAddr.protocol(), ec );
		assert( !ec );
		m_cAcceptor.set_option(tcp::acceptor::reuse_address(true) , ec );
		assert( !ec );
		m_cAcceptor.bind( m_cServerAddr , ec ); 
		assert( !ec );
		m_cAcceptor.listen( socket_base::max_connections , ec ); 
		assert( !ec );
		for ( int i = 0 ; i < MAX_SOCKET_CONNECT ; ++i ) 
		{
			SetAccept( *m_arrSocket[ i ] );
		}
		thread_group tg;
		for ( int i = 0; i < MAX_THREAD ; ++i )
		{
			tg.create_thread(boost::bind( &NetServer::Run , this ) );
		}
		this_thread::yield();
		tg.join_all();
	}

	void NetServer::Stop()
	{

	}

	void NetServer::Update(uint32 nDelay)
	{
		if ( int32 nSize = (int32)m_vecAcceptSocket.size() ) 
		{
			mutex::scoped_lock cLock( m_cClientListMutex );
			nSize = m_vecAcceptSocket.size();
			for ( int32 i = 0 ; i < nSize ; ++i )
			{
				SocketVectorIter iter = std::find( m_vecUsedSocket.begin() , m_vecUsedSocket.end() , m_vecAcceptSocket[i] );
				if ( iter != m_vecUsedSocket.end() ) 
				{
					(*m_pOnExit)( **iter );
					m_vecUsedSocket.erase( iter );
				}
				(*m_pOnEnter)( *m_vecAcceptSocket[i] );
				m_vecUsedSocket.push_back( m_vecAcceptSocket[i] );
			}
			m_vecAcceptSocket.clear();
			cLock.unlock();
		}

		if(int32 nSize = m_vecUsedSocket.size())
		{
			static NetMsgHead* pHead;
			for ( int32 nIndex = 0 ; nIndex < nSize ; ++nIndex ) 
			{
				if(NetSocket* pNetSocket = m_vecUsedSocket[ nIndex ])
				{
					switch ( pNetSocket->ReadMsg( &pHead ) )
					{
					case MSG_READ_INVALID:
						{
							(*m_pOnExit)( *pNetSocket );
							m_vecUsedSocket.erase( m_vecUsedSocket.begin() + nIndex );
							pNetSocket->Clear();
							SetAccept(*pNetSocket); 
							nIndex--;
							nSize--;
							FLOG4("OnNetMsgExit %d" , m_vecUsedSocket.size() );
						}
						break;
					case MSG_READ_OK:
						{
							(*m_pOnMsg)( *pNetSocket , pHead );
							pNetSocket->RemoveMsg( pHead->nSize );
						}
						break;
					case MSG_READ_WAITTING:
					case MSG_READ_REVCING:
						break;
					}
				}
			}
		}
	}

	void NetServer::HandleAccept( const boost::system::error_code& rError , NetSocket* pSocket )
	{
		FLOG4("One connected......ok");
		if ( rError ) 
		{
			SetAccept( *pSocket );
		}else
		{
			mutex::scoped_lock lock( m_cClientListMutex );
			m_vecAcceptSocket.push_back( pSocket );
			pSocket->Run();
			lock.unlock();
		}
	}

	NetSocket& NetServer::GetSocket(int32 nIndex)
	{
		return *m_arrSocket[nIndex];
	}

	//----------------------NetServerMgr----------------------------

	NetServer* NetServerMgr::AddNetServer()
	{
		NetServer* pNetServer = new NetServer;
		m_vecNetServer.push_back(pNetServer);
		return pNetServer;
	}

	NetServer* NetServerMgr::GetNetServer(int32 nServerID)
	{
		vector<NetServer*>::iterator it = m_vecNetServer.begin();
		for (;it != m_vecNetServer.end(); ++it)
		{
			if((*it)->ID() == nServerID)
				return *it;
		}
		return NULL;
	}
}