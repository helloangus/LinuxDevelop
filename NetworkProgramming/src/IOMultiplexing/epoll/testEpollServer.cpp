#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>

#define IP_addr "127.0.0.1"
#define Port 9999
#define MaxClient 128


int main()
{
    cout << "Server running..." << endl;
    // 1、创建监听套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 2、绑定监听套接字

    // 设置端口复用
    int sock_opt = 1;
    int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &sock_opt, sizeof(sock_opt));
    if(ret == -1)
    {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in lsa;
    inet_pton(AF_INET, IP_addr, &lsa.sin_addr.s_addr);
    lsa.sin_family = AF_INET;
    lsa.sin_port = htons(Port);
    socklen_t lsa_len =sizeof(lsa);
    ret = bind(lfd, (struct sockaddr *)&lsa, lsa_len);
    if(ret == -1)
    {
        perror("bind");
        return -1;
    }


    // 3、设置监听
    ret = listen(lfd, 8);
    if(ret == -1)
    {
        perror("listen");
        return -1;
    }


    // 4、设置IO多路复用
    // 创建epoll实例
    int epfd = epoll_create(1);
    if(epfd == -1)
    {
        perror("epoll_create");
        return -1;
    }

    // 将监听的文件描述符加入epoll实例中
    struct epoll_event lfd_epev;
    lfd_epev.events = EPOLLIN;
    lfd_epev.data.fd = lfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &lfd_epev);

    struct epoll_event epevs[BUFSIZ];

    while(true)
    {
        // 调用epoll_wait函数，让内核检测哪些文件描述符有数据
        int ret = epoll_wait(epfd, epevs, BUFSIZ, -1);
        if(ret == -1)
        {
            perror("epoll_wait");
            return -1;
        }
        else if(ret == 0)
        {
            continue; 
        }
        // 检测到的变化的文件描述符大于0
        else if(ret > 0)
        {
            // 遍历返回的epevs
            for (size_t i = 0; i < ret; i++)
            {
                // 处理新连接
                if(epevs[i].data.fd == lfd)
                {
                    struct sockaddr_in ca;
                    socklen_t ca_len = sizeof(ca);
                    int cfd = accept(lfd, (struct sockaddr *)&ca, &ca_len);
                    if(cfd == -1)
                    {
                        perror("accept");
                        return -1;
                    }

                    // 输出客户端信息
                    char client_IP[16];
                    inet_ntop(AF_INET, &ca.sin_addr.s_addr, client_IP, sizeof(client_IP));    
                    unsigned int client_port = ntohs(ca.sin_port);
                    cout << "Client IP is " << client_IP << ", client port is " << client_port << "." << endl;

                    // 将新连接对应的文件描述符加入到检测集合
                    struct epoll_event cfd_epev;
                    cfd_epev.data.fd = cfd;
                    cfd_epev.events = EPOLLIN;
                    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &cfd_epev);
                }
                // 有客户端发来数据
                else
                {
                    char recv_buf[BUFSIZ] = {0};
                    int recv_len = read(epevs[i].data.fd, recv_buf, BUFSIZ);
                    if(recv_len == -1)
                    {
                        perror("read");
                        return -1;
                    }
                    else if(recv_len == 0)
                    {
                        // 客户端断开连接
                        cout << "Client closed." << endl;
                        close(epevs[i].data.fd);
                        epevs[i].data.fd = -1;
                        continue;
                    }
                    else
                    {
                        cout << "Recv client data: " << endl << recv_buf << endl;
                    }
                    write(epevs[i].data.fd, recv_buf, BUFSIZ);
                }
            }
        }
    }
    close(lfd);
    close(epfd); 

    return 0;
}