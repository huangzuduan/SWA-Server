#ifndef _NET_LOGIN_HANDLER_H_
#define _NET_LOGIN_HANDLER_H_

class Player;
struct NetMsgHead;

/*
 *
 *	Detail: 请求断开
 *   
 *  Created by hzd 2015-1-20
 *
 */
void NetRequestClose( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 请求加密密钥信息
 *   
 *  Created by hzd 2014-12-10
 *
 */
void NetLoginEncryptInfoHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 登录
 *   
 *  Created by hzd 2014-9-27
 *
 */
void NetLoginAccountHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 角色列表
 *   
 *  Created by hzd 2014-9-27
 *
 */
void NetLoginRoleListHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 选择角色后进入游戏
 *   
 *  Created by hzd 2014-9-27
 *
 */
void NetLoginSelectRoleHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 创建新角色
 *   
 *  Created by hzd 2014-9-27
 *
 */
void NetLoginRegisterRoleHandler( Player& rPlayer , NetMsgHead* pHead );


/*
 *
 *	Detail: 资源加载完成
 *   
 *  Created by hzd 2014-9-27
 *
 */
void NetLoginResourcesLoaded( Player& rPlayer , NetMsgHead* pHead );

#endif

