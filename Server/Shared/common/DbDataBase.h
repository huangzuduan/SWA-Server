#ifndef SWA_DB_DATABASE_H_
#define SWA_DB_DATABASE_H_

#include <time.h>
#include <queue>

#include "BaseSingle.h"
#include "BaseDefine.h"
#include "DbConfig.h"
#include "DbField.h"
#include "DbRecord.h"
#include "DbRecordSet.h"


const ulong DB_MAX_DBACCESSTIME	= 10;
const ulong DB_MAX_STRINGSIZE	= 256;

/*
*
*	Detail: 数据库操作类
*   
*  Created by hzd 2014-10-20
*
*/
class DataBase : public IDataBase
{
public:

	// 打开数据连接 
	bool Open( const char* pHost, const char* pUsernName,const char* pPassword, const char* pDBName,bool bSQLChk = true );
		
	// 关闭连接 
	void Close();
	
	// 释放自己 
	virtual	void Release();

	// 获得连接ID 
	virtual uint32 ID();
		
	// 同步执行sql返回结果 
	virtual MYSQL_RES* ExecuteRsyncSQL( const char* pszSQL );

	// 导步执行sql推荐 
	bool ExecuteAsyncSQL( const char* pszSQL , void* pUser , SQLResultCallBack pCallback );

	// 获得结果集 
	virtual DbResult* GetAsyncResult();

	// 无sql执行回调操作(不推荐使用) 
	virtual	bool ExecuteAsyncPlayer(void* pData,PlayerCallBack pCallback);
	
	// 获得请求数 
	virtual uint32 GetRequestSize();

	// 获得结果数 
	virtual uint32 GetResultSize();

	// 获得数据连接对象 
	MYSQL* GetDBHandle();

	// 是否可用中
	bool IsOpen();

private:

	DataBase();
	virtual ~DataBase();

	// 检查sql语句是否合法 
	bool CheckSQL( const char* szSQL );

	// 连接数据库 
	MYSQL* Connect(char* szHost , char* szUser , char* szPasswd , char* szDB , uint32 nPort = MYSQL_PORT , char* szSocket = NULL , int32 nTimeout = 1000 );

	// 数据库线程执行的循环函数 
	virtual int32 OnThreadCreate();

	// 数据库线程销毁函数
	virtual int32 OnThreadDestroy();

	// 处理sql执行函数
	virtual int32 OnThreadProcess();

private:

	uint32	m_nID;								// 数据库连接ID 

	MYSQL*	m_pDbc;								// 数据库连接对象 
	mutex	m_cDBMutex;							// 加入队列锁 
	mutex	m_cQueueMutex;						// 执行队列锁 

	bool	m_bOpen;							// 是否可用 (一般不用判断，可用)
	char	m_arrHost[DB_MAX_STRINGSIZE];		// 数据库host
	char	m_arrUser[DB_MAX_STRINGSIZE];		// 数据库username
	char	m_arrPasswd[DB_MAX_STRINGSIZE];		// 数据库password
	char	m_arrDBName[DB_MAX_STRINGSIZE];		// 数据库名 
		
	queue<DbRequest*>	m_queAsyncSQLRequest;	// sql请求队列 
	queue<DbResult*>	m_queAsyncSQLResult;	// sql执行结果队列

	friend class DatabaseMgr;

};

// 数据管理器 
class DatabaseMgr : public BaseSingle<DatabaseMgr>
{
public:
	DatabaseMgr();
	/*
		*
		*	Detail: 添加一个数据库连接
		*   
		*  Created by hzd 2015-1-26
		*
		*/
	IDataBase* AddDatabase( const char* szDBServer , const char* szLoginName , const char* szPassword , const char* szDBName , bool bEnableSQLChk = true );

	/*
		*
		*	Detail: 获得某数据库连接
		*   
		*  Created by hzd 2015-1-26
		*
		*/
	IDataBase* GetDatabase(uint32 nDbID);

	/*
		*
		*	Detail: 获得主DB(默认为第一个连接)
		*   
		*  Created by hzd 2015/01/29  
		*
		*/
	IDataBase* GetMainDB();

	/*
		*
		*	Detail: 设置主DB
		*   
		*  Created by hzd 2015/01/29  
		*
		*/
	void SetMainDB(IDataBase& rIDataBase);

private:

	std::vector<IDataBase*> m_vecDbDatabase;  // 数据连接容器 

	IDataBase* m_pMainDB;					// 主DB

};



#endif

