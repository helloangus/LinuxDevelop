#include <iostream>
using namespace std;


#include "define.h"


int main(int argc, char * argv[])
{
    cout << "Hello World!" << endl;

    if(argc <= 1)
    {
        cout << "Please run as follows: " << basename(argv[0]) << "port number" << endl;
        exit(-1);
    }

    // 获取端口号
    int port = atoi(argv[1]);

    // 对SIGPIPE信号进行处理
    addSig(SIGPIPE, SIG_IGN);

    // 创建线程池
    threadPool<httpConn> * pool = NULL;
    try
    {
        pool = new threadPool<httpConn>(MAX_THREAD_NUM, MAX_REQUESTS);
    }
    catch(const char * errmsg)
    {
        cerr << "Error message: " << errmsg << endl;
        exit(-1);
    }

    // 创建一个数组用于保存所有客户端信息
    httpConn * clients = new httpConn[MAX_FD];

    // 创建一个套接字用于监听客户端连接
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0)
    {
        cerr << "Error message: " << strerror(errno) << endl;
        exit(-1);
    }

    // 设置端口复用
    int reuse = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        cerr << "Error message: " << strerror(errno) << endl;
        exit(-1);
    }

    // 绑定
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Error message: " << strerror(errno) << endl;
        exit(-1);
    }

    // 监听
    if(listen(listen_fd, 5) < 0)
    {
        cerr << "Error message: " << strerror(errno) << endl;
        exit(-1);
    }

    // 创建一个epoll对象，事件数组，添加
    epoll_event events[MAX_EVENT_NUMBER];
    int epoll_fd = epoll_create(5);

    // 将监听的文件描述符添加到epoll对象中
    addFd(epoll_fd, listen_fd, false);
    httpConn :: m_epoll_fd = epoll_fd;

    // 循环监听
    while(true)
    {
        // 等待事件发生
        int number = epoll_wait(epoll_fd, events, MAX_EVENT_NUMBER, -1);
        if((number < 0) && (errno != EINTR))
        {
            cerr << "Error message: " << strerror(errno) << endl;
            break;
        }
        // 处理事件
        for(int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == listen_fd)
            {
                // 处理新连接
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int connfd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                if(connfd < 0)
                {
                    cerr << "Error message: " << strerror(errno) << endl;
                    continue;
                }
                if(httpConn :: m_usr_count >= MAX_FD)
                {
                    // 客户端连接数超过最大限制
                    // 给客户端发送错误信息
                    send(connfd, "Too many connections", 17, 0);
                    close(connfd);
                    continue;
                }
                
                // 将新连接的文件描述符添加到epoll对象中
                clients[connfd].init(connfd, client_addr);
            }
            else
            {
                // 处理已连接的客户端
                if(events[i].events & EPOLLIN)
                {
                    // 处理客户端发送的数据
                    if(!clients[sockfd].read())
                    {
                        // 关闭连接
                        close(sockfd);
                        clients[sockfd].closeConn();
                    }
                    else
                    {
                        // 交给工作线程处理
                        pool->append(clients + sockfd);
                    }
                    clients[sockfd].process();
                }
                else if(events[i].events & EPOLLOUT)
                {
                    // 处理客户端发送的数据
                    if(!clients[sockfd].write())
                    {
                        // 关闭连接
                        close(sockfd);
                        clients[sockfd].closeConn();
                    }
                    else
                    {
                        // 交给工作线程处理
                        pool->append(clients + sockfd);
                    }
                    
                }
                else if(events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
                {
                    // 处理客户端错误
                    clients[sockfd].closeConn();
                }
            }
        }
    }

    // 关闭epoll对象
    close(epoll_fd);
    // 关闭监听套接字
    close(listen_fd);
    // 释放资源
    delete [] clients;
    delete pool;
    
    return 0;
}

// 注册信号捕捉函数
void addSig(int signum, void (*func)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = func;
    sigfillset(&sa.sa_mask);
    sigaction(signum, &sa, NULL);
}