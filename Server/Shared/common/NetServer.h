//
//  server.h
//  test
//
//  Created by fox on 12-11-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Net_SERVER_H_
#define Net_SERVER_H_

#include "NetSocket.h"
#include "BaseDefine.h"
#include "BaseSingle.h"

namespace SWA
{

    class NetServer : public io_service 
    {
       
    public:
        
        virtual ~NetServer();

		/*
		 *
		 *	Detail: 获得标识ID
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
		int32 ID();
        
		/*
		 *
		 *	Detail: 设置启动嗠地址与监听端口
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
        void SetAddress( const char* pIp , uint16 pPort );

		/*
		 *
		 *	Detail: 设置Socket事件回调
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
        void SetHandler( PNetServerHandler pEnter , PNetServerMsgHandler pMsg  , PNetServerHandler pExit);
        	 

       /*
        *
        *	Detail: 启动服务
        *   
        *  Created by hzd 2015/01/23  
        *
        */ 	 
        void Start();

		/*
		 *
		 *	Detail: 停止服务
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
        void Stop();

		/*
		 *
		 *	Detail: 将Socket连接重新挂起
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
        void SetAccept( NetSocket& pSocket );
        
		/*
		 *
		 *	Detail: 消息处理 
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
        void Update(uint32 delay);

		/*
		 *
		 *	Detail: 获得Socket 
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
		NetSocket& GetSocket(int32 nIndex);

	private:

		NetServer();

		/*
		 *
		 *	Detail: IO循环处理 
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
        void Run();

		/*
		 *
		 *	Detail: 对所有客户端连接事件绑定启动 
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
		void   HandleStart();

		/*
		 *
		 *	Detail: 连接回调 
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
		void   HandleAccept( const boost::system::error_code& error , NetSocket* socket );
        
	private:
		
		int32			m_nServerID;						// 服务器标识唯一ID 
        SocketVector    m_vecUsedSocket;					// 使用中的Socket 
        SocketVector    m_vecAcceptSocket;					// 进入中的Socket 
			
        NetSocket*		m_arrSocket[ MAX_SOCKET_CONNECT ];	// 所有的Scoket 
        
        tcp::endpoint   m_cServerAddr;						// 服务地址端 
        tcp::acceptor   m_cAcceptor;						// 连接管理器 

		PNetServerHandler		m_pOnEnter;			// 连接回调 
		PNetServerHandler		m_pOnExit;			// 消息回调 
		PNetServerMsgHandler	m_pOnMsg;			// 断开回调 
        
        thread  m_cServiceThread;					// IO事件线程 
        mutex   m_cClientListMutex;					// 锁 

		friend class NetServerMgr;
        
    };

	class NetServerMgr : public BaseSingle<NetServerMgr>
	{
	public:

		/*
		 *
		 *	Detail: 创建一个NetServer，返回非NULL则成功
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
		NetServer* AddNetServer();

		/*
		 *
		 *	Detail: 通过serverID，可获得NetServer对象
		 *   
		 *  Created by hzd 2015/01/23  
		 *
		 */
		NetServer* GetNetServer(int32 nServerID);

	private:

		std::vector<NetServer*>	m_vecNetServer;	// 服务器窗器

	};

}

#endif
