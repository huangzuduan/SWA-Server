#include "MysqlProtobufHelper.h"
#include "boost/lexical_cast.hpp"
#include "StringUtils.h"

namespace db
{

	using namespace std;
	using namespace google::protobuf;




	int doQuery(db::DBConnection &conn, const std::string &sql)
	{
		if (sql == "")
		{
			return 0;
		}

		if (!conn.isConnected()) {
			printf("not connected to mysql server \n");
			return 1;
		}

		//LOG(INFO) << "SQL: " << sql.c_str();
		if (!conn.queryWithReconnectRetrial(sql.c_str()))
		{
			printf("query db failed\n");
			return 1;
		}

		do
		{
			MYSQL_RES* result_ = mysql_store_result(conn.getMysqlRef());
			mysql_free_result(result_);
		} while (!mysql_next_result(conn.getMysqlRef())); // to solve the "2014:Commands out of sync; " problem

		return 0;
	}

	std::string generateInsertSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		const google::protobuf::Message & msg)
	{
		string sql = "INSERT INTO ";
		sql.append(tableName);
		string keys = "(";
		string values = "VALUES (";

		//	const Descriptor* msg_desc = msg.GetDescriptor();
		const Reflection * msg_reflect = msg.GetReflection();

		vector<const FieldDescriptor *> fields;
		msg_reflect->ListFields(msg, &fields);

		if (fields.size() == 0)
			return "";

		int first = 1;
		for (vector<const FieldDescriptor *>::iterator it = fields.begin();
			it != fields.end(); it++)
		{
			string data;
			stringstream ss;
			const string &key = (*it)->name();

			if (!first)
			{
				keys += ", ";
				values += ", ";
			}
			else
			{
				first = 0;
			}

			keys += "`" + key + "`";

			switch ((*it)->type())
			{
			case FieldDescriptor::TYPE_INT32:
				ss << msg_reflect->GetInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT32:
				ss << msg_reflect->GetUInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_INT64:
				ss << msg_reflect->GetInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT64:
				ss << msg_reflect->GetUInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_STRING:
				ss << msg_reflect->GetString(msg, *it);
				break;
			case FieldDescriptor::TYPE_ENUM:
				ss << (msg_reflect->GetEnum(msg, *it));
				break;
			case FieldDescriptor::TYPE_MESSAGE:
				msg_reflect->GetMessage(msg, *it).SerializeToString(&data);
				ss << data;
				break;
			default:
				printf("unkown field type : %d , field name: %s \n", (*it)->type(), (*it)->name().c_str());
				break;
			}

			string escapedStr = realEscape(mysql, ss.str());
			values += "\"" + escapedStr + "\"";
		}

		sql += keys + ") " + values + ");";

		return sql;
	}

	int doReplaceProto(db::DBConnection &conn,
		const std::string & tableName,
		const google::protobuf::Message & proto)
	{
		string table(tableName);
		string sql = generateReplaceSQLFromProto(conn.getMysqlRef(), table, proto);
		if (sql == "")
			return 2;

		if (!conn.queryWithReconnectRetrial(sql.c_str()))
		{
			return 3; // TODO: repalce by error code
		}

		do
		{
			MYSQL_RES* result_ = mysql_store_result(conn.getMysqlRef());
			mysql_free_result(result_);
		} while (!mysql_next_result(conn.getMysqlRef())); // to solve the "2014:Commands out of sync; " problem

		return 0;
	}

	int doInsertProto(db::DBConnection &conn,
		const std::string & tableName,
		const google::protobuf::Message & proto)
	{
		string table(tableName);
		string sql = generateInsertSQLFromProto(conn.getMysqlRef(), table, proto);
		if (sql == "")
			return 2;

		//LOG(INFO) << "SQL: " << sql.c_str();

		if (!conn.queryWithReconnectRetrial(sql.c_str()))
		{
			return 3; // TODO: repalce by error code
		}

		if (conn.affectedRows() == 1)
		{

			do
			{
				MYSQL_RES* result_ = mysql_store_result(conn.getMysqlRef());
				mysql_free_result(result_);
			} while (!mysql_next_result(conn.getMysqlRef())); // to solve the "2014:Commands out of sync; " problem

			return 0;
		}

		return 1;
	}

	//UPDATE t_account SET ban='1', regdate='123'  WHERE name='zf';

	std::string generateUpdateSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		int32_t charid,
		const google::protobuf::Message & msg)
	{
		string sql = "UPDATE " + tableName + " SET ";

		//	const Descriptor* msg_desc = msg.GetDescriptor();
		const Reflection * msg_reflect = msg.GetReflection();

		vector<const FieldDescriptor *> fields;
		msg_reflect->ListFields(msg, &fields);

		if (fields.size() == 0)
			return "";

		int first = 1;
		stringstream ss;
		string data;
		for (vector<const FieldDescriptor *>::iterator it = fields.begin();
			it != fields.end(); it++)
		{
			const string &key = (*it)->name();

			if (!first)
			{
				sql += ", ";
			}
			else
			{
				first = 0;
			}

			sql.append("`" + key + "`=");

			// TODO: below procedure should be optimized
			switch ((*it)->type())
			{
			case FieldDescriptor::TYPE_INT32:
				ss << msg_reflect->GetInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT32:
				ss << msg_reflect->GetUInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_INT64:
				ss << msg_reflect->GetInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT64:
				ss << msg_reflect->GetUInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_STRING:
				ss << msg_reflect->GetString(msg, *it);
				break;
			case FieldDescriptor::TYPE_ENUM:
				ss << (msg_reflect->GetEnum(msg, *it));
				break;
			case FieldDescriptor::TYPE_MESSAGE:
				msg_reflect->GetMessage(msg, *it).SerializeToString(&data);
				ss << data;
				data.clear();
				break;
			default:
				printf("unkown field type : %d \n", (*it)->type());
				break;
			}

			string escapedStr = realEscape(mysql, ss.str());
			ss.str(std::string());

			sql += "'" + escapedStr + "'";

		}

		sql += " WHERE `accid`='" + StringUtils::toString(charid) + "';";

		return sql;
	}

	std::string generateUpdateSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		const std::string & keyField,
		const std::string & keyValue,
		const google::protobuf::Message & msg)
	{
		string sql = "UPDATE " + tableName + " SET ";

		//	const Descriptor* msg_desc = msg.GetDescriptor();
		const Reflection * msg_reflect = msg.GetReflection();

		vector<const FieldDescriptor *> fields;
		msg_reflect->ListFields(msg, &fields);

		if (fields.size() == 0)
			return "";

		int first = 1;
		stringstream ss;
		string data;
		for (vector<const FieldDescriptor *>::iterator it = fields.begin();
			it != fields.end(); it++)
		{
			const string &key = (*it)->name();

			if (!first)
			{
				sql += ", ";
			}
			else
			{
				first = 0;
			}

			sql.append("`" + key + "`=");

			// TODO: below procedure should be optimized
			switch ((*it)->type())
			{
			case FieldDescriptor::TYPE_INT32:
				ss << msg_reflect->GetInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT32:
				ss << msg_reflect->GetUInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_INT64:
				ss << msg_reflect->GetInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT64:
				ss << msg_reflect->GetUInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_STRING:
				ss << msg_reflect->GetString(msg, *it);
				break;
			case FieldDescriptor::TYPE_ENUM:
				ss << (msg_reflect->GetEnum(msg, *it));
				break;
			case FieldDescriptor::TYPE_MESSAGE:
				msg_reflect->GetMessage(msg, *it).SerializeToString(&data);
				ss << data;
				data.clear();
				break;
			default:
				printf("unkown field type : %d \n", (*it)->type());
				break;
			}

			string escapedStr = realEscape(mysql, ss.str());
			ss.str(std::string());

			sql += "'" + escapedStr + "'";

		}

		sql += " WHERE `" + keyField + "`='" + keyValue + "';";

		return sql;
	}

	std::string generateReplaceSQLFromProto(MYSQL *mysql,
		const std::string & tableName,
		const google::protobuf::Message & msg)
	{
		string sql = "REPLACE INTO ";
		sql.append(tableName);
		string keys = "(";
		string values = "VALUES (";

		//	const Descriptor* msg_desc = msg.GetDescriptor();
		const Reflection * msg_reflect = msg.GetReflection();

		vector<const FieldDescriptor *> fields;
		msg_reflect->ListFields(msg, &fields);

		if (fields.size() == 0)
			return "";

		int first = 1;
		for (vector<const FieldDescriptor *>::iterator it = fields.begin();
			it != fields.end(); it++)
		{
			string data;
			stringstream ss;
			const string &key = (*it)->name();

			if (!first)
			{
				keys += ", ";
				values += ", ";
			}
			else
			{
				first = 0;
			}

			keys += "`" + key + "`";

			switch ((*it)->type())
			{
			case FieldDescriptor::TYPE_DOUBLE:
				ss << msg_reflect->GetDouble(msg, *it);
				break;
			case FieldDescriptor::TYPE_INT32:
				ss << msg_reflect->GetInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT32:
				ss << msg_reflect->GetUInt32(msg, *it);
				break;
			case FieldDescriptor::TYPE_INT64:
				ss << msg_reflect->GetInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_UINT64:
				ss << msg_reflect->GetUInt64(msg, *it);
				break;
			case FieldDescriptor::TYPE_STRING:
				ss << msg_reflect->GetString(msg, *it);
				break;
			case FieldDescriptor::TYPE_ENUM:
				ss << (msg_reflect->GetEnum(msg, *it));
				break;
			case FieldDescriptor::TYPE_MESSAGE:
				msg_reflect->GetMessage(msg, *it).SerializeToString(&data);
				ss << data;
				break;
			default:
				printf("unkown field type :%d , field name:%s \n", (*it)->type(), (*it)->name().c_str());
				break;
			}

			string escapedStr = realEscape(mysql, ss.str());
			values += "\"" + escapedStr + "\"";
		}

		sql += keys + ") " + values + ");";

		return sql;

	}



	int mysqlRow2Protobuf(MYSQL_ROW & row, int numFields,
		MYSQL_FIELD *fields, google::protobuf::Message * msg)
	{

		using namespace google::protobuf;

		const google::protobuf::Descriptor * msg_desc = msg->GetDescriptor();
		const google::protobuf::Reflection * msg_reflect = msg->GetReflection();

		const google::protobuf::FieldDescriptor* field_desc = NULL;
		Message * m = NULL;
		for (int i = 0; i < numFields; i++)
		{
			//		LOG4CXX_DEBUG(gFogsLogger,
			//				"Field " << i << ",name: " << fields[i].name <<
			//				", type: " << fields[i].type << ", length: " << fields[i].length);

			std::string name(fields[i].name);
			field_desc = msg_desc->FindFieldByName(name);
			if (field_desc == NULL)
			{
				if (name != "accid")
				{
					printf("protobuf and db field name dismatch :%s \n ", name.c_str());
				}
				continue;
			}

			if (row[i] == NULL)
			{
				printf("db field:%s return NULL \n", name.c_str());
				continue;
			}

			switch (field_desc->type())
			{
			case google::protobuf::FieldDescriptor::TYPE_INT32:
			{
				int32_t value = 0;
				if (!StringUtils::toInt32(row[i], value))
				{
					msg_reflect->SetInt32(msg, field_desc, value);
				}
				break;
			}

			case google::protobuf::FieldDescriptor::TYPE_UINT32:
			{
				uint32_t value = 0;
				if (!StringUtils::toUInt32(row[i], value))
				{
					msg_reflect->SetUInt32(msg, field_desc, value);
				}
				break;
			}

			case google::protobuf::FieldDescriptor::TYPE_INT64:
			{
				int64_t value = 0;
				if (!StringUtils::toInt64(row[i], value))
				{
					msg_reflect->SetInt64(msg, field_desc, value);
				}
				break;
			}

			case google::protobuf::FieldDescriptor::TYPE_UINT64:
			{
				uint64_t value = 0;
				if (!StringUtils::toUInt64(row[i], value))
				{
					msg_reflect->SetUInt64(msg, field_desc, value);
				}
				break;
			}

			case google::protobuf::FieldDescriptor::TYPE_STRING:
				msg_reflect->SetString(msg, field_desc, std::string(row[i]));
				break;
			case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
				if (row[i] != NULL)
				{
					m = msg_reflect->MutableMessage(msg, field_desc);
					m->ParseFromArray(row[i], fields[i].length);
					//LOG4CXX_INFO(gFogsLogger,
					//	"parse a message from database " <<
					//	m->DebugString());
				}
				break;
			default:
				printf("WARNING: protobuf and db field type dismatch : %d \n", fields[i].type);
				break;
			}
		}

		return 0;
	}

}


