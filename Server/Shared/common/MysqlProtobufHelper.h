#ifndef MYSQLPROTOBUFHELPER_HPP_
#define MYSQLPROTOBUFHELPER_HPP_

#include "DBHelper.h"

#include "DBConnection.h"

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include "StringUtils.h"


namespace db
{

	template <typename T>
	static int mysqlRecord2Protobuf(MYSQL* mysql, T &data)
	{
		boost::shared_ptr<MYSQL_RES> resultPtr(mysql_use_result(mysql),
			mysql_free_result);
		if (!resultPtr) {
			printf("error in mysql connection:%s \n", mysql_error(mysql));
			return 1;
		}

		MYSQL_FIELD *fields = mysql_fetch_fields(resultPtr.get());
		unsigned int numFields = mysql_num_fields(resultPtr.get());
		MYSQL_ROW row;
		unsigned long *lengths;

		//LOG(INFO) << "Fields num " << numFields;

	//		using namespace google::protobuf;

		const google::protobuf::Descriptor * msg_desc = data.GetDescriptor();
		const google::protobuf::FieldDescriptor* field_desc = NULL;
		google::protobuf::Message * m = NULL;

		while ((row = mysql_fetch_row(resultPtr.get())))
		{
			lengths = mysql_fetch_lengths(resultPtr.get());

			for (unsigned int i = 0; i < numFields; i++)
			{
				/*LOG(INFO) <<
						"Field " << i << ",name: " << fields[i].name <<
						", type: " << fields[i].type;*/

				std::string name(fields[i].name);
				field_desc = msg_desc->FindFieldByName(name);
				if (field_desc == NULL)
				{
					printf("protobuf and db field name dismatch : %s\n ", fields[i].name);
					continue;
				}

				if (row[i] == NULL)
				{
					printf("db field: %s is NULL \n", name.c_str());
					continue;
				}

				const google::protobuf::Reflection * reflect = data.GetReflection();
				switch (field_desc->type())
				{
				case google::protobuf::FieldDescriptor::TYPE_INT32:
				{
					int32_t value = 0;
					if (!StringUtils::toInt32(row[i], value))
					{
						reflect->SetInt32(&data, field_desc, value);
					}
					break;
				}

				case google::protobuf::FieldDescriptor::TYPE_UINT32:
				{
					uint32_t value = 0;
					if (!StringUtils::toUInt32(row[i], value))
					{
						reflect->SetUInt32(&data, field_desc, value);
					}
					break;
				}

				case google::protobuf::FieldDescriptor::TYPE_INT64:
				{
					int64_t value = 0;
					if (!StringUtils::toInt64(row[i], value))
					{
						reflect->SetInt64(&data, field_desc, value);
					}
					break;
				}

				case google::protobuf::FieldDescriptor::TYPE_UINT64:
				{
					uint64_t value = 0;
					if (!StringUtils::toUInt64(row[i], value))
					{
						reflect->SetUInt64(&data, field_desc, value);
					}
					break;
				}

				case google::protobuf::FieldDescriptor::TYPE_STRING:
					reflect->SetString(&data, field_desc, std::string(row[i]));
					break;

				case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
					if (row[i] != NULL)
					{
						m = reflect->MutableMessage(&data, field_desc);
						m->ParseFromArray(row[i], fields[i].length);
					}
					else
					{
						printf("fields:%s  is null \n", fields[i].name);
					}
					break;
				default:
					printf("protobuf and db field type dismatch :%d \n", fields[i].type);
					return 1;
				}

			}
		}

		return 0;

	}

	template <typename T>
	int doQueryProto(db::DBConnection &conn, const std::string &sql, T & pb)
	{
		if (!conn.isConnected()) {
			printf("not connected to mysql server \n");
			return 1;
		}

		//LOG(INFO) << "SQL: " << sql.c_str();
		if (!conn.queryWithReconnectRetrial(sql.c_str()))
		{
			printf("query db failed \n");
			return 1;
		}

		int ret = mysqlRecord2Protobuf(conn.getMysqlRef(), pb);
		if (ret)
		{
			printf("convert record failed \n");
			return ret;
		}

		return 0;
	}

	template <typename T>
	int doQueryRepeatedProto(db::DBConnection &conn, const std::string &sql, T&  pb)
	{
		if (!conn.isConnected()) {
			printf("not connected to mysql server \n");
			return 1;
		}

		//LOG(INFO) << "SQL: " << sql.c_str();
		if (!conn.queryWithReconnectRetrial(sql.c_str()))
		{
			printf("query db failed \n");
			return 1;
		}

		//int ret = mysqlRecord2Protobuf(conn.getMysqlRef(), pb);
		int ret = mysqlRecords2RepeatedPtrField(conn.getMysqlRef(), pb);
		if (ret)
		{
			printf("convert record failed \n");
			return ret;
		}

		return 0;
	}


	int doQuery(db::DBConnection &conn, const std::string &sql);

	std::string generateInsertSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		const google::protobuf::Message & msg);

	std::string generateUpdateSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		int32_t charid,
		const google::protobuf::Message & msg);

	std::string generateUpdateSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		const std::string & keyField,
		const std::string & keyValue,
		const google::protobuf::Message & msg);

	std::string generateReplaceSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		const google::protobuf::Message & msg);


	int doInsertProto(db::DBConnection &conn,
		const std::string & tableName,
		const google::protobuf::Message & proto);

	int doReplaceProto(db::DBConnection &conn,
		const std::string & tableName,
		const google::protobuf::Message & proto);


	int mysqlRow2Protobuf(MYSQL_ROW & row, int numFields,
		MYSQL_FIELD *fields, google::protobuf::Message * msg);



	template <typename T>
	int mysqlRecords2RepeatedPtrField(MYSQL* mysql,
		T & repeatedPtr)
	{
		boost::shared_ptr<MYSQL_RES> resultPtr(mysql_use_result(mysql),
			mysql_free_result);

		if (!resultPtr)
		{
			printf("error in mysql connection: %s \n", mysql_error(mysql));
			return 1;
		}

		MYSQL_FIELD *fields = mysql_fetch_fields(resultPtr.get());
		unsigned int numFields = mysql_num_fields(resultPtr.get());
		MYSQL_ROW row;
		unsigned long *lengths;

		//	LOG4CXX_DEBUG(fogs::log::gFogsLogger,
		//			"Fields num " << numFields);

		while ((row = mysql_fetch_row(resultPtr.get())))
		{
			if (mysqlRow2Protobuf(row, numFields, fields, repeatedPtr.Add()))
			{
				// TODO
				printf("WARNING \n");
			}
		}

		return 0;
	}


	template <typename T>
	int queryRepeatedPtrField(db::DBConnection &conn, const std::string & sql,
		T * repeatedPtr)
	{
		int ret = doQuery(conn, sql);
		if (ret)
		{
			return ret;
		}

		ret = mysqlRecords2RepeatedPtrField(conn.getMysqlRef(), repeatedPtr);
		if (ret)
		{
			printf("convert record failed \n");
			return ret;
		}

		return 0;
	}


}

#endif /* MYSQLPROTOBUFHELPER_HPP_ */
