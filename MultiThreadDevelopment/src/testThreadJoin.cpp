#include <iostream>
using namespace std;

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "multiThreadDev.h"

extern int testNum;

void * callBackFunc(void *);

int testThreadJoin()
{
    // 创建一个子线程
    pthread_t tid;
    int errNum = pthread_create(&tid, NULL, callBackFunc, (void *)&testNum);
    if(errNum != 0)
    {
        char * errMes = strerror(errNum);
        cout << "pthread_creat error message: " << errMes << endl;
    }
    // 打印提示信息
    sleep(1);
    cout << "This is the main thread." << endl;
    cout << "The child tid is " << tid << ", " << "and the main thread id is " << pthread_self() << "." << endl;

    // 回收子线程资源
    int * thread_retval;
    errNum = pthread_join(tid, (void **)&thread_retval);
    if(errNum != 0)
    {
        char * errMes = strerror(errNum);
        cout << "pthread_creat error message: " << errMes << endl;
    }
    cout << "Exit data: " << *thread_retval << "." << endl;
    // 退出主线程
    pthread_exit(NULL); // 相当于exit()

    return 0;
}