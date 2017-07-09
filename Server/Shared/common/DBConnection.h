#ifndef DBCONNECTION_HPP_
#define DBCONNECTION_HPP_

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#ifndef __GNUC__
//#include <winsock2>
#include <winsock2.h>
#endif
#include <mysql/mysql.h>

namespace db
{
	class DBConnection : private boost::noncopyable
	{
	public:
		DBConnection();
		virtual ~DBConnection();

		bool connect(const std::string &dbHost, const std::string &username,
			const std::string &password, const std::string &dbName,
			unsigned int port = 0);

		inline bool isConnected(void) const { return isConnected_; }
		long affectedRows(void);
		void close(void);

		MYSQL* getMysqlRef(void) { return m_pMysql; }

		bool queryWithReconnectRetrial(const char* sql);

		bool tableExist(const std::string & name);

		boost::int64_t getRowsCount(const std::string& stem);

		void setReadTimeout(unsigned int value) { readTimeout_ = value; }
		void setWriteTimeout(unsigned int value) { writeTimeout_ = value; }
		void setConnectTimeout(unsigned int value) { connectTimeout_ = value; }

	protected:
		bool realConnect();

	protected:

		MYSQL* m_pMysql;

	private:
		bool isConnected_;
		std::string serverAddr_;
		unsigned int port_;
		std::string username_;
		std::string password_;
		std::string dbName_;


		unsigned int readTimeout_; //seconds
		unsigned int writeTimeout_;
		unsigned int connectTimeout_;
	};
}
#endif

