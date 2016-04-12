#include "httpd.h"

void usage(const char* proc)
{
	printf("port...%s\n",proc);
}

void print_log(const char* fun,const int line,const int error_no,const char* error)
{
	char *buf;
	sprintf(buf,"line:%d,errno:%d,error:%s",fun,line,error_no,error);
	syslog(LOG_NOTICE,buf);
}

void  set_sock_reuse_port(int listenfd)
{
	int flag = 1;
	if (setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag)) == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
	}
}

int start(short *port)
{
	int listen_sock=socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(1);
	}

	struct sockaddr_in local;
	bzero(&local,sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(*port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	socklen_t len = sizeof(local);
	int ret = bind(listen_sock,(struct sockaddr*)&local,len);
	if (ret == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(1);
	}

	ret = listen(listen_sock,5);
	if (ret == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(1);
	}

	set_sock_reuse_port(listen_sock);
	syslog(LOG_DEBUG,"start sock success");
	return listen_sock;
}

int get_line(int sockfd,char buf[],int buf_size)
{
	char c = '\0';
	int n = 0;
	int i=0;
	while(buf_size - 1>0 && c != '\n')
	{
		recv(sockfd,&c,1,NULL);
		if (c == '\r')
		{
			n = recv(sockfd,&c,1,MSG_PEEK);
			if (n > 0 && c == '\n')
			{
				recv(sockfd,&c,1,NULL);
			}
			else
				c = '\n';
		}
		buf[i] = c;
		buf_size--;
		i++;
	}
	buf[i] = '\0';
	return i;
}

void unimplement(int sock_client)
{
	char* buf = "not define action";
	if (send(sock_client,buf,sizeof(buf),0) == sizeof(buf))
	{
		syslog(LOG_DEBUG,"unimplement success");
	}
}

void accept_request(void* sock)
{

	int sock_client = (int)sock;
	int cgi = 0;
	char buf[BUFFER_SIZE];
	char method[255];
	char *query_string;
	char url[BUFFER_SIZE];
	char path[BUFFER_SIZE];
	memset(buf,'\0',sizeof(buf));
	memset(method,'\0',sizeof(method));
	memset(url,'\0',sizeof(url));
	memset(path,'\0',sizeof(path));
	int i = 0,j = 0;
	int numbers;
	struct stat st;
#ifdef _DEBUG_
	while(*buf == '\n');
	{
		numbers = get_line(sock_client,buf,sizeof(buf));
		printf("%s",buf);
	}
#endif
	numbers = get_line(sock_client,buf,sizeof(buf));
	while ( !isspace(buf[i]) && (j < sizeof(method) - 1))
	{

		method[j] = buf[i];
		i++,j++;
	
	method[j] = '\0';
	if(strcasecmp(method ,"GET") && strcasecmp(method ,"POST"))
	{
		unimplement(sock_client);
		return;
	}
	while(isspace(buf[i]))
	{
		i++;
	}
	j = 0;
	while( !isspace(buf[i]) && j < BUFFER_SIZE && i < BUFFER_SIZE-1)
	{
		url[j] = buf[i];
		i++,j++;
	}
	url[i] = '\0';
	if( !strcasecmp(method,"GET"))
	{	
		query_string = url;
		while(*query_string != '\0'&& *query_string != '?')
		{
			query_string++;
		}
		if (*query_string == '?')
			*query_string = '\0';
		query_string++;
		cgi = 0;
	}

	if ( !strcasecmp(method,"POST"))
	{
		cgi = 1;
	}
	
	sprintf(path,"htdocs%s",url);
	if (path[strlen(path)-1] == '/')
	{//if the path is a dir then return the static file
		strcat(path,"index.html");
	}

	if (stat(path,&st) == -1)
	{
		while ((numbers >0) && strcmp("\n",buf))
		{
			numbers = get_line(sock_client,buf,sizeof(buf));//this line is throwed the head information
		}
		not_found(sock_client);
	}
	else
	{
		if ((st.st_mode & S_IFMT) == S_IFDIR)//the path is a dir or a file
		{
			strcat(path,"index.html");
		}
		if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
		{
			cgi = 1;
		}

		if (cgi == 0)
		{
			send_file(sock_client,path,st.st_size);
			syslog(LOG_DEBUG,"accept_request success send a html");
		}
		else
		{
			execute_cgi(sock_client,path,method,query_string);
			syslog(LOG_DEBUG,"accept_request,success exec cgi program");
		}

	}
	close(sock_client);
}

void not_found(int sock_client)
{	
	char buf[1024];
	sprintf(buf,"HTTP/1.0 404 NOT FOUND so that\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"SERVER:EASYHTTPD\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"Content-Type: text/html\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"<HTML><TITLE>NOT FOUND</TITLE>\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"<BODY><P>The server could not fulfill\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"your request because the resource specified\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"is unavailable or nonexistent.\r\n");
	send(sock_client,buf,strlen(buf),0);
	sprintf(buf,"</BODY></HTML>\r\n");
	send(sock_client,buf,strlen(buf),0);
	syslog(LOG_DEBUG,"send not_found information success");
}

void erase_head(int sock_client)
{
	char buf[BUFFER_SIZE];
	int numbers = 1;
	while((numbers > 0) && (strcmp("\n",buf)))
	{
		numbers = get_line(sock_client,buf,sizeof(buf));
	}

}

void bad_request(int sock_client)
{
	char buf[BUFFER_SIZE];
	
	bzero(buf,sizeof(buf));
	sprintf(buf,"HTTP/1.0 400 BAD REQUEST\r\n");
	send(sock_client,buf,sizeof(buf),0);
	sprintf(buf,"CONTENT-Type: text/html\r\n");
	send(sock_client,buf,sizeof(buf),0);
	sprintf(buf,"\r\n");
	send(sock_client,buf,sizeof(buf),0);
	sprintf(buf,"<p>Your browser sent a bad request,");
	send(sock_client,buf,sizeof(buf),0);
	sprintf(buf,"such as a post without a content-Length.\r\n");
	send(sock_client,buf,sizeof(buf),0);
	syslog(LOG_DEBUG,"send bad_request information success");
}

void execute_cgi(int sock_client,char *path,char *method,char *query_string)
{
	int input_pipe[2]= {0,0};
	int output_pipe[2] = {0,0};
	int content_length = -1;
	int numbers = 0;
	char buf[BUFFER_SIZE];
	char c;
	int status = 0;	
	if (strcasecmp(method,"get") == 0)
	{
		erase_head(sock_client);
	}
	
	else//post
	{
		do {
			numbers = get_line(sock_client,buf,sizeof(buf));
			buf[15] = '\0';
			if (strcasecmp(buf,"Content-Length:") == 0)
			{
				content_length = atoi(&(buf[16]));
			}
		}while(strcmp("\n",buf) && numbers >0);
		if (content_length == -1)
		{
				bad_request(sock_client);
				return ;
		}

	}
	bzero(buf,sizeof(buf));
	sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n");//this is the key \r\n
	send(sock_client,buf,strlen(buf),0);

	int ret = pipe(input_pipe);
	if (ret == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(1);
	}
	ret = pipe(output_pipe);
	if (ret == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
	}

	int	id = fork();
	if (id == -1)
	{
		
		close(input_pipe[1]);
		close(output_pipe[0]);
		close(input_pipe[0]);
		close(output_pipe[1]);
		perror("fork");
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(1);
	}
	if (id > 0)//parents
	{
		close(input_pipe[0]);
		close(output_pipe[1]);
		if (strcasecmp(method,"post") == 0)
		{
			int i = 0;
			for (;i<content_length;i++)
			{
				recv(sock_client,&c,1,0);//recv message form client
				write(input_pipe[1],&c,1);//write it to pipe and the massage will transform to child,the message is query_string.
			}
		}
		int tmp = 0;
		while(read(output_pipe[0],&c,1) > 0)
		{
			tmp = send(sock_client,&c,1,0);//get the result and return to client.
			if (tmp == -1)
			{
				print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
			}
		}
		close(input_pipe[1]);
		close(output_pipe[0]);
		if (waitpid(id,&status,0) == -1)
		{
			syslog(LOG_NOTICE,"waitpid failed");
		}
	}
	else if (id == 0)//child
	{
		char meth_env[255];//because child want to exec so we need to reset the input and output 
		char query_env[255];
		char length_env[255];
		close(input_pipe[1]);
		close(output_pipe[0]);
		sprintf(meth_env,"REQUEST_METHOD=%s",method);
		int ret = putenv(meth_env);
		if (ret != 0)
		{
			print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		}
	
		sprintf(query_env,"QUERY_STRING=%s",query_string);
		ret = putenv(query_env);
		if (ret != 0)
		{
			print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		}
		sprintf(length_env,"CONTENT_LENGTH=%d",content_length);
		ret = putenv(length_env);
		if (ret != 0)
		{
			print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		}
		dup2(output_pipe[1],1);
		dup2(input_pipe[0],0);
		execl(path,path,NULL);
	}	
}

void send_file(int sock_client,char *path,int file_size)
{
	if (path == NULL)
		return;
	int fd = open(path,O_RDONLY);
	if (fd == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		return;
	}
	char head[BUFFER_SIZE];
	bzero(head,sizeof(head));
	strcat(head,"HTTP/1.0 200 OK \r\n\r\n");
	send(sock_client,head,strlen(head),0);

	if (sendfile(sock_client,fd,0,file_size) == -1)
	{
		print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
	}
	syslog(LOG_DEBUG,"send_file success");
}

int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		usage(argv[0]);
		exit(1);
	}
	
	
	openlog("httpd",LOG_PID,LOG_LOCAL5);
	int sock_client = -1;
	struct sockaddr_in client_address;
	socklen_t client_add_len = sizeof(client_address);
	int  port = atoi(argv[1]);
	int listen_sock = start(&port);

	pthread_t new_thread;
	int ret;
	while (1)
	{

		sock_client = accept(listen_sock,(struct sockaddr*)&client_address,&client_add_len);//if accept success then return a new descriptor ,the descriptor is the net connect the server and the client ,so we can get all of the message from this descriptor.
		if (sock_client == -1)
		{	
			print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
			continue;
		}
		ret = pthread_create(&new_thread,NULL,&accept_request,(void *)sock_client);
		if (ret != 0)
		{
			print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
			exit(1);
		}

	}
	return 0;
}
