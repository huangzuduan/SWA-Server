#ifndef SWA_NETSOCKET_H_
#define SWA_NETSOCKET_H_

#include "NetIOBuffer.h"
#include "CommonDefine.h"

/*
 *
 *	Detail: Socket类
 *   
 *  Created by hzd 2014-1-1
 *
 */

namespace SWA
{
    enum ERecvType
    {
        REVC_FSRS_NULL	= 0,
        REVC_FSRS_HEAD	,
        REVC_FSRS_BODY	,
    };
	enum  EMsgRead
	{
		MSG_READ_INVALID	,
		MSG_READ_WAITTING	,
		MSG_READ_OK			,
		MSG_READ_REVCING	,
	};

    class NetSocket : public tcp::socket
    {
    public:
        NetSocket( io_service& rIo_service );
		virtual ~NetSocket();

		/*
		 *
		 *	Detail: 获得SocketID，从0开始自增
		 *   
		 *  Created by hzd 2015/01/21  
		 *
		 */
		int32 SID()
		{
			return m_nID;
		}

		/*
		 *
		 *	Detail: 压入要准备发送的数据
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   ParkMsg( NetMsgHead* pHead );

		/*
		 *
		 *	Detail: 发送数据
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   SendMsg();

		/*
		 *
		 *	Detail: 读取缓存中的数据
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        EMsgRead  ReadMsg( NetMsgHead** pHead );

		/*
		 *
		 *	Detail: 移除缓存中的数据
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   RemoveMsg( uint32 nLen );

		/*
		 *
		 *	Detail: 断开socket
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   Disconnect();

		/*
		 *
		 *	Detail: 恢复到构造函数那种状态
		 *   
		 *  Created by hzd 2015-1-20
		 *
		 */
		void   Clear();

		/*
		 *
		 *	Detail: 启动Socket，可以进入收发数据
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   Run();

		/*
		 *
		 *	Detail: 获得Socket客户端IP地址
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		const string  GetIp(){return remote_endpoint().address().to_string();}

    private:
		
		/*
		 *
		 *	Detail: 关闭socket
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void Close();

		/*
		 *
		 *	Detail: 收到指定长度数据回调函数
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   RecvMsg( const boost::system::error_code& ec, size_t bytes_transferred );

		/*
		 *
		 *	Detail: 发送数据回调函数，发送后发现有新数据，则会选择断续发送（一般不会发生这种情况）
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   SendMsg( const boost::system::error_code& ec, size_t bytes_transferred );
		
		/*
		 *
		 *	Detail: 读消息头
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   ReadHead();

		/*
		 *
		 *	Detail: 读消息主体
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   ReadBody();

		/*
		 *
		 *	Detail: 超时回调函数，将会关闭socket
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void    HandleWait( const boost::system::error_code& error );

	private:

		int32				m_nID;				// socketID，从0开始 

		deadline_timer		m_cTimer;			// 定时器，用于检测用户多久没有与服务器通信，否则会角色HandleWait函数 

		NetIOBuffer			m_cIBuffer;			// 管理收到的字节管理器 
		NetIOBuffer			m_cOBuffer;			// 管理发出的字节管理器 

		mutable_buffers_1	m_cHeadBuffer;		// 收到头数据缓存绑定类 
		mutable_buffers_1	m_cBodyBuffer;      // 收到的主体数据绑在绑定类 
		mutable_buffers_1	m_cSendBuffer1;     // 发送的数据缓存绑定类 

		ERecvType			m_eRecvStage;		
		uint16				m_nBodyLen;			
		char				m_arrRecvBuffer[ MAX_SOCKET_BUFFER ];	

		int32				m_bSending;			
		char				m_arrSendBuffer[ MAX_SOCKET_BUFFER ];	

		bool				m_bInvaild; 

    };

	typedef void(*PNetServerHandler)(NetSocket& pSocket);
	typedef void(*PNetServerMsgHandler)(NetSocket& pSocket , NetMsgHead* pMsg );
	typedef void(*PNetClientMsgHandler)(NetMsgHead* pMsg );

    typedef list<NetSocket*> SocketList;
    typedef list<NetSocket*>::iterator SocketListIter;
    typedef vector<NetSocket*> SocketVector;
    typedef vector<NetSocket*>::iterator SocketVectorIter;
    
};

#endif
