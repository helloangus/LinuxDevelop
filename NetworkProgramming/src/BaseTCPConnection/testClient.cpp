#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>

#define IP_addr "127.0.0.1"
#define Port 9999

int main()
{
    cout << "Client is running..." << endl;

    // 1、创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        return -1;
    }

    // 2、连接服务器
    struct sockaddr_in server_addr;
    inet_pton(AF_INET, IP_addr, &server_addr.sin_addr.s_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Port);
    int ret = connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1)
    {
        perror("connect");
        return -1;
    }

    // 3、通信（回射通信）
    // 给服务器端发送数据
    char send_buf[BUFSIZ] = {""};
    while(true)
    {
        cout << "Input sending data: " << endl;
        fgets(send_buf, BUFSIZ, stdin);
        cout << endl;
        write(fd, send_buf, BUFSIZ);

        // 接收来自服务器端的数据
        char rec_buf[BUFSIZ] = {0};
        ssize_t len = read(fd, rec_buf, BUFSIZ);
        if(len == -1)
        {
            perror("read");
            return -1;
        }
        else if(len == 0)
        {
            // 服务器端断开连接
            cout << "Server closed." << endl;
            break;
        }
        else
        {
            cout << "Recv data: " << endl << rec_buf << endl;
        }
   
    }
    
    // 关闭连接
    close(fd);

    return 0;
}