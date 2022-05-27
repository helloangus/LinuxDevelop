#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>


#define IP_addr "127.0.0.1"
#define Port 9999

int main()
{
    cout << "Server running..." << endl;

    // 1、创建用于监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    // 每次调用系统API时都需要判断一下是否成功
    if(lfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 2、绑定用于监听的IP地址和端口号
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, IP_addr, &(saddr.sin_addr.s_addr));
    saddr.sin_port = htons(Port);   // 需转化为网络字节序
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1)
    {
        perror("bind");
        return -1;
    }

    // 3、监听
    ret = listen(lfd, 8);
    if(ret == -1)
    {
        perror("listen");
        return -1;
    }

    // 4、接收客户端连接
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if(cfd == -1)
    {
        perror("accept");
        return -1;
    }

    // 输出客户端信息
    char client_IP[16];
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_IP, sizeof(client_IP));    
    unsigned int client_port = ntohs(client_addr.sin_port);
    cout << "Client IP is " << client_IP << ", client port is " << client_port << "." << endl;

    // 5、双方通信(回射通信)
    // 获取客户端数据
    char recv_buf[BUFSIZ] = {0};
    while(true)
    {
        ssize_t recv_len = read(cfd, recv_buf, BUFSIZ);
        if(recv_len == -1)
        {
            perror("read");
            return -1;
        }
        else if(recv_len == 0)
        {
            // 客户端断开连接
            cout << "Client closed." << endl;
            break;
        }
        else
        {
            cout << "Recv data: " << endl << recv_buf << endl;
        }

        // 给客户端发送数据
        write(cfd, recv_buf, BUFSIZ);
   
    }

    // 关闭连接
    close(cfd);
    close(lfd);

    return 0;
}