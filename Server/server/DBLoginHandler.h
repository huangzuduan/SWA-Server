#ifndef _DBLOGINHANDLER_H_
#define _DBLOGINHANDLER_H_


class Player;

using namespace SWA;

// 登录平台账号返回
void DBLoginRepAccount(void* pData ,const void* dbdata);


// 创建角色
void DBLoginRepCreateRole(void* pData ,const void* dbdata);

// 返回角色列表
void DBSelectRoleList(void* pData ,const void* dbdata);

// 选择角色返回 
void DBLoginRepSelectRole(void* pData ,const void* dbdata);

// 加载完所有的角色数据回调
void NoDBCharacterLoadAllDataFinished(void* pData);

#endif



