#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define BUFSIZE 1024

int testPipe()
{
    // 在fork之前创建管道
    int pipeFd[2];
    int ret = pipe(pipeFd);
    if(ret == -1)
    {
        perror("pipe");
        return -1;
    }

    // 创建子进程
    pid_t pid = fork();
    char buf[BUFSIZE] = {0};
    if(pid > 0)
    {
        // 父进程
        // 从管道中读取数据
        // 关闭写端
        close(pipeFd[1]);
        // string hello = "Hello, I am Parent.";
        // string str;
        // int sec = 0;
        // 无数据阻塞
        while(true)
        {
            // 先读取数据
            int len = read(pipeFd[0], buf, sizeof(buf));
            cout << "Parent recv: " << buf << " Parent pid is " << getpid() << endl;
            bzero(buf, BUFSIZE);    // 清空BUFF

            // // 再写入数据
            // string s_sec = to_string(sec);
            // str = hello + " Time is "  + s_sec + ".";
            // const char *c_str = str.c_str();
            // write(pipeFd[1], c_str, strlen(c_str));
            // sleep(1);
            // ++sec;
        }
    }
    else if(pid == 0)
    {
        // 子进程
        // 在管道中写入数据
        // 关闭读端
        close(pipeFd[0]);
        string hello = "Hello, I am child.";
        string str;
        int sec = 0;
        // 若管道满了也阻塞
        while(true)
        {
            // 先写入数据
            string s_sec = to_string(sec);
            str = hello + " Time is "  + s_sec + ".";
            const char *c_str = str.c_str();
            write(pipeFd[1], c_str, strlen(c_str));
            sleep(1);
            ++sec;

            // // 再读取数据
            // int len = read(pipeFd[0], buf, sizeof(buf));
            // cout << "Child recv: " << buf << " Child pid is " << getpid() << endl;
            // bzero(buf, BUFSIZE);

        }
    }
    else
    {
        perror("fork");
        return -1;
    }
    return 0;
}