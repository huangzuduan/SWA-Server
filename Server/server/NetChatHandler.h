#ifndef _NETCHATHANDLER_H_
#define _NETCHATHANDLER_H_


class Player;
struct NetMsgHead;

/*
 *
 *	Detail: 世界聊天
 *   
 *  Created by hzd 2014-9-27
 *
 */
void NetChatWorldMsgHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 个人聊天、陌生人、好友均可，会有一个最近聊天记录列表
 *   
 *  Created by hzd 2014-10-22
 *
 */
void NetChatPrviteMsgHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 队伍聊天、自己队伍内的聊天
 *   
 *  Created by hzd 2014-10-22
 *
 */
void NetChatTeamMsgHandler( Player& rPlayer , NetMsgHead* pHead );

#endif

