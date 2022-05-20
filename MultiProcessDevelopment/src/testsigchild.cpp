#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <wait.h>

void mySaHandler(int);

int testsigchild()
{
    // 为防止子进程结束前，父进程还未注册信号捕捉
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // 创建一些子进程
    pid_t pid;
    for (size_t i = 0; i < 10; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
        else if(pid < 0)
        {
            perror("fork");
            return -1;
        }
    }
    
    // 父进程
    if(pid > 0)
    {
        // 捕捉子进程退出时发送的sigchild信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = mySaHandler;
        sigemptyset(&act.sa_mask);
        int ret = sigaction(SIGCHLD, &act, NULL);

        // 注册完成，解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        if(ret == -1)     
        {
            perror("sigaction");
            return -1;
        }
    
        while(true)
        {
            cout << "Parent process pid is " << getpid() << endl;
            sleep(2);
        }
    }
    else if(pid == 0)
    {
        cout << "Child process pid is " << getpid() << endl;
        sleep(1);
    }

    return 0;
}


void mySaHandler(int sigNum)
{
    cout << "The recv signal number is " << sigNum << "." << endl;
    while(true)
    {
        // 非阻塞
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1)
        {
            perror("wait");
            break;
        }
        else if(ret == 0)
        {
            // 说明还有子进程未死亡
            break;
        }
        cout << "The waited child process pid is " << ret << "." << endl;
    }
    
}