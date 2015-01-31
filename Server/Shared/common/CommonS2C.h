#ifndef __COMMONS2C_H_
#define __COMMONS2C_H_

#include "CommonDefine.h"


// s2c协议
enum EProS2C
{

	// 特殊协议
	PRO_S2C_ENCRYPT_INFO			= 900,	// 返回密钥信息

	// 所有协议号范围0~65536
	// 返回协议范围  1000\3000\5000\单数开发
	PRO_S2C_CHARACTER_LIST			= 2000,	// 登录帐号返回的所有角色简单信息
	PRO_S2C_CHARACTER_INFO			= 2001,	// 选择单个角色返回角色详细信息
	PRO_S2C_LOGIN_RESULT			= 2002,	// 登录的结果
	PRO_S2C_SELECT_RESULT			= 2003,	// 选择角色的结果
	PRO_S2C_CHAT_TO_WORLD			= 2004,	// 返回聊天消息
};


#pragma pack(push, 1)

// 返回登录信息
struct S2CLoginResult : public NetMsgHead
{
	S2CLoginResult():NetMsgHead(PRO_S2C_LOGIN_RESULT)
	{

	}

	enum ELoginResult
	{
		LR_SUCCESS,
		LR_SIGN_FAIL,
		LR_FAIL,
		LR_REPEAT,
	} eStatus; // 登录结果

};

// 返回密钥
struct S2CEncryptInfo : public NetMsgHead
{
	S2CEncryptInfo():NetMsgHead(PRO_S2C_ENCRYPT_INFO)
	{
		nSize = sizeof(*this);
	}
	char arrEncryptKey[MAX_ENCRYPT_LENTH];
};


#pragma pack(pop)




#endif