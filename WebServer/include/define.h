#ifndef __DEFINE__
#define __DEFINE__

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <exception>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "threadPool.h"
#include "httpConn.h"

#define MAX_FD 65535
#define MAX_EVENT_NUMBER 1024
#define MAX_THREAD_NUM 10
#define MAX_REQUESTS 1000

// 添加信号捕捉函数
void addSig(int signum, void (*func)(int));

// 添加文件描述符到epoll对象中
extern void addFd(int epoll_fd, int fd, bool one_shot);
// 从epoll对象中删除文件描述符
extern void delFd(int epoll_fd, int fd);
// 修改epoll文件描述符对应的事件，重置socket上one_shot标志，确保下一次可读时，epollin事件能再次触发
extern void modFd(int epoll_fd, int fd, int ev);


#endif