#ifndef __SWA_BASE_H_
#define __SWA_BASE_H_

#include "BaseDefine.h"
#include "BaseSingle.h"
#include "DbDataBase.h"

namespace SWA 
{
	class DbServer : public BaseSingle<DbServer>
	{
	public:

		DbServer(void);
		~DbServer(void);

		void	InitDataDb(IDataBase* pDb);
		void	InitLogDb(IDataBase* pDb);
		void	Update(uint32 delay);

		IDataBase*	GetDataDb()
		{
			return m_pDataDb;
		}

	private:

		bool		m_bStatus ;
		bool		m_bLogStatus;
		IDataBase*	m_pDataDb;
		IDataBase*	m_pLogDb;

	};
}

#define gDbServer  DbServer::Instance()

#endif
