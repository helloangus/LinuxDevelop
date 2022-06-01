#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <pthread.h>

#define IP_addr "127.0.0.1"
#define Port 9999
#define MaxClient 128

void * communication(void *);

struct sockInfo
{
    int fd; // 通信的文件描述符
    pthread_t tid; // 线程号
    struct sockaddr_in addr;
};

struct sockInfo sockInfos[MaxClient];

int main()
{
    cout << "Multi thread server running..." << endl;

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
    int ret = bind(lfd, (struct sockaddr *)&lsa, sizeof(lsa));
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

    // 初始化数据
    for (size_t i = 0; i < MaxClient; i++)
    {
        bzero(&sockInfos[i], sizeof(sockInfo));
        sockInfos[i].fd = -1;
        sockInfos[i].tid = -1;
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
            // 创建子线程与服务器通信
            struct sockInfo * pInfo;
            // 找到一个可用的结构体
            for (size_t i = 0; i < MaxClient; i++)
            {
                if(sockInfos[i].fd == -1)
                {
                    pInfo = &sockInfos[i];
                    break;
                }
                if(i == MaxClient-1)
                {
                    sleep(1);
                    i = -1;
                }
            }
            pInfo->fd = cfd;
            memcpy(&pInfo->addr, &client_addr, client_addr_len);
            
            int ret = pthread_create(&pInfo->tid, NULL, communication, (void *)pInfo);
            if(ret != 0)
            {
                char * errmsg = strerror(ret);
                cout << "Error message: " << *errmsg << endl;
            }

            pthread_detach(pInfo->tid);
        }
    }


    // 6、关闭连接
    close(lfd);

    return 0;
}


void * communication(void * arg)
{
    // 子线程与客户端通信
    // 需要传递的信息： cfd、客户端的信息、线程号

    struct sockInfo *pInfo = (struct sockInfo*)arg;

    // 5、双方通信
    // 输出客户端信息
    char client_IP[16];
    inet_ntop(AF_INET, &pInfo->addr.sin_addr.s_addr, client_IP, sizeof(client_IP));    
    unsigned int client_port = ntohs(pInfo->addr.sin_port);
    cout << "Client IP is " << client_IP << ", client port is " << client_port << "." << endl;

    // 5、双方通信(回射通信)
    // 获取客户端数据
    char recv_buf[BUFSIZ] = {0};
    while(true)
    {
        ssize_t recv_len = read(pInfo->fd, recv_buf, BUFSIZ);
        if(recv_len == -1)
        {
            perror("read");
            close(pInfo->fd);
            return NULL;
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
        write(pInfo->fd, recv_buf, BUFSIZ);
    }
    close(pInfo->fd);
    return NULL;   // 子进程退出
}