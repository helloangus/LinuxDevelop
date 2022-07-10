#ifndef __HTTPCONNECTION__
#define __HTTPCONNECTION__

#include "define.h"

class httpConn
{
    public:
        static int m_epoll_fd;  // 所有的socket上的事件都被注册到同一个epoll对象中
        static int m_usr_count; // 当前连接的用户数量

        httpConn();
        ~httpConn();

        // 初始化httpConn对象
        void init(int sockfd, const sockaddr_in& addr);

        // 非阻塞的读取数据
        bool read();
        // 非阻塞的写入数据
        bool write();

        // 由线程池中的工作线程处理客户端请求的入口函数
        void process();

        // 关闭连接
        void closeConn();

    private:
        int m_sockfd;   // 客户端socket描述符
        sockaddr_in m_address; // 客户端地址信息


};



#endif