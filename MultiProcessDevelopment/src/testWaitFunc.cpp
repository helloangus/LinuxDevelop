#include <iostream>
using namespace std;
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int testWaitFunc()
{
    cout << "I am the parent process, pid is " << getpid() << endl;
    pid_t usrPid = 0;
    // 循环创建五个子进程
    for (size_t i = 0; i < 5; i++)
    {
        usrPid = fork();
        if(usrPid == 0)
        {
            cout << "A child process is created, pid is " << getpid() << endl;
            break;
        }
        else if(usrPid == -1)
        {
            perror("fork");
            return -1;
        }
    }
    
    // 父进程循环wait，并给出提示
    if(usrPid > 0)
    {
        while (true)
        {
            cout << "Parent process is still alive, pid is " << getpid() << endl;
            int exitSta;
            // 若没有子进程可以wait，则一直阻塞
            int ret = wait(&exitSta);
            if(ret == -1)
            {
                perror("wait");
                break;
            }
            if(WIFEXITED(exitSta))
            {
                // 正常退出
                cout << "The exit code is " << WEXITSTATUS(exitSta) << endl;
            }
            else if(WIFSIGNALED(exitSta))
            {
                // 由于信号异常退出
                cout << "Signal code is " << WTERMSIG(exitSta) << endl;
            }
            cout << "A child process dies, pid is " << ret << endl;
        }
        
    }
    // 子进程循环打印
    else if(usrPid == 0)
    {
        // while(true)
        // {
            cout << "I am a child process, pid is " << getpid() << endl;
            sleep(5);
            exit(9);
        // }   
    }
    return 0;
}