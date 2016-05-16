
#include "DbRecordSet.h"
#include "DbRecord.h"


const uint32 INVALID_KEY = UINT_MAX;

DbRecordSet::DbRecordSet( DataBase& dbtool , EDbMode eMode ):m_rDataBase( dbtool ) ,m_nCursor( 0 ),m_nKeyIndex( INVALID_KEY ) ,m_nAutoIncIndex( INVALID_KEY ) ,m_eMode( eMode )
{
	memset( m_arrSQL , 0L , sizeof(m_arrSQL) );
}

DbRecordSet::~DbRecordSet()
{
	while ( m_vecRecord.size() != 0 ) 
	{
		DbRecord* pRec = m_vecRecord[m_vecRecord.size() - 1];
		S_SAFE_RELEASE( pRec );
	}

	m_nCursor = 0;
	m_nKeyIndex = INVALID_KEY;
	m_eMode = E_DB_MODE_NONE;
	m_vecFieldInfo.clear();
	m_vecRecord.clear();
}


bool			DbRecordSet::Create( const char* pszSQL )
{	
	if ( !pszSQL )
		return false;

	if ( m_rDataBase.IsOpen() )
		return false;

	MYSQL_RES* pRES = m_rDataBase.ExecuteRsyncSQL( pszSQL );

	// save SQL statement
	memcpy( m_arrSQL, pszSQL, strlen(pszSQL) );
	//_strupr_s( mSQL , DB_MAX_SQL);
	return this->Create( pRES );
}

// 将查询到的数据源(多条),
bool			DbRecordSet::Create( MYSQL_RES* pRes )
{
	if ( !pRes )
		return false;

	// 填写字段信息
	MYSQL_FIELD* fields = mysql_fetch_fields( pRes );
	uint32 unNumFields = mysql_num_fields( pRes );
	/************************************************************************/
	/* 用mysql_fetch_field_direct() 获得字段信息                                                                    */
	/************************************************************************/
	MYSQL_FIELD* field = NULL;

	for (uint32 ii = 0 ; ii < unNumFields; ii++)
	{
		field = mysql_fetch_field_direct(pRes,ii);
		DbFieldInfo fieldInfo;
		fieldInfo.strName = field->name;
		fieldInfo.nType = field->type;
		fieldInfo.nAttr = field->flags;
		fieldInfo.nLen = field->length;

		m_vecFieldInfo.push_back( fieldInfo );

		if (field->flags & PRI_KEY_FLAG )
			m_nKeyIndex = ii;

		if ( field->flags & AUTO_INCREMENT_FLAG )
			m_nAutoIncIndex = ii;
	}
	
	if ( !fields )
	{
		mysql_free_result( pRes );
		return false;
	}

	int64 ulNumRecord = mysql_num_rows( pRes );

	for ( ulong r = 0 ; r < ulNumRecord ; r++ )
	{
		// 移动到第i个记录
		mysql_data_seek( pRes , r );
		// 取第i个记录数据
		MYSQL_ROW row = mysql_fetch_row( pRes );
		if ( !row )
			continue;
 		DbRecord* pRec = new DbRecord( *this , row , unNumFields );
 		m_vecRecord.push_back( pRec );
	}

	// move first!
	this->Move(0);
	// get table name
	strcpy( m_arrTableName , field->table );
	// free MYSQL_RES
	mysql_free_result( pRes );
	return true;
}


void			DbRecordSet::Remove( DbRecord* pRec )
{	
	std::vector< DbRecord* >::iterator itor = std::find( m_vecRecord.begin() , m_vecRecord.end() , pRec );

	if ( itor != m_vecRecord.end() )
	{
		m_vecRecord.erase(itor);
	}
}


DbRecord*		DbRecordSet::MakeDefaultRecord( const char* pszTable )
{	
	if ( !pszTable )
		return NULL;

	if ( !m_rDataBase.IsOpen() )
		return NULL;

	char szSQL[100];
	sprintf( szSQL , "SELECT * FROM %s LIMIT 0" , pszTable );

	MYSQL_RES* pRES = m_rDataBase.ExecuteRsyncSQL( szSQL );
	
	if ( !pRES )
		return NULL;

	// 填写字段信息
	MYSQL_FIELD* fields = mysql_fetch_fields( pRES );
	
	if ( !fields )
		return NULL;

	uint32 unNumFields = mysql_num_fields( pRES );

	for (uint32 i = 0 ; i < unNumFields ; i++ )
	{
		DbFieldInfo fieldInfo;
		fieldInfo.strName = fields[i].name;
		fieldInfo.nType = fields[i].type;
		fieldInfo.nAttr = fields[i].flags;
		fieldInfo.nLen = fields[i].length;

		m_vecFieldInfo.push_back(fieldInfo);

		if ( fields[i].flags & PRI_KEY_FLAG )
		{
			m_nKeyIndex	= i;
		}
		if ( fields[i].flags & AUTO_INCREMENT_FLAG )
		{
			m_nAutoIncIndex= i;
		}
	}

	// 填写缺省记录数据
 	DbRecord* pDefRec = new DbRecord( *this , fields , unNumFields );
 	m_vecRecord.push_back( pDefRec );

	// move first!
	this->Move(0);

	// save SQL statement
	memcpy( m_arrSQL , szSQL , strlen(szSQL) );
	//_strupr_s( mSQL , DB_MAX_SQL);

	// get table name
	strcpy( m_arrTableName , fields[0].table );

	// free MYSQL_RES
	mysql_free_result( pRES );

	return pDefRec;
}



DbIRecord*		DbRecordSet::GetRecord() const
{	
	if ( 0 > m_nCursor )
		return NULL;
	return m_vecRecord[m_nCursor];
}

void*		DbRecordSet::GetRecordBuff(uint32 nIndex) const 
{
	
	if ( nIndex >= m_vecRecord.size() )
		return NULL;
	return (m_vecRecord[nIndex])->GetRowBuff();
}


void			DbRecordSet::Move( uint32 nIndex )
{	
	if ( m_vecRecord.empty() )
	{
		m_nCursor = -1;
		return;
	}

	if ( nIndex >= m_vecRecord.size() )
		m_nCursor = (int32)m_vecRecord.size() - 1;
	else
		m_nCursor = nIndex;
}


bool			DbRecordSet::Update( bool bSync /* = true */ )
{	
 	std::vector< DbRecord* >::iterator itor = m_vecRecord.begin();
 	for (; itor != m_vecRecord.end() ; ++itor )
 	{
 		DbRecord* pRec = *itor;
 		if ( pRec )
 			continue;
 		this->UpdateRecord( pRec , bSync );
 	}
	return true;
}



void			DbRecordSet::ClsEditFlag()
{
	for ( uint32 i = 0 ; i< m_vecRecord.size() ; i++ )
	{
 		DbRecord* pRec = m_vecRecord[i];
 		if (!pRec)
 			continue;
 		pRec->ClsEditFlag();
	}
}


DbIRecord*		DbRecordSet::MakeDefRecord()
{
	if ( !m_rDataBase.IsOpen() )
		return NULL;

	if ( E_DB_MODE_EDIT != m_eMode )
		return NULL;
	// new a default record, but it isn`t in dbtool	
	uint32 unAmountField = (uint32)m_vecFieldInfo.size();
 	DbRecord* pRec = new DbRecord( *this , unAmountField );
 	if ( !pRec )
 		return NULL;
 	return pRec;
}


void			DbRecordSet::BuildSQLCondition( char* pszConditionSQL )
{
	if ( !pszConditionSQL )
		return;
	//memset( pszConditionSQL , 0L , sizeof(pszConditionSQL) );
	pszConditionSQL[0]	='\0';
 	DbRecord* pRec = m_vecRecord[m_nCursor];
 	if ( !pRec )
 		return;
	sprintf( pszConditionSQL , "%s=%s", pRec->KeyName() , (char*)pRec->Key() );
}


void			DbRecordSet::BuildSQLOperation( char* pszOperationSQL )
{
	if ( !pszOperationSQL )
		return;
	//memset(pszOperationSQL , 0L , sizeof(pszOperationSQL) );
	pszOperationSQL[0]	='\0';
 	DbRecord* pRec = m_vecRecord[m_nCursor];
 	if ( !pRec )
 		return;
 	if  ( E_DB_MODE_EDIT == m_eMode )
 		pRec->BuildSQLOperation( pszOperationSQL );
}


bool			DbRecordSet::UpdateRecord( DbRecord* pRec , bool bSync /* = true */ )
{
	if ( !m_rDataBase.IsOpen() )
		return false;

	if ( E_DB_MODE_EDIT != m_eMode )
		return false;

 	if ( !pRec )
 		return false;

	// build sql operate
	char szOperationSQL[1024];

	if ( !pRec->BuildSQLOperation(szOperationSQL) )
		return false;

	// build sql condition
	char szConditionSQL[128];
	pRec->BuildSQLCondition( szConditionSQL );

	char szSQL[2048] = "";
	sprintf( szSQL , "UPDATE %s SET %s WHERE %s LIMIT 1" , m_arrTableName , szOperationSQL , szConditionSQL );

	bool ret = false;

	//20070511 		if (bSync)
	// 			ret = mDB.ExecuteSyncSQL(szSQL);
	// 		else
	// 			ret = mDB.ExecuteAsyncSQL(szSQL);


	// clear edit flag
	if ( ret )
		this->ClsEditFlag();

	return ret;
}



bool			DbRecordSet::DeleteRecord( DbRecord* pRec , bool bArchive /* = false */ )
 {
 	if ( !m_rDataBase.IsOpen() )
 		return false;
 
 	if ( E_DB_MODE_EDIT != m_eMode )
 		return false;
 
 	if ( bArchive )
 	{
 		// build operate sql
 		char szOperationSQL[1024];
 		pRec->BuildSQLOperation( szOperationSQL );
 
 		char szConditionSQL[128];
 		pRec->BuildSQLCondition( szConditionSQL );
 
 		char szSQL[2048];
 		sprintf( szSQL , "UPDATE %s SET %s WHERE %s LIMIT 1", m_arrTableName , szOperationSQL , szConditionSQL );

 		if ( !m_rDataBase.ExecuteRsyncSQL(szSQL) )
 			return false;
 	}
 	else
 	{
 		// build cdffition sql
 		char szConditionSQL[128];
 		pRec->BuildSQLCondition( szConditionSQL );
 
 		char szSQL[1024];
 		sprintf( szSQL , "DELETE FROM %s WHERE %s LIMIT 1" , m_arrTableName , szConditionSQL );
 		
		if ( !m_rDataBase.ExecuteRsyncSQL(szSQL) )
 			return false;
 	}
 
 	std::vector< DbRecord* >::iterator itor = std::find( m_vecRecord.begin() , m_vecRecord.end() , pRec );
 
	if ( itor != m_vecRecord.end() )
 		m_vecRecord.erase( itor );
 
 	S_SAFE_RELEASE( pRec );
 
	return true; 
}

bool DbRecordSet::InsertRecord( DbRecord* pRecord )
{
	if ( NULL == pRecord )
		return NULL;

	if ( !m_rDataBase.IsOpen() )
		return NULL;

	if ( E_DB_MODE_EDIT != m_eMode )
		return NULL;

	char szOperationSQL[1024];
	pRecord->BuildSQLOperation( szOperationSQL );

	static char szSQL[128];
	memset(szSQL,0,128);
	sprintf( szSQL , "INSERT INTO %s SET %s", m_arrTableName , szOperationSQL );
	if ( m_rDataBase.ExecuteRsyncSQL(szSQL) )
		return NULL;
	m_vecRecord.push_back( pRecord );
	return true;

}

