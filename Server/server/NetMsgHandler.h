#ifndef _NETMSGHANDLER_H_
#define _NETMSGHANDLER_H_

class Player;
struct NetMsgHead;

using namespace SWA;

typedef void( *NetMsgHandlerf )( Player& player , NetMsgHead* pHead );


// 协议与对应的调用函数 
typedef struct 
{
    uint32			nType;	// 协议
    NetMsgHandlerf	pFun;	// 处理函数
}   NetMsgHandler;

typedef	map< uint32 , NetMsgHandler > NetMsgHandlerMap;
typedef map< uint32 , NetMsgHandler >::iterator NetMsgHandlerMapIter;

/*
 *
 *	Detail: 初始化消息与对应的函数
 *   
 *  Created by hzd 2015/01/26  
 *
 */
void InitNetMsg();

/*
 *
 *	Detail: 检查消息大小是否正确
 *   
 *  Created by hzd 2015/01/26  
 *
 */
bool CheckMsg(const NetMsgHead* pMsg , uint32 nSize );

// 常用消息转化宏
#define	CHECKMSG( classType ) const classType* pMsg = (classType*)pHead; if ( !CheckMsg( pMsg , sizeof( classType ) ) ){ FLOG4("CheckMsg Error! %d",pMsg->nType); return ;}

/*
 *
 *	Detail: 有客户端连接回调
 *   
 *  Created by hzd 2015/01/26  
 *
 */
void OnNetMsgEnter( NetSocket& rSocket );

/*
 *
 *	Detail: 有客户端断开回调(此时socket已经回收)
 *   
 *  Created by hzd 2015/01/26  
 *
 */
void OnNetMsgExit( NetSocket& rSocket );

/*
 *
 *	Detail: 有客户端消息收到
 *   
 *  Created by hzd 2015/01/26  
 *
 */
void OnNetMsg( NetSocket& rSocket , NetMsgHead* pHead );
	 
void OnError( int32 nError , int32 nServiceID , NetSocket& rSocket , const byte* pBuffer );
	


#endif

