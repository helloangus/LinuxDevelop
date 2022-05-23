#include <iostream>
using namespace std;

#include <unistd.h>
#include <wait.h>

int testWriteSHM();
int testReadSHM();

int testSHM()
{

    // 创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return -1;
    }
    // 父进程执行读操作
    else if(pid > 0)
    {
        sleep(1);
        testReadSHM();
        sleep(1);
        wait(NULL);
    }
    // 子进程执行写操作
    else
    {
        testWriteSHM();
    }


    return 0;
}