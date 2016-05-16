#ifndef SWA_DB_FIELD_H_
#define SWA_DB_FIELD_H_

#include "DbConfig.h"

struct DbFieldInfo
{
	string strName;
	uint32 nType;
	uint32 nAttr;
	ulong  nLen;
};

class DbField : public DbIData
{
	friend class DbRecord;

public:

	DbField( const DbFieldInfo sInfo  , DbRecord& rDbRecord);
	DbField( const DbField& rField );
	virtual ~DbField();

	DbField& operator = ( const DbField& rField );

public:

	// Interface
	virtual void Release()
	{
		delete this;
	}

	virtual operator char() const;
	virtual operator uchar() const;
	virtual operator int16() const;
	virtual operator uint16() const;
	virtual operator int32() const;
	virtual operator uint32() const;
	virtual operator int64() const;
	virtual operator uint64() const;
	virtual operator float32() const;
	virtual operator float64() const;
	virtual operator char*() const;
	virtual operator const char*() const;

	virtual DbIData& operator = ( char chOp );
	virtual DbIData& operator = ( uchar chOp );
	virtual DbIData& operator = ( int16 nOp );
	virtual DbIData& operator = ( uint16 unOp );
	virtual DbIData& operator = ( int32 iOp );
	virtual DbIData& operator = ( uint32 uOp );
	virtual DbIData& operator = ( int64 uOp );
	virtual DbIData& operator = ( uint64 uOp );
	virtual DbIData& operator = ( float32 fltOp );
	virtual DbIData& operator = ( float64 dblOp );
	virtual DbIData& operator = ( const char* szVal );

public:

	const char* GetName() const
	{
		return m_Info.strName.c_str();
	}
		
	uint32 GetType() const
	{
		return m_Info.nType;
	}

	uint32 GetAttr() const
	{
		return m_Info.nAttr;
	}

	ulong GetLen() const
	{
		return m_Info.nLen;
	}

	bool IsChanged() const
	{
		return m_bChanged;
	}

	void TagChanged( bool bChange )
	{
		m_bChanged = bChange;
	}

	bool SetValue( const char* pszValue );
	char* GetValue();

private:

	static void FireError();

private:

	const DbFieldInfo	m_Info;			// 字段信息 
	bool				m_bChanged;		// 是否发生改变
	DbRecord&			m_rDbRecord;	// 所在的行
	uint16				m_nBufStart;	// 在行buff中数据的起始位置 

};

#endif

