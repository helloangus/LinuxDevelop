#include <iostream>
using namespace std;

#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define IP_addr "127.0.0.1"
#define Port 8888

int main()
{

    cout << "Server is running..." << endl;

    // 1、创建一个通信用socket
    int cfd = socket(PF_INET, SOCK_DGRAM, 0);
    if(cfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 2、绑定
    // 设置端口复用
    int sock_opt = 1;
    int ret = setsockopt(cfd, SOL_SOCKET, SO_REUSEPORT, &sock_opt, sizeof(sock_opt));
    if(ret == -1)
    {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in lsa;
    inet_pton(AF_INET, IP_addr, &lsa.sin_addr.s_addr);
    lsa.sin_family = AF_INET;
    lsa.sin_port = htons(Port);

    ret = bind(cfd, (struct sockaddr*)&lsa, sizeof(lsa));
    if(ret == -1)
    {
        perror("bind");
        return -1;
    }

    // 3、通信
    while(true)
    {
        char recv_buf[BUFSIZ] = {0};
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int recv_len = recvfrom(cfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if(recv_len == -1)
        {
            perror("recvfrom");
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
            char client_IP[16];
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_IP, sizeof(client_IP));    
            unsigned int client_port = ntohs(client_addr.sin_port);
            cout << "Client IP is " << client_IP << ", client port is " << client_port << "." << endl;
            cout << "Recv client data: " << endl << recv_buf << endl;

            sendto(cfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        }
    }
    
    close(cfd);

    return 0;
}