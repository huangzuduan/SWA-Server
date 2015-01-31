#include "DbDataBase.h"
#include "DbRecordSet.h"
#include <iostream>
#include <string.h>

#ifdef WIN32
#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "winmm.lib")
#endif

namespace SWA
{

	DataBase::DataBase() :m_pDbc( NULL ),m_bOpen( false ) 
	{	
		memset(m_arrHost,0,sizeof(m_arrHost));
		memset(m_arrUser,0,sizeof(m_arrUser));
		memset(m_arrPasswd,0,sizeof(m_arrPasswd));
		memset(m_arrDBName,0,sizeof(m_arrDBName));
		static uint32 s_nIncDbID = 0;
		m_nID = ++s_nIncDbID;
	}

	DataBase::~DataBase()
	{
		while ( m_queAsyncSQLRequest.size() )
		{
			SSleep(100);
		}
		this->Close();
	}

	uint32 DataBase::ID()
	{
		return m_nID;
	}

	void DataBase::Release()
	{ 
		delete this;
	}

	int32 DataBase::OnThreadDestroy()
	{
		mysql_thread_end();
		return 0;
	}

	void DataBase::Close()
	{
		m_bOpen = false;
		SSleep(100);
		if (m_pDbc != NULL)
		{
			mysql_close(m_pDbc);
			m_pDbc = NULL;
		}
	}

	bool DataBase::Open(const char* pHost, const char* pUsernName, const char* pPassword, const char* pDBName, bool bSQLChk /*= true*/)
	{
		strcpy(m_arrHost, pHost);
		strcpy(m_arrUser, pUsernName);
		strcpy(m_arrPasswd, pPassword);
		strcpy(m_arrDBName, pDBName);

		if (!pHost || !pUsernName || !pPassword || !pDBName)
			return false;

		if (m_bOpen = m_pDbc = this->Connect( m_arrHost, m_arrUser, m_arrPasswd, m_arrDBName))
		{
			thread t(boost::bind( &DataBase::OnThreadCreate , this ) );
			return true;
		}else
		{
			FLOG4( "dbtool connect failed." );
			return false;
		}
	}

	MYSQL* DataBase::Connect( char* szHost, char* szUser, char* szPasswd, char* szDB, uint32 nPort, char* szSocket, int32 nTimeout)
	{	
		if(MYSQL* pHdbc = mysql_init( NULL ))
		{
			mysql_options( pHdbc , MYSQL_READ_DEFAULT_GROUP , "" );
			mysql_options( pHdbc , MYSQL_OPT_CONNECT_TIMEOUT , "" );

			pHdbc->options.read_timeout = nTimeout;
			pHdbc->reconnect = 1;
			pHdbc->free_me = 1;

			if ( mysql_real_connect( pHdbc , szHost , szUser , szPasswd , szDB ,
				nPort , szSocket , CLIENT_MULTI_RESULTS | CLIENT_MULTI_STATEMENTS | CLIENT_FOUND_ROWS | CLIENT_INTERACTIVE ) )
			{
				mysql_set_character_set( pHdbc, "utf8" );
				mysql_set_server_option( pHdbc, MYSQL_OPTION_MULTI_STATEMENTS_ON );
				return pHdbc;
			}else
			{
				printf( "ERROR: dbtool real connect failed: %s \n" , mysql_error(pHdbc) );
				mysql_close(pHdbc);
				return NULL;
			}
		}
		return NULL;
	}

	uint32 DataBase::GetRequestSize()
	{ 
		return m_queAsyncSQLRequest.size();
	}

	uint32 DataBase::GetResultSize()
	{ 
		return m_queAsyncSQLResult.size(); 
	}

	MYSQL* DataBase::GetDBHandle()
	{
		return m_pDbc;
	}

	bool DataBase::IsOpen()
	{
		return m_bOpen;
	}

	int32 DataBase::OnThreadCreate() 
	{
		while( m_bOpen && m_pDbc )
		{
			this->OnThreadProcess();
		}
		return 0;
	}

	int32 DataBase::OnThreadProcess()
	{
		if (m_queAsyncSQLRequest.size())
		{
			m_cQueueMutex.lock();
			DbRequest* pRequest = m_queAsyncSQLRequest.front();
			m_queAsyncSQLRequest.pop();
			m_cQueueMutex.unlock();

			if(pRequest->mCallbackType == ECALLBACK_NULL)
			{
				FLOG4("Error in database CALLBACK_NULL_NULL");
				assert(0);
				return 0;
			}

			if(pRequest->mCallbackType == ECALLBACK_DB_NULL)
			{
				m_cDBMutex.lock();
				if ( mysql_query( m_pDbc, pRequest->mDesrible) != 0 )
				{
					FLOG( LOGLEVEL4 , "Error in database requesting %s" , mysql_error(m_pDbc) );
					S_SAFE_DELETE(pRequest);
					m_cDBMutex.unlock();
					return 0;			
				}
				mysql_store_result(m_pDbc);
				mysql_next_result(m_pDbc); 
				m_cDBMutex.unlock();

			}else if(pRequest->mCallbackType == ECALLBACK_DB_HANDLER){

				m_cDBMutex.lock();
				if ( mysql_query( m_pDbc, pRequest->mDesrible) != 0 )
				{
					FLOG( LOGLEVEL4 , "Error in database requesting %s" , mysql_error(m_pDbc) );
					S_SAFE_DELETE(pRequest);
					m_cDBMutex.unlock();
					return 0;			
				}

				mysql_next_result(m_pDbc); // 如果查询的是多结果集，必需要调用该接口才可以获取下一条数据(第一条),在数据连接时就设置为多结果集 
				m_cDBMutex.unlock();

				DbRecordSet* pRecordSet = new DbRecordSet( *this , E_DB_MODE_EDIT );
 				MYSQL_RES* pSqlResult = mysql_store_result(m_pDbc);
				if (!pRecordSet->Create(pSqlResult))
				{
					S_SAFE_DELETE(pRequest);
					S_SAFE_DELETE(pRecordSet);
					return 0;					
				}

				DbResult* pResult = new DbResult;
				memset( pResult , 0 , sizeof(DbResult) );
				pResult->pCallResult	= pRequest->mCallResult;
				pResult->pCallNoResult	= pRequest->mCallPlayer;
				pResult->bFlag			= true;
				pResult->pData			= pRequest->pData;
				pResult->pRecordSet		= pRecordSet;
				pResult->eCallbackType	= pRequest->mCallbackType;

				S_SAFE_DELETE(pRequest);

				m_cQueueMutex.lock();
				m_queAsyncSQLResult.push( pResult );
				m_cQueueMutex.unlock();

			}else if(pRequest->mCallbackType == ECALLBACK_NULL_HANDLER){

				DbResult* pResult = new DbResult;
				
				memset( pResult , 0 , sizeof(DbResult) );
				pResult->pCallResult = pRequest->mCallResult;
				pResult->pCallNoResult = pRequest->mCallPlayer;
				pResult->bFlag = true;
				pResult->pData = pRequest->pData;
				pResult->pRecordSet = NULL;
				pResult->eCallbackType = pRequest->mCallbackType;

				S_SAFE_DELETE(pRequest);

				m_cQueueMutex.lock();
				m_queAsyncSQLResult.push( pResult );
				m_cQueueMutex.unlock();
				
			}
			return 0;
		}
		else
		{
			SSleep(1);
			return 0;                   //**************** a *return* routine here ********************
		}
	}

	MYSQL_RES* DataBase::ExecuteRsyncSQL( const char* pszSQL )
	{
		MYSQL_RES* pRes = NULL;
		m_cDBMutex.lock();
		// =0 suceess, !=0 fail
		if(mysql_query(m_pDbc,pszSQL) != 0 ){
			m_cDBMutex.unlock();
			return NULL;
		}
		pRes = mysql_store_result(m_pDbc);
		if (pRes == NULL) {
			m_cDBMutex.unlock();
			return NULL;
		}
		m_cDBMutex.unlock();
		return pRes; //mysql_free_result(pRes); 在使用完成pRes后，调用释放
	}

	bool DataBase::ExecuteAsyncSQL( const char* pszSQL , void * pData , SQLResultCallBack callback )
	{
		if ( !this->IsOpen() )
			return false;

		if ( strlen(pszSQL) == 0 )
		{
			cerr << "sql command is null" << endl;
			return false;
		}

		// chk sql
		if ( !(pszSQL && strlen(pszSQL) > 0 && strlen(pszSQL) < 4000) )
		{
			cerr << "strlen(pszSQL = " << strlen(pszSQL) << endl;
			cerr << "no sql command" << endl;
			return false;
		}

		DbRequest* request = new DbRequest;
		if (request == NULL) {
			//logManager::Print("memory (new dbRequest) allocating failed.");
			FLOG4("memory (new dbRequest) allocating failed.");
			return false;
		}

		strncpy( request->mDesrible , pszSQL , ::strlen(pszSQL) );

		request->mCallResult	= callback;
		request->pData				= pData;

		if(pData == NULL && callback == NULL)
			request->mCallbackType	= ECALLBACK_DB_NULL;

		if(pData != NULL && callback == NULL)
			request->mCallbackType	= ECALLBACK_NULL;

		if(pData !=NULL && callback != NULL)
			request->mCallbackType	= ECALLBACK_DB_HANDLER;

		if(pData == NULL && callback != NULL)
			request->mCallbackType	= ECALLBACK_NULL_HANDLER;

		m_cQueueMutex.lock();
		m_queAsyncSQLRequest.push( request );
		m_cQueueMutex.unlock();

		return true;
	}

	bool DataBase::CheckSQL( const char* szSQL )
	{
		if ( !szSQL )
			return false;

		char pszSQL[DB_MAX_SQL] = "";
		strcpy( pszSQL , szSQL );
		if ( 0 == strcmp(pszSQL, "UPDATE") )	
		{
			if ( !strstr(pszSQL, "WHERE") || !strstr(pszSQL, "LIMIT") )
			{
				return false;
			}
		}
		if ( 0 == strcmp(pszSQL, "DELETE") )
		{
			if ( !strstr(pszSQL, "WHERE") )
			{
				return false;
			}
		}

		return true;
	}

	DbResult* DataBase::GetAsyncResult()
	{
		if (m_queAsyncSQLResult.size())
		{
			m_cQueueMutex.lock();
			DbResult* pResult = m_queAsyncSQLResult.front();
			m_queAsyncSQLResult.pop();
			m_cQueueMutex.unlock();
			return pResult;
		}
		return NULL;
	}

	// 加入数据库查询队列，但不进行数据库查询操作
	bool DataBase::ExecuteAsyncPlayer(void* pData, PlayerCallBack callback)
	{
		DbRequest* pRequest = new DbRequest;
		pRequest->mCallPlayer = callback;
		pRequest->pData = pData;
		pRequest->mCallbackType = ECALLBACK_NULL_HANDLER;
		m_cQueueMutex.lock();
		m_queAsyncSQLRequest.push( pRequest );
		m_cQueueMutex.unlock();
		return true;
	}

	DatabaseMgr::DatabaseMgr():m_pMainDB(NULL)
	{

	};

	IDataBase* DatabaseMgr::AddDatabase( const char* szDBServer , const char* szLoginName , const char* szPassword , const char* szDBName , bool bEnableSQLChk /*= true*/)
	{
		SWA::DataBase* pDB = new DataBase;
		if (pDB->Open( szDBServer , szLoginName , szPassword , szDBName , bEnableSQLChk ) )
		{
			m_vecDbDatabase.push_back(pDB);
			return pDB;
		}
		pDB->Release();	
		return NULL;
	}

	IDataBase* DatabaseMgr::GetDatabase(uint32 nDbID)
	{
		std::vector<IDataBase*>::iterator it = m_vecDbDatabase.begin();
		for (; it != m_vecDbDatabase.end(); ++it)
		{
			if((*it)->ID() == nDbID)
				return *it;
		}
		return NULL;
	}

	IDataBase* DatabaseMgr::GetMainDB()
	{
		if(m_pMainDB)
			return m_pMainDB;
		if(m_vecDbDatabase.size())
			return *m_vecDbDatabase.begin();
		return NULL;
	}

	void DatabaseMgr::SetMainDB(IDataBase& rIDataBase)
	{
		m_pMainDB = &rIDataBase;
	}

} 

