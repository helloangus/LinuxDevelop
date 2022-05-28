#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

#define IP_addr "127.0.0.1"
#define Port 9999

void waitChild(int);

int main()
{
    cout << "Multi process server running..." << endl;

    // 注册信号捕捉
    struct sigaction sa;
    sa.sa_flags = 0;
    int ret = sigemptyset(&sa.sa_mask);
    if(ret == -1)
    {
        perror("sigemptyset");
        return -1;
    }
    sa.sa_handler = waitChild;
    ret = sigaction(SIGCHLD, &sa, NULL);

    // 1、创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket");
        return -1;
    }


    // 2、绑定用于监听的IP和端口号
    struct sockaddr_in lsa;
    lsa.sin_family = AF_INET;
    inet_pton(AF_INET, IP_addr, &lsa.sin_addr.s_addr);
    lsa.sin_port = htons(Port);
    ret = bind(lfd, (struct sockaddr *)&lsa, sizeof(lsa));
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

    // 4、设置父进程循环接收客户端连接
    while(true)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if(cfd == -1)
        {
            // 如果在accept阻塞的时候，有SIGCHILD信号产生了中断，则accept也会产生errorno，此时需要另外做判断
            if(errno == EINTR)
            {
                // 此时并没有新的客户端接入，直接进行下一轮循环
                continue;
            }
            perror("accept");
            return -1;
        }
        else
        {
            // 创建子进程与服务器通信
            pid_t pid;
            pid = fork();
            if(pid == -1)
            {
                perror("fork");
                return -1;
            }
            else if(pid == 0)
            {
                // 5、双方通信
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
                        cout << "Recv client data: " << endl << recv_buf << endl;
                    }

                    // 给客户端发送数据
                    write(cfd, recv_buf, BUFSIZ);
                }
                close(cfd);
                close(lfd);
                return 0;   // 子进程退出
            }
            else if(pid > 0)
            {
                close(cfd); // 父进程也需要关闭
            }
        }
    }


    // 6、关闭连接
    close(lfd);

    return 0;
}


void waitChild(int arg)
{
    while(true)
    {
        // 可能死亡好几个子进程才发出一个child信号
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1)
        {
            // 所有子进程已经回收完成
            break;
        }
        else if(ret == 0)
        {
            // 还有子进程或者
            break;
        }
        else if(ret > 0)
        {
            cout << "The recycled child process pid is " << ret << endl;
        }
    }
}