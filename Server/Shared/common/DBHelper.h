#ifndef DBINTERFACE_HPP_
#define DBINTERFACE_HPP_

#include "DBConnection.h"


std::string realEscape(MYSQL *mysql, const std::string& str);

bool checkTableExistInDatabase(MYSQL &mysql, const std::string & dbName,
	const std::string & tableName);

#endif /* DBINTERFACE_HPP_ */
