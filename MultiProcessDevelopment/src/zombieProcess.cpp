#include <iostream>
using namespace std;

#include <stdlib.h>
#include <unistd.h>

int testCreatZombieProcess()
{
    
    // 父进程循环产生子进程
    // 子进程打印完之后就死亡
    cout << "I am parent process, pid is " << getpid() << endl;
    while (true)
    {
        pid_t pid = fork();
        if(pid > 0)
        {
            sleep(0.01);
        }
        else if(pid == 0)
        {
            cout << "I am child process, pid is " << getpid() << endl;
            break;
        }
        else
        {
            perror("fork");
            return -1;
        }
    }
    return 0;
}