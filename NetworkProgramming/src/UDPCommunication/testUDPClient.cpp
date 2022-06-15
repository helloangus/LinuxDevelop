#include <iostream>
using namespace std;

#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define IP_addr "127.0.0.1"
#define Port 8888

int main()
{

    cout << "Client is running..." << endl;

    // 1、创建一个通信用socket
    int sfd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 2、通信
    char send_buf[BUFSIZ] = {""};
    struct sockaddr_in server_addr;
    inet_pton(AF_INET, IP_addr, &server_addr.sin_addr.s_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Port);
    socklen_t server_addr_len = sizeof(server_addr);
    while(true)
    {
        cout << "Input sending data: " << endl;
        fgets(send_buf, BUFSIZ, stdin);
        cout << endl;
        sendto(sfd, send_buf, sizeof(send_buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // 接收来自服务器端的数据
        char recv_buf[BUFSIZ] = {0};
        int recv_len = recvfrom(sfd, recv_buf, sizeof(recv_buf), 0, NULL, NULL);
        if(recv_len == -1)
        {
            perror("recvfrom");
            return -1;
        }
        else if(recv_len == 0)
        {
            // 服务器端断开连接
            cout << "Server closed." << endl;
            break;
        }
        else
        {
            cout << "Recv server data: " << endl << recv_buf << endl;
        }
   
    }
    
    close(sfd);

    return 0;
}