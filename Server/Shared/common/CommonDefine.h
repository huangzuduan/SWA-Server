
#ifndef PROTOCOL_DEFIEN_H_
#define PROTOCOL_DEFIEN_H_

#include "BaseDefine.h"

#ifndef __COMMONDEF_H_
#define __COMMONDEF_H_

#include <memory>
union ValueType
{
	ValueType()
	{
		memset(this, 0, sizeof(*this));
	}

	template<typename T>
	ValueType(T v)
	{
		if (!(sizeof(*this) < sizeof(T)))
		{
			memcpy(this, &v, sizeof(T));
		}
	}
	int8	nInt8;
	int16	nInt16;
	int32	nInt32;
	int64	nInt64;
	uint8	nUint8;
	uint16	nUint16;
	uint32	nUint32;
	uint64	nUint64;
	float32	fFloat;
};

#endif


typedef vector< string > StringVector;
typedef vector< string >::iterator StringIter;

typedef vector< int32 > IntVector;
typedef vector< int32 >::iterator IntIter;

#define EncryptDefualt "1234567890ABCDEFGHIJKLMNOPQSRTWX"

/*常用常量定义*/
enum 
{
	MAX_ACCOUNT_LENG				= 32,	// 平台账号最大长度
	MAX_PASSWORD_LENG				= 32,   // 平台密码最大长度
	MAX_NICK_LENGTH					= 32,	// 昵称最大长度
	MAX_ROLE_TYPE_COUNT				= 4,	// 一个平台帐号最多允许创建角色数量
	MAX_CHAT_MSG_LENGTH				= 128,	// 聊天内容最大长度
	MAX_ENCRYPT_LENTH				= 32,	// 密钥长度
};

//客户端当前状态
enum ClientStatus
{
	CS_INVALID  = 0,
	CS_LOGIN_ING,			//	登录中
	CS_LOGIN_OK	,			//	登录完成
	CS_REGISTER_ING,		//  注册中
	CS_REGISTER_OK,			//	注册成功
	CS_LOADDATA_ING,		//	初始化数据中
	CS_LOADDATA_OK,			//	初始化数据完成
	CS_IN_GAME	,			//	进入游戏(选择或注册角色后)
	CS_RES_OK	,			//	前端资源加载完成
};


template <typename T> void Swap(T*  obj1,T* obj2)  
{  
	unsigned char * pObj1 = reinterpret_cast<unsigned char *>(obj1);  
	unsigned char * pObj2 = reinterpret_cast<unsigned char *>(obj2);  
	for (unsigned long x = 0; x < sizeof(T); ++x)  
	{  
		pObj1[x] ^= pObj2[x];  
		pObj2[x] ^= pObj1[x];  
		pObj1[x] ^= pObj2[x];  
	}  
}


// 角色类型
enum EPlayerRoleType
{
	ROLE_TYPE_INVAILD = 0,// 无效的
	ROLE_TYPE_1 ,
	ROLE_TYPE_2,
	ROLE_TYPE_3,
	ROLE_TYPE_4,

};


struct NetMsgHead
{
	NetMsgHead(uint32 _nType):nType(_nType)
	{}
	uint32 nSize;	// 大小
	uint32 nType;	// 协议
};

#endif