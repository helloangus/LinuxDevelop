#include <iostream>
using namespace std;
#include <signal.h>
typedef void (*sighandler_t)(int);
#include <sys/time.h>

void myHandler(int);

int testsigaction()
{

    // 注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myHandler;
    sigemptyset(&act.sa_mask); // 清空临时阻塞信号集
    int ret_SIG = sigaction(SIGALRM, &act, NULL);
    if(ret_SIG == -1)
    {
        perror("signal");
        return -1;
    }
    // 设置定时器参数结构体
    itimerval *parm = new itimerval;
    parm->it_interval.tv_sec = 2;
    parm->it_interval.tv_usec = 0;
    parm->it_value.tv_sec = 3;
    parm->it_value.tv_usec = 0;
    // 设置定时器
    int ret = setitimer(ITIMER_REAL, parm, NULL);
    if(ret == -1)
    {
        perror("setitimer");
        return -1;
    }
    // 打印提示
    cout << "The countdown starts..." << endl;
    // 死循环
    while(true)
    {
        
    }
    


    return 0;
}


// // 定义信号捕捉回调函数
// void myHandler(int sigNum)
// {
//     cout << "The signal number is " << sigNum << "." << endl;
// }