#ifndef SWA_NETSOCKET_H_
#define SWA_NETSOCKET_H_

#include "NetIOBuffer.h"
#include "CommonDefine.h"

/*
 *
 *	Detail: Socket��
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
		 *	Detail: ���SocketID����0��ʼ����
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
		 *	Detail: ѹ��Ҫ׼�����͵�����
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   ParkMsg( NetMsgHead* pHead );

		/*
		 *
		 *	Detail: ��������
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   SendMsg();

		/*
		 *
		 *	Detail: ��ȡ�����е�����
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        EMsgRead  ReadMsg( NetMsgHead** pHead );

		/*
		 *
		 *	Detail: �Ƴ������е�����
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   RemoveMsg( uint32 nLen );

		/*
		 *
		 *	Detail: �Ͽ�socket
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
        void   Disconnect();

		/*
		 *
		 *	Detail: �ָ������캯������״̬
		 *   
		 *  Created by hzd 2015-1-20
		 *
		 */
		void   Clear();

		/*
		 *
		 *	Detail: ���Socket�����Խ����շ�����
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   Run();

		/*
		 *
		 *	Detail: ���Socket�ͻ���IP��ַ
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		const string  GetIp(){return remote_endpoint().address().to_string();}

    private:
		
		/*
		 *
		 *	Detail: �ر�socket
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void Close();

		/*
		 *
		 *	Detail: �յ�ָ���������ݻص�����
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   RecvMsg( const boost::system::error_code& ec, size_t bytes_transferred );

		/*
		 *
		 *	Detail: �������ݻص����������ͺ����������ݣ����ѡ��������ͣ�һ�㲻�ᷢ�����������
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   SendMsg( const boost::system::error_code& ec, size_t bytes_transferred );
		
		/*
		 *
		 *	Detail: ����Ϣͷ
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   ReadHead();

		/*
		 *
		 *	Detail: ����Ϣ����
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void   ReadBody();

		/*
		 *
		 *	Detail: ��ʱ�ص�����������ر�socket
		 *   
		 *  Created by hzd 2015-1-21
		 *
		 */
		void    HandleWait( const boost::system::error_code& error );

	private:

		int32				m_nID;				// socketID����0��ʼ 

		deadline_timer		m_cTimer;			// ��ʱ�������ڼ���û����û���������ͨ�ţ�������ɫHandleWait���� 

		NetIOBuffer			m_cIBuffer;			// �����յ����ֽڹ����� 
		NetIOBuffer			m_cOBuffer;			// ����������ֽڹ����� 

		mutable_buffers_1	m_cHeadBuffer;		// �յ�ͷ���ݻ������ 
		mutable_buffers_1	m_cBodyBuffer;      // �յ����������ݰ��ڰ��� 
		mutable_buffers_1	m_cSendBuffer1;     // ���͵����ݻ������ 

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
