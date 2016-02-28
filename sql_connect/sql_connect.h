#ifndef _SQL_CONNECT_H_
#define _SQL_CONNECT_H_

#include <iostream>
#include <mysql.h>


class sql_connector
{

public:
	sql_connector(const std::string &_host,const std::string &_usr,const std::string &_passwd,const std::string &_db);
	~sql_connector();
	bool sql_insert(const std::string &);
	bool updata_sql();
	bool delete_sql();
	bool delete_table();
	bool sql_select(std::string _field_name[],std::string out_data[1024][5],int &out_row,std::string );
	bool begin_connect();
	bool close_connect();
	void show_info();
private:
	MYSQL_RES *res;
	MYSQL *mysql_conn;
	std::string host;
	std::string usr;
	std::string passwd;
	std::string db;
};


#endif
