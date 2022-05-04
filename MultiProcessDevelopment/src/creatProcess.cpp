#include <sys/types.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int testCreatProcess()
{
    pid_t pid = fork();

    // 判断父子进程
    if(pid > 0)
    {
        cout << "子进程的pid为：" << pid << endl;
        cout << "这里是父进程，pid为" << getpid() << "ppid为" << getppid() << endl;
    }
    else if(pid == 0)
    {
        cout << "这里是子进程，pid为" << getpid() << "ppid为" << getppid() << endl;
    }
    else
    {
        perror("fork");
        return -1;
    }

    for (size_t i = 0; i < 3; i++)
    {
        cout << i << endl;
        sleep(1);
    }
    return 0;
    
}