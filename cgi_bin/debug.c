#include "comm_head.h"

int main()
{
	int content_length = -1;
	char method[COMM_SIZE];
	char query_string[COMM_SIZE];
	char post_data[4*COMM_SIZE];
	char *tmp;
	bzero(method,sizeof(method));
	bzero(query_string,sizeof(query_string));
	bzero(post_data,sizeof(post_data));

	printf("<html>\n");
	printf("\t<n>hello world</n>\t");
	printf("<p> method: %s<br>",method);
	tmp = getenv("REQUEST_METHOD");
	printf("test   !!!!!!!!!!!!!%s\n",tmp);
	if (tmp == NULL)
		exit(1);
	strcpy(method,tmp);
	if (strcasecmp(method,"GET") == 0)
	{
		tmp = getenv("QUERY_STRING");
		if (tmp != NULL)
			strcpy(query_string,getenv("QUERY_STRING"));
		printf("<p>query_string : %s</p>",query_string);
	}
	else if (strcasecmp(method,"POST") == 0)
	{
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0;
		for (;i<content_length;i++)
		{
			read(0,&post_data[i],1);
		}
		post_data[i] = '\0';
		printf("<p>post_data :%s</p>",post_data);
	}
	
	printf("</html>");
}
