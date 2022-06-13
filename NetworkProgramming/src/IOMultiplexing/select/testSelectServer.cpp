#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
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
    fd_set rd_set, tmp_rd_set;
    FD_ZERO(&rd_set);
    FD_SET(lfd, &rd_set);
    int max_fd = lfd;

    while(true)
    {
        // 调用select函数，让内核检测哪些文件描述符有数据
        tmp_rd_set = rd_set;
        int ret = select(max_fd+1, &tmp_rd_set, NULL, NULL, NULL);    // 阻塞
        if(ret == -1)
        {
            perror("select");
            return -1;
        }
        else if(ret == 0)
        {
            continue;
        }
        // 检测到的变化的文件描述符大于0
        else if(ret > 0)
        {
            // 有新的连接
            if(FD_ISSET(lfd, &tmp_rd_set))
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
                FD_SET(cfd, &rd_set);
                max_fd = max_fd > cfd ? max_fd : cfd;   // 更新最大的文件描述符
            }

            // 遍历整个检测集合，找到有数据写入的文件描述符
            for (size_t i = lfd + 1; i < max_fd + 1; i++)
            {
                if(FD_ISSET(i, &tmp_rd_set))
                {
                    // 说明该客户端有数据传入
                    char recv_buf[BUFSIZ] = {0};
                    int recv_len = read(i, recv_buf, BUFSIZ);
                    if(recv_len == -1)
                    {
                        perror("read");
                        return -1;
                    }
                    else if(recv_len == 0)
                    {
                        // 客户端断开连接
                        cout << "Client closed." << endl;
                        close(i);
                        FD_CLR(i, &rd_set);
                        continue;
                    }
                    else
                    {
                        cout << "Recv client data: " << endl << recv_buf << endl;
                    }
                    write(i, recv_buf, BUFSIZ);
                }
            }
            
        }
    }
    close(lfd);

    return 0;
}