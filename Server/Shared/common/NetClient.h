
/*
 *
 *	Detail: SocketÕ®–≈Client¿‡
 *   
 *  Created by hzd 2014-8-4
 *
 */
#ifndef SWA_BASE_NETCLIENT_H_
#define SWA_BASE_NETCLIENT_H_

#include "NetSocket.h"

namespace SWA {

		class NetClient : public io_service
		{

		public:

			friend class NetSocket;

			NetClient(void);
			virtual ~NetClient(void);

			
			void	SetAddress( const char* pIp , uint16 nPort );
			void	SetHandler( PNetServerHandler pEnter ,  PNetClientMsgHandler pMsg , PNetServerHandler pExit );

			void	Start();
			void	Update();

			
			NetSocket&	GetSocket() {
				return m_pGameSocket;
			}

		protected:
			void					HandleStart();
			void					HandleConnect(const boost::system::error_code& error,NetSocket* rSocket);
			void					Run();
			void					SetConnect(NetSocket& rSocket);
			void HandleConnect(const boost::system::error_code& error);
				
			PNetServerHandler		m_pOnMsgConnected;
			PNetClientMsgHandler	m_pOnMsgRecevied;
			PNetServerHandler		m_pOnMsgDisconnect;

			tcp::endpoint			m_cServerAddr;

			NetSocket				m_pGameSocket;

			thread					m_cServiceThread;

		};

}

#endif

