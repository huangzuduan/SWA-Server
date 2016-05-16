#ifndef CLIENT_NET_HANDLER_H_
#define CLIENT_NET_HANDLER_H_

/*
 *
 *	Detail: 客户端Handler事件
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class ClientNetHandler
{
public:

	ClientNetHandler(void);
	~ClientNetHandler(void);

	// 请求由服务器主动断开
	static void SendRequestClose();

	/////////////////////////////发送/////////////////////////////////////////////
	static void SendEncrypt();
	static void SendLogin();
	static void SendCharacterList();
	static void SendRegister();
	static void SendChar2World();
	static void SendLoadResed();

	/////////////////////////////接收/////////////////////////////////////////////

	static void RecvEncryptInfo(NetMsgHead* head);
	static void RevcCharacterList(NetMsgHead* head);
	static void RevcLoginResult(NetMsgHead* head);
	static void RecvSelectResult(NetMsgHead* head);
	static void RecvRegisterResult(NetMsgHead* head);
	static void RecvCharacterInfo(NetMsgHead* head);
	static void RecvChatInfo(NetMsgHead* head);


};

#endif

