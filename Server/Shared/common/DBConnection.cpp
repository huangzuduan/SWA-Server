#include <mysql/errmsg.h>

#include "DBConnection.h"
#include <string.h>
//#include "DBHelper.hpp"

namespace db
{


	using namespace std;

	//__attribute__((unused))
	static bool initialize()
	{
		return 0 == mysql_library_init(0, NULL, NULL);
	}

	static void finalize()
	{
		mysql_library_end();
	}

	class __APIInitializer
	{
	private:
		__APIInitializer() {
			//		bool initResult = initialize();
					//LOG4CXX_ERROR(gCommonLogger, "Initialize DB API, " <<
					//		(initResult ? "Succeed" : "Failed"));
		}
		__APIInitializer(const __APIInitializer& other);
		~__APIInitializer() {
			//LOG4CXX_INFO(gCommonLogger, "Uninitialize DB API");
			finalize();
		}
		__APIInitializer& operator=(const __APIInitializer& other);
	public:
		static __APIInitializer& getInst() {
			static __APIInitializer obj;
			return obj;
		}
	};

	static const __APIInitializer& gDBInitializer = __APIInitializer::getInst();

	DBConnection::DBConnection() :
		m_pMysql(NULL),
		isConnected_(false),
		readTimeout_(60),
		writeTimeout_(60),
		connectTimeout_(60)
	{
	}

	DBConnection::~DBConnection()
	{
		close();
	}

	void DBConnection::close()
	{
		//LOG4CXX_WARN(gCommonLogger, "DB is closing!");
		// call mysql_close despite whether it's been inited.
		mysql_close(m_pMysql);
	}

	bool DBConnection::connect(const std::string& dbHost,
		const std::string& username,
		const std::string& password,
		const std::string& dbName,
		unsigned int port)
	{
		serverAddr_ = dbHost;
		username_ = username;
		password_ = password;
		dbName_ = dbName;
		port_ = port;

		if (realConnect())
		{
			isConnected_ = true;
			return true;
		}
		return false;
	}

	bool DBConnection::realConnect()
	{
		m_pMysql = mysql_init(NULL);
		if (NULL == m_pMysql)
		{
			return false;
		}

		mysql_options(m_pMysql, MYSQL_OPT_CONNECT_TIMEOUT, &connectTimeout_);
		mysql_options(m_pMysql, MYSQL_OPT_READ_TIMEOUT, &readTimeout_);
		mysql_options(m_pMysql, MYSQL_OPT_WRITE_TIMEOUT, &writeTimeout_);

		MYSQL* handle = mysql_real_connect(m_pMysql, serverAddr_.c_str(),
			username_.c_str(), password_.c_str(),
			dbName_.c_str(), port_, NULL, CLIENT_MULTI_STATEMENTS);
		/*MYSQL* handle = mysql_real_connect(&mysql_, serverAddr_.c_str(),
				username_.c_str(), password_.c_str(),
				dbName_.c_str(), port_, NULL, 0);*/

		if (!handle || mysql_errno(m_pMysql) != 0)
		{
			const char* errMsg = mysql_error(m_pMysql);
			// LOG(ERROR) << "Database connect exception: " << errMsg;
			return false;
		}

		mysql_set_character_set(m_pMysql, "utf8");

		//mysql_set_server_option(&mysql_, MYSQL_OPTION_MULTI_STATEMENTS_ON);

		return true;
	}

	bool DBConnection::queryWithReconnectRetrial(const char* sql)
	{
		if (mysql_real_query(m_pMysql, sql, strlen(sql)) == 0)
		{
			//		do
			//	    {
			//			MYSQL_RES* result_=mysql_store_result(&mysql_);
			//	        mysql_free_result(result_);
			//	    }while(!mysql_next_result(&mysql_)); // to solve the "2014:Commands out of sync; " problem

			return true;
		}

		int bErr = mysql_errno(m_pMysql);
		if (CR_SERVER_LOST == bErr || CR_CONN_HOST_ERROR == bErr
			|| CR_SERVER_GONE_ERROR == bErr)
		{
			if (realConnect())
			{
				//			LOG(INFO) << "Reconnected database server";
				if (mysql_real_query(m_pMysql, sql, strlen(sql)) == 0)
				{
					//				do
					//				{
					//					MYSQL_RES* result_=mysql_store_result(&mysql_);
					//					mysql_free_result(result_);
					//				}while(!mysql_next_result(&mysql_)); // to solve the "2014:Commands out of sync; " problem

					return true;
				}
			}
		}

		//	LOG(ERROR) << "Error execute SQL, errno:" << mysql_errno(m_pMysql)
		 //           << ": " << mysql_error(m_pMysql) << " SQL:" << sql;
		return false;
	}


	bool  DBConnection::tableExist(const std::string & name)
	{
		//return checkTableExistInDatabase(mysql_, dbName_, name);
		return true;
	}



	boost::int64_t DBConnection::getRowsCount(const std::string& stem)
	{
		if (!queryWithReconnectRetrial(stem.c_str()))
			return -1;

		MYSQL_RES* res = mysql_store_result(m_pMysql);
		if (!res) {
			//        LOG(ERROR) << "mysql store result is failed, " 
			//            << mysql_error(m_pMysql);
			return -1;
		}

		boost::int64_t numRows = mysql_num_rows(res);
		mysql_free_result(res);

		return numRows;
	}

	long DBConnection::affectedRows()
	{
		return (long)mysql_affected_rows(m_pMysql);
	}

}

