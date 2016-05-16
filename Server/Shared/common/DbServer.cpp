#include "DbServer.h"



DbServer::DbServer(void):m_bStatus(false),m_bLogStatus(false),m_pDataDb(NULL),m_pLogDb(NULL)
{
}

DbServer::~DbServer(void)
{
}

void DbServer::InitDataDb(IDataBase* pDb)
{
	if(pDb == NULL)
		return;
	m_pDataDb = pDb;
	m_bStatus	= true;
}

void	DbServer::InitLogDb(IDataBase* pDb)
{
	if(pDb == NULL)
		return;
	m_pLogDb = pDb;
	m_bLogStatus	= true;
}

void DbServer::Update(uint32 nDelay)
{
	if(m_bStatus)
	{
		DbResult* pResult = NULL;
		while ( ( pResult = m_pDataDb->GetAsyncResult() ) != NULL)
		{
			pResult->DB_Call_Read( pResult );
			pResult->Release();
		}
	}

	if(m_bLogStatus)
	{
		DbResult* pResult = NULL;
		while ( ( pResult = m_pLogDb->GetAsyncResult() ) != NULL)
		{
			pResult->DB_Call_Read( pResult );
			pResult->Release();
		}
	}
		
}



