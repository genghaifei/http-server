#include "comm_head.h"
#include "sql_connect.h"
#include <string.h>

const std::string _host("192.168.189.129");
const std::string _usr("root");
const std::string _passwd("");
const std::string _db("client_comm");

void reg(char *str)
{
	sql_connector comm(_host,_usr,_passwd,_db);
	comm.begin_connect();
	char name[255];
	char age[10];
	char school[255];
	char hobby[255];
	char id[10];
	char *tmp = str;
	while(*str != '\0')
	{
		if (*str == '=' || *str == '&')
		{
			*str = ' ';
		}
		str++;
	}
	sscanf(tmp,"%*s %s %*s %s %*s %s %*s %s",name,age,school,hobby);
	std::string data;
	data += "'";
	data += name;
	data += "',";
	data += age;
	data += ",'";
	data += school;
	data += "','";
	data += hobby;
	data += "'";
	std::cout<<data<<std::endl;
	comm.sql_insert(data);

}


int main()
{
	
	int content_length = -1;
	char method[COMM_SIZE];
	char query_string[COMM_SIZE];
	char post_data[4*COMM_SIZE];

	bzero(method,sizeof(method));
	bzero(query_string,sizeof(query_string));
	bzero(post_data,sizeof(post_data));

	printf("<html>");
	printf("<body>");
	printf("<n>register case </n>");
	char *tmp;
	tmp = getenv("REQUEST_METHOD");
	if (tmp == NULL)
		exit(1);
	strcpy(method,tmp);
	if (strcasecmp(method,"GET") == 0)
	{
		tmp = getenv("QUERY_STRING");
		if (tmp != NULL)
			strcpy(query_string,tmp);
		reg(query_string);
	}
	else if (strcasecmp(method,"POST") == 0)
	{
		tmp = getenv("CONTENT_LENGTH");
		if (tmp == NULL)
			exit(1);
		content_length = atoi(tmp);
		int i = 0;
		for (;i<content_length;i++)
		{
			read(0,&post_data[i],1);
		}
		post_data[i] = '\0';
		reg(post_data);
	}
	printf("</body>");
	printf("</html>");
}
