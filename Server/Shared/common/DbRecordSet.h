#ifndef SWA_DB_RECORDSET_H_
#define SWA_DB_RECORDSET_H_

#include "DbConfig.h"
#include "DbField.h"
#include "DbDataBase.h"

namespace SWA
{

	class DbRecord;
	class DbRecordSet : public DbIRecordSet
	{
		friend class DataBase;
		friend class DbRecord;

    protected:
		DbRecordSet( DataBase& rDataBase , EDbMode eMode );
		virtual ~DbRecordSet();
	public:

		bool Create( MYSQL_RES* pRES );
					 
		virtual void Release()
		{
			delete this;
		}
		
		virtual DbIRecord* GetRecord() const;
		virtual void* GetRecordBuff(uint32 nIndex) const;
		virtual ulong CountRecord() const
		{
			return m_vecRecord.size(); 
		}

		virtual void Move( uint32 nIndex );
		virtual bool Update( bool bSync = true );
		virtual void ClsEditFlag();
		virtual DbIRecord* MakeDefRecord();

		vector<DbFieldInfo>& GetFieldInfos(){
			return m_vecFieldInfo;
		}

		size_t FieldsCount() const
		{ 
			return m_vecFieldInfo.size();
		}

		// Operation
	protected:
		
		bool Create( const char* pszSQL );
		void Remove( DbRecord* pRec );
		DbRecord* MakeDefaultRecord( const char* pszTable );

		char* GetTableName()
		{
			return m_arrTableName;
		}

		const DbFieldInfo&	GetFieldInfo( int32 nIndex ) const
		{
			return m_vecFieldInfo[nIndex];
		}

		void BuildSQLCondition( char* pszConditionSQL );
		void BuildSQLOperation( char* pszOperationSQL );
			 
		bool UpdateRecord( DbRecord* pRec , bool bSync = true );
		bool DeleteRecord( DbRecord* pRec , bool bArchive = false );
		bool InsertRecord( DbRecord* pRec );

	private:

		DbRecordSet( const DbRecordSet& res );
		DbRecordSet& operator= ( const DbRecordSet& res );

	private:

		DataBase&				m_rDataBase;
		EDbMode					m_eMode;

		int32					m_nCursor;

		char					m_arrSQL[DB_MAX_SQL];
		char					m_arrTableName[32];
		uint32					m_nKeyIndex;
		uint32					m_nAutoIncIndex;

		vector< DbFieldInfo >	m_vecFieldInfo;
		vector< DbRecord* >		m_vecRecord;

	};
};




#endif



