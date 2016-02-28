#include "demo_client.h"

void usage(char *str)
{
	printf("%s,ip address ,port number.",str);
}

int main(int argc,char* argv[])
{
	if (argc <= 2)
	{
		usage(argv[0]);
		exit(1);
	}

	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	if (listenfd < 0)
	{
		perror("socket");
		exit(1);
	}
	struct sockaddr_in client;
	bzero(&client,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(atoi(argv[2]));
	client.sin_addr.s_addr = inet_addr(argv[1]);
	socklen_t client_len = sizeof(client);

	int ret = connect(listenfd,(struct sockaddr*)&client,client_len);
	if (ret < 0)
	{
		perror("connect");
		exit(1);
	}
	close(listenfd);

	return 0;
}
