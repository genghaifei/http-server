#ifndef _HTTPD_H_
#define _HTTPD_H_
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/syslog.h>
#include <sys/epoll.h>

struct arg
{
	int sock_client;
	char *path;
	char *method;
	char *query_string;
};
struct all_fd
{
	int listen_sock;
	int epollfd;
};
#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024
#define _BACK_LOG_ 5
#endif
