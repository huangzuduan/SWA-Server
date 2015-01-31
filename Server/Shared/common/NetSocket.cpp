#include "NetSocket.h"
#include "NetServer.h"
#include "LogManager.h"


namespace SWA
{

	NetSocket::NetSocket( io_service& rIo_service ):tcp::socket ( rIo_service ) ,m_cHeadBuffer( buffer( m_arrRecvBuffer , sizeof(NetMsgHead) ) ),
		m_cBodyBuffer( buffer( m_arrRecvBuffer + sizeof(NetMsgHead) , MAX_SOCKET_BUFFER - sizeof(NetMsgHead) ) ) ,m_cSendBuffer1( buffer( m_arrSendBuffer , MAX_SOCKET_BUFFER ) ),
		m_nBodyLen( 0 ) , m_eRecvStage( REVC_FSRS_NULL ) ,m_bSending( false ) ,m_bInvaild( false ) ,m_cTimer( rIo_service )
	{
		static int32 nSocketIncreseID = 0;
		m_nID = nSocketIncreseID++;
		m_cIBuffer.InitBuffer( MAX_IBUFFER_CACHE );
		m_cOBuffer.InitBuffer( MAX_OBUFFER_CACHE );
	}

	NetSocket::~NetSocket()
	{

	}

	void NetSocket::Disconnect() 
	{ 
		Close();
	}

	void NetSocket::Close()
	{
		m_bInvaild = true;
		boost::system::error_code cEc1;
		tcp::socket::close(cEc1);
	}

	EMsgRead NetSocket::ReadMsg( NetMsgHead** pHead )
	{    
		if(m_bInvaild)
			return MSG_READ_INVALID;
	
		if ( int32 nLen = m_cIBuffer.GetLen() ) 
		{
			*pHead = (NetMsgHead*)m_cIBuffer.GetStart();
			if ( (*pHead)->nSize > nLen )
			{
				return MSG_READ_REVCING;
			}
			return MSG_READ_OK;
		}
		return MSG_READ_WAITTING;
	}

	void NetSocket::RemoveMsg( uint32 nLen )
	{
		m_cIBuffer.RemoveBuffer( nLen );
	}

	void NetSocket::RecvMsg( const boost::system::error_code& ec, size_t bytes_transferred )
	{
		m_cTimer.cancel();
		if ( ec )
		{
			Close();
			return;
		}

		switch ( m_eRecvStage )
		{
		case REVC_FSRS_HEAD:
			{
				NetMsgHead* head = (NetMsgHead*)m_arrRecvBuffer;
				m_nBodyLen = head->nSize;

				if ( m_nBodyLen == 0 )
				{
					Close();
					return;
				}

				if ( m_nBodyLen == sizeof( NetMsgHead ) )
				{
					m_cIBuffer.Write( m_arrRecvBuffer , sizeof( NetMsgHead) );
					ReadHead();
					return;
				}
				else
				{
					m_eRecvStage = REVC_FSRS_BODY;
					ReadBody();
					return;
				}
			}
			break;
		case REVC_FSRS_BODY:
			{
				m_cIBuffer.Write( m_arrRecvBuffer , m_nBodyLen );
				m_eRecvStage = REVC_FSRS_HEAD;
				ReadHead();
				return;
			}
			break;
		default:
			{
				assert( 0 );
			}
			break;
		}
	}

	void NetSocket::Clear()
	{
		m_bSending		= false;
		m_bInvaild		= false;
		m_nBodyLen		= 0;
		m_eRecvStage	= REVC_FSRS_NULL;
		m_cIBuffer.ClearBuffer();
		m_cOBuffer.ClearBuffer();
	}

	void NetSocket::ParkMsg( NetMsgHead* pHead )
	{
		m_cOBuffer.Write( (char*)pHead , pHead->nSize );
	}

	void NetSocket::SendMsg()
	{
		if ( !m_bSending ) 
		{
			if ( int nLen = m_cOBuffer.ReadRemove( &m_arrSendBuffer ) )
			{
				m_bSending = true;
				async_write( *this , m_cSendBuffer1 , transfer_exactly( nLen ) , boost::bind( &NetSocket::SendMsg , this , boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )  ) ;
			}
		}
	}

	void NetSocket::SendMsg( const boost::system::error_code& ec, size_t bytes_transferred )
	{
		if ( ec )
		{
			Close();
			return;
		}
		else
		{
			if ( int nLen = m_cOBuffer.ReadRemove( &m_arrSendBuffer ) )
			{
				async_write( *this , m_cSendBuffer1 , transfer_exactly( nLen ) , boost::bind( &NetSocket::SendMsg , this , boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred )  ) ;
			}
			else
			{
				m_bSending = false;
			}
		}

	}

	void NetSocket::HandleWait( const boost::system::error_code& error )
	{
		if ( error )
		{
			return;
		}
		Close(); // 会调用ReadMsg ec = 1
	}

	void NetSocket::ReadHead()
	{
		async_read( *this , m_cHeadBuffer ,
			transfer_exactly( sizeof(NetMsgHead) ) ,
			boost::bind( &NetSocket::RecvMsg , this , boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) ) ;
		m_cTimer.cancel();
		m_cTimer.expires_from_now( boost::posix_time::seconds( 300 ) );
		m_cTimer.async_wait( boost::bind( &NetSocket::HandleWait , this , boost::asio::placeholders::error ) );
	}


	void NetSocket::ReadBody()
	{
		async_read( *this , m_cBodyBuffer ,
			transfer_exactly( m_nBodyLen - sizeof(NetMsgHead) ) ,
			boost::bind( &NetSocket::RecvMsg , this , boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) ) ;
		m_cTimer.cancel();
		m_cTimer.expires_from_now( boost::posix_time::seconds( 300 ) );
		m_cTimer.async_wait( boost::bind( &NetSocket::HandleWait , this , boost::asio::placeholders::error) );
	}

	void NetSocket::Run()
	{
		m_eRecvStage = REVC_FSRS_HEAD;
		ReadHead();
	}

}
