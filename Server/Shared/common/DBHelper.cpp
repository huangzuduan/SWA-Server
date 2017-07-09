#include "DBHelper.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <boost/shared_ptr.hpp>

std::string realEscape(MYSQL *mysql, const std::string& str)
{
	if (str.empty())
	{
		return str;
	}

	size_t length = str.length();
	boost::shared_ptr<char> escaped((char*)std::calloc(1, length * 2 + 1),
		std::free);
	mysql_real_escape_string(mysql, escaped.get(), str.c_str(), length);

	return std::string(escaped.get());
}



bool checkTableExistInDatabase(MYSQL &mysql, const std::string & dbName,
	const std::string & tableName)
{
	std::stringstream sqlStream;
	//	sqlStream << "select * from information_schema.tables where TABLE_SCHEMA='" <<
	//			dbName << "' and TABLE_NAME='" << tableName << "'";

	sqlStream << "select * from information_schema.tables where TABLE_SCHEMA='" <<
		dbName << "';";


	std::string sql = sqlStream.str();
	if (mysql_real_query(&mysql, sql.c_str(), strlen(sql.c_str())) != 0)
	{
		printf("Check table %s exist in database %s  failed, errno %s, error message %s , SQL:%s \n", tableName.c_str() , dbName.c_str(), mysql_errno(&mysql) , mysql_error(&mysql),sql);
		return false;
	}

	boost::shared_ptr<MYSQL_RES> resultPtr(mysql_use_result(&mysql),
		mysql_free_result);
	if (!resultPtr)
	{
		printf("Check table %s exist in database %s  failed, errno %s , error message %s, SQL: %s \n" , tableName.c_str(), dbName.c_str(), mysql_errno(&mysql) , mysql_error(&mysql) ,sql);
		return false;
	}

	if (mysql_num_rows(resultPtr.get()) == 0)
	{
		printf("Table %s  doesn't exist in database %s , SQL : %s \n", tableName.c_str(), dbName.c_str(), sql.c_str());
		return false;
	}

	return true;

}
