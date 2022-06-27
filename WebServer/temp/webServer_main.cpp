#include <iostream>
using namespace std;


#include "define.h"


int main(int argc, char * argv[])
{
    cout << "Hello World!" << endl;

    if(argc <= 1)
    {
        cout << "Please run as follows: " << basename(argv[0]) << "port number" << endl;
        exit(-1);
    }

    // 获取端口号
    int port = atoi(argv[1]);

    // 对SIGPIPE信号进行处理
    addSig(SIGPIPE, SIG_IGN);

    // 创建线程池
    threadPool<httpConn> * pool = NULL;
    try
    {
        pool = new threadPool<httpConn>(MAX_THREAD_NUM, MAX_REQUESTS);
    }
    catch(const char * errmsg)
    {
        cerr << "Error message: " << errmsg << endl;
        exit(-1);
    }


    return 0;
}

// 注册信号捕捉函数
void addSig(int signum, void (*func)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = func;
    sigfillset(&sa.sa_mask);
    sigaction(signum, &sa, NULL);
}