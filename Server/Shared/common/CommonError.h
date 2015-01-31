#ifndef _GAMEERROR_H_
#define _GAMEERROR_H_


#include "CommonDefine.h"
#include "LogManager.h"


// 错误码

namespace error 
{

	enum
	{
	
		// 系统错误码
		GAME_ERROR_CONFIG_NOEXIST			= 1001,		// 配置不存在
		GAME_ERROR_UID_REPEAT				= 1002,		// UID记录重复

		// 用户错误码
		GAME_ERROR_NOT_LOGIN				= 2001,		// 未登录
		GAME_ERROR_PARAME_ERROR				= 2002,		// 参数错误%d
		GAME_ERROR_ROLE_MAX					= 2003,		// 创建角色数量达上限%d
		GAME_ERROR_SIGN_ERROR				= 2004,		// sign值错误
		GAME_ERROR_NOTGUEST_ID				= 2005,		// 非法ID错误
		GAME_ERROR_ZONEID_ERROR				= 2006,		// zoneid错误%d
		GAME_ERROR_NICK_EXIST				= 2007,		// 该角色已经存在
		GAME_ERROR_LOGIN_REPEAT				= 2008,		// 重复请求登录
		GAME_ERROR_LOGIN_OTHRE				= 2009,		// 你的账号在别处登录

	};

}




#endif


