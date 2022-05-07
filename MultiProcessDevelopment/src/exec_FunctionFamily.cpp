#include <iostream>
#include <unistd.h>
using namespace std;

int testExecFunction()
{
    // 先使用fork创建子进程，在子进程中调用exec函数族，
    // 运行对应的可执行程序，并覆盖当前进程的虚拟地址空间
    pid_t pid = fork();
    if(pid > 1)
    {
        cout << "I am parent process, pid is " << getpid() << endl;
    }
    else if(pid == 0)
    {
        cout << "I am child process, pid is " << getpid() << endl;
        int ret = execl("hello.out", "I_am_hello that", "This", NULL);
        if(ret == -1)
        {
            perror("execlp");
            return -1;
        }
        
    }
    else
    {
        perror("fork");
        return -1;
    }

    for (size_t i = 0; i < 3; i++)
    {
        cout << "i is " << i << ", pid = " << getpid() << endl; 
    }
    
    return 0;
}