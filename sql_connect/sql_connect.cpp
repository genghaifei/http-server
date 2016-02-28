#include "sql_connect.h"

sql_connector::sql_connector(const std::string &_host,\
		const std::string &_usr,\
		const std::string &_passwd,\
		const std::string &_db)
{
	this->host = _host;
	this->usr = _usr;
	this->passwd = _passwd;
	this->db = _db;
	this->mysql_conn = mysql_init(NULL);
	this->res = NULL;
};

sql_connector::~sql_connector()
{
	if (res != NULL)
		free(res);
	if (mysql_conn)
		close_connect();
};

bool sql_connector::begin_connect()
{
	if (mysql_real_connect(mysql_conn,host.c_str(),usr.c_str(),passwd.c_str(),db.c_str(),3306,NULL,0) == NULL)
	{
		std::cerr<<"connect error"<<std::endl;
		return false;
	}
	else
	{
	//	std::cout<<"connect done"<<std::endl;
		return true;
	}
}

bool sql_connector::close_connect()
{
	mysql_close(mysql_conn);
	std::cout<<"connect close..."<<std::endl;

}

bool sql_connector::sql_select(std::string field_name[],std::string _out_data[1024][5],int &_out_row,std::string sql)
{
	if (mysql_query(mysql_conn,sql.c_str()) == 0)
	{
	//	std::cout<<"query success"<<std::endl;
	}
	else
	{
		std::cerr<<"query failed"<<std::endl;
		return false;
	}
	res = mysql_store_result(mysql_conn);
	int row_num = mysql_num_rows(res);
	int field_num = mysql_num_fields(res);
	_out_row = field_num;

	MYSQL_FIELD *fd = NULL;
	int i = 0;
	for(;fd = mysql_fetch_field(res);)
		field_name[i++] = fd->name;
	for(i = 0;i<row_num;i++)
	{
		MYSQL_ROW _row = mysql_fetch_row(res);
		if(_row)
		{
			int start = 0;
			for(;start<field_num;start++)
			{
				_out_data[i][start] = _row[start];
			}
		}
	}
	return true;
}



bool sql_connector::sql_insert(const std::string &data)
{
	std::string sql = "INSERT INTO student_5_class(name,age,school,hobby) values";
	sql += "(";
	sql += data;
	sql+= ");";
	if (mysql_query(mysql_conn,sql.c_str()) == 0)
	{
		std::cout<<"query success"<<std::endl;
		return true;
	}
	else
	{
		std::cerr<<"query failed"<<std::endl;
		std::cout<<mysql_error(mysql_conn)<<std::endl;
		return false;
	}
}



void sql_connector::show_info()
{
	std::cout<<mysql_get_client_info()<<std::endl;

};

//int main()
//{
//	const std::string _host = "192.168.189.129";
//	const std::string _usr = "root";
//	const std::string _passwd = "";
//	const std::string _db = "client_comm";
//	const std::string data = "'s6',3,'xgong','seep'";
//	sql_connector comm(_host,_usr,_passwd,_db);
//	comm.begin_connect();
//	comm.sql_insert(data);
////	comm.show_info();
//
//	std::string field_name[5];
//	std::string _out_data[1024][5];
//	int row;
//	std::string sql = "select * from student_5_class";
//	comm.sql_select(field_name,_out_data,row,sql);
//	for (int i = 0;i<row;i++)
//	{
//		std::cout<<"\t"<<field_name[i];
//	}
//	std::cout<<std::endl;
//	for (int i = 0;i<row;i++)
//	{
//		for (int j = 0;j< 5;j++)
//		{
//			std::cout<<"\t"<<_out_data[i][j];
//		}
//		std::cout<<std::endl;
//	}
//	return 0;
//}
