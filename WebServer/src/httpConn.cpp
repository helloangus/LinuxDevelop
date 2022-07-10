#ifndef __HTTPCONN_H__
#define __HTTPCONN_H__

#include "httpConn.h"
#include "define.h"

int httpConn::m_epoll_fd = -1;
int httpConn::m_usr_count = 0;

httpConn :: httpConn()
{

}


httpConn :: ~httpConn()
{

}

void httpConn :: process()
{
    cout << "process" << endl;
    // 解析http请求

    // 生成响应
}

void httpConn :: init(int sockfd, const sockaddr_in& addr)
{
    m_sockfd = sockfd;
    m_address = addr;

    // 设置端口复用
    int reuse = 1;
    if(setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        cerr << "Error message: " << strerror(errno) << endl;
        exit(-1);
    }

    // 添加到epoll对象中
    addFd(httpConn :: m_epoll_fd, m_sockfd, true);
    ++m_usr_count;
}


bool httpConn :: read()
{
    return true;
}

bool httpConn :: write()
{
    return true;
}


void httpConn :: closeConn()
{
    // 关闭连接
    if(m_sockfd > 0)
    {
        delFd(httpConn :: m_epoll_fd, m_sockfd);
        m_sockfd = -1;
        --m_usr_count;
    }
}


void addFd(int epoll_fd, int fd, bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLHUP;
    if (one_shot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);

    // 设置非阻塞
    int setNonBlock(int);
    setNonBlock(fd);
}

// 从epoll对象中删除文件描述符
void delFd(int epoll_fd, int fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

// 修改epoll文件描述符对应的事件
void modFd(int epoll_fd, int fd, int ev)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLHUP;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
}

// 设置非阻塞
int setNonBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1)
    {
        cerr << "Error message: " << strerror(errno) << endl;
        return -1;
    }
    flags |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flags) == -1)
    {
        cerr << "Error message: " << strerror(errno) << endl;
        return -1;
    }
    return 0;
}

#endif