#include "comm_head.h"


void math_add(char *str)
{
	char *data1;
	char *data2;

	while (*str != '=')
		str++;
	str++;
	data1 = str;
	while(*str != '&')
	{
		str++;
	}
	*str++ = '\0';
	while(*str != '=')
		str++;
	str++;
	data2 = str;
//	if (isdigit(data1)== 0)
//	{
//		printf("<p>you enter a wrong parment</p>\n");
//		return;
//	}

	int num1 = atoi(data1);
	int num2 = atoi(data2);
	int result = num1 + num2;
	printf("<p> math result add is %d </p>\n",result);
}

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
	printf("<body>\n");
	printf("\t<n>method world</n>\t");
	printf("<p>method : %s<br>",method);

	tmp = getenv("REQUEST_METHOD");
	if (tmp == NULL)
		exit(1);
	strcpy(method,tmp);
	if (strcasecmp(method,"GET") == 0)
	{
		tmp = getenv("QUERY_STRING");
		if (tmp != NULL)
			strcpy(query_string,tmp);
		math_add(query_string);
	}
	else if (strcasecmp(method,"POST") == 0)
	{
		tmp = getenv("CONTENT_LENGTH");
		if (tmp == NULL)
			exit(1);
		content_length = atoi(tmp);
		int i = 0;
		for (;i < content_length;i++)
		{
			read(0,&post_data[i],1);
		}
		post_data[i] = '\0';
		math_add(post_data);
	}
	printf("</body>\n");
	printf("</html>\n");


}
