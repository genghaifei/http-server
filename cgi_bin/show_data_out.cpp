#include "comm_head.h"
#include "sql_connect.h"

const std::string _host = "192.168.189.129";
const std::string _usr = "root";
const std::string _passwd = "";
const std::string _db = "client_comm";

void show_sql(char *sql)
{
	sql_connector conn(_host,_usr,_passwd,_db);
	conn.begin_connect();
	std::string field_name[10];
	std::string _out_data[1024][5];
	int row;
	conn.sql_select(field_name,_out_data,row,sql);

	std::cout<<"<table border=\"1\">"<<std::endl;
	std::cout<<"<tr>"<<std::endl;
	for (int i = 0;i<row;i++)
	{
		std::cout<<"<th>"<<field_name[i]<<"</th>"<<std::endl;
	}	

	for (int i = 0;i<row;i++)
	{
		std::cout<<"<tr>"<<std::endl;
		for(int j =0;j<5;j++)
			std::cout<<"<td>"<<_out_data[i][j]<<"</td>"<<std::endl;
	std::cout<<"</tr>"<<std::endl;
	}
	
	std::cout<<"</body>"<<"</html>"<<std::endl;
	
}


int main()
{
	int content_length = -1;
	char query_string[COMM_SIZE];
	char method[COMM_SIZE];
	char post_data[4*COMM_SIZE];
	char *default_sql = "SELECT * FROM student_5_class";

	bzero(query_string,sizeof(query_string));
	bzero(method,sizeof(method));
	bzero(post_data,sizeof(post_data));

	printf("<html>");
	printf("<body>");
	char *tmp = getenv("REQUEST_METHOD");
	if (tmp != NULL)
		strcpy(method,tmp);
	printf("<n>the method is %s</n>",method);
	if (strcasecmp(method,"GET") == 0)
	{
		tmp = getenv("QUERY_STRING");
		if (tmp != NULL)
			strcpy(query_string,tmp);
		char *sql = query_string;
		while(*sql != '=')
			sql++;
		sql++;
		if (*sql == '\0')
			sql = default_sql;
		show_sql(sql);
	}
	if (strcasecmp(method,"POST") == 0)
	{
		tmp = getenv("CONTENT_LENGTH");
		if (tmp != NULL)
		{
			content_length = atoi(tmp);
		}
		if (content_length != -1)
		{
			int i = 0;
			for (;i<content_length;i++)
			{
				read(0,&post_data[i],1);
			}
			post_data[i] = '\0';
		}
		char *sql = post_data;
		while(*sql != '=')
			sql++;
		sql++;
		if (*sql == '\0')
			sql = default_sql;
		show_sql(sql);
	}
	return 0;

}
