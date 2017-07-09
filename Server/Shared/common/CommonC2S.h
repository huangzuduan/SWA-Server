#ifndef __COMMONC2S_H_
#define __COMMONC2S_H_

#include "CommonDefine.h"

// c2s 协议
enum EProC2S
{
	// 所有协议号范围0~65536
	// 请求协议范围  1000\3000\5000\单数开发
	//--------------------特殊协议--------------------
	PRO_C2S_NULL					= 0,
	PRO_C2S_ENCRYPT					= 800,// 获得密钥
	PRO_C2S_CLOSE					= 801,

	//--------------------登录功能--------------------
	PRO_C2S_LOGIN_ACCOUNT			= 1000,	// 登录
	PRO_C2S_REGISTER_ROLE			= 1001,	// 注册
	PRO_C2S_SELECT_ROLE				= 1002,	// 选择角色
	PRO_C2S_CREATE_ROLE				= 1003,	// 创建角色
	PRO_C2S_LOGIN_ROLE_LIST			= 1004,	// 角色列表
	PRO_C2S_CLIENT_RES_LOADED		= 1005,	// 资源加载完成 

	//--------------------角色功能--------------------
	PRO_C2S_CHARACTER_INFO			= 1100, // 角色功能

	//--------------------聊天功能--------------------
	PRO_C2S_CHAT_TO_WORLD			= 1200, // 世界聊天

};

#pragma pack(push, 1)

// 发送加密请求 
struct C2SEncryptInfo : public NetMsgHead
{
	C2SEncryptInfo():NetMsgHead(PRO_C2S_ENCRYPT)
	{
		nSize = sizeof(*this);
	}
};

// 登录协议
struct C2SLogin : public NetMsgHead
{
	C2SLogin() : NetMsgHead(PRO_C2S_LOGIN_ACCOUNT)
	{
		nSize = sizeof(*this);
		memset(arrUsername, 0, sizeof(arrUsername));
		memset(arrPassword, 0, sizeof(arrPassword));
	}
	char arrUsername[MAX_ACCOUNT_LENG];
	char arrPassword[MAX_PASSWORD_LENG];
};



#pragma pack(pop)


#endif