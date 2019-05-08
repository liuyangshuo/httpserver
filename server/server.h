#ifndef _SERVER_H_
#define _SERVER_H_
#include "config.h"
#include <epoll.h>

#define CONF_FILE "server.conf"
#define CONF_PATH "../config"
#define MAX_BACKLOG 100
#define MAX_EVENTS 1024

typedef struct _epollfd_connfd
{
    int epollfd;
    int connfd;
}_epollfd_connfd;

#endif
