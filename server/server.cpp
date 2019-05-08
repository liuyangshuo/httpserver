#include "server.h"

using namespace std;

int my_socket()
{
    int socket;
    if (socket(AF_INET, SOCK_STREAM, 0) == -1)
    {
        perror("get socket failed!");
        exit(EXIT_FAILURE)
    }
    return socket;
}

void set_nonblocking(int listen_fd)
{
    int flags = fcntl(listen_fd, F_GETFL, 0);
    if (flags < 0)
    {
        perror("fcntl: F_GETFL");
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    int ret = fcntl(listen_Fd, F_SETFL, flags);
    if (ret < 0)
    {
        perror("fcntl: F_SETFL");
        exit(EXIT_FAILURE);
    }
}

void set_addr_reuse_off(int fd)
{
    int off = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        perror("setsockopt: SO_REUSEADDR off");
        exit(EXIT_FAILURE);
    }
}

void *thread_func(void *param)
{
    
}

int main()
{
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct servent *pservent;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    int epoll_fd;
    int nfds;
    int conn_sock;
    _epollfd_connfd epollfd_connfd;
    socklen_t addrlen;
    pthread_t tid;
    pthread_attr_t pthread_attr_detach;

    conf_init(CONF_PATH, CONF_FILE);

    int listen_fd = my_socket();
    
    //设置监听套接字为非阻塞模式
    set_nonblocking(listen_fd);
    //关闭套接字的地址复用
    set_reuse(listen_fd);
    pservent = getservbyname("http","tcp");
    server_addr.sin_family = AF_INET;
    //server_addr.sin_port = pservent->s_port;
    server_addr.sin_port = htos(80);
    server_addr.sin_addr.s_addr = htol(INADDR_ANY);

    if (bind(listen_fd, (struct socketaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind failed!");
        exit(EXIT_FAILURE);
    }

    if (listen(listen_fd, MAX_BACKLOG) == -1)
    {
        perror("listen failed!");
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_creat(MAX_EVENTS);
    ev.events = EPOLLIN;//可读事件
    ev.data.fd = listen_fd;
    //将监听事件加入epoll中
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    pthread_attr_init(&pthread_attr_detach);
    pthread_attr_setdetachstate(&pthread_attr_detach, PTHREAD_CREATE_DETACHED);

    for (;;)
    {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            continue;
        }
        for (int n = 0; n < nfds; ++n)
        {
            //处理监听套接字出发的事件
            if (events[n].data.fd == listen_fd)
            {
                conn_sock = accept(listen_fd, (struct socketaddr*)&client_addr, &addrlen);
                set_nonblocking(conn_sock);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_sock, &ev);
            }
            else
            {
                epollfd_connfd.epollfd = epollfd;
                epollfd_connfd.connfd = events[n].data.fd;
                //epoll不再监听这个客户端套接字
                epoll_ctl(epollfd, EPOLL_CTL_DEL, conn_sock, &ev);
                pthread_create(&tid, &pthread_attr_detach, &thread_func, (void*)&epollfd_connfd);
            }
        }
    }
    pthread_attr_destroy(&pthread_attr_detach);
    close(listen_fd);

    return 0;
}
