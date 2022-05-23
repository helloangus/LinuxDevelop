#include <iostream>
using namespace std;
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "multiThreadDev.h"

int testNum = 99;
void * callBackFunc(void *);


int testThreadCreat()
{
    pthread_t tid;

    // 创建一个子线程
    int ret = pthread_create(&tid, NULL, callBackFunc, (void *)&testNum);
    if(ret != 0)
    {
        char *errstr = strerror(ret);
        cout << "Error message: " << errstr << " ." << endl;
    }

    cout << "This is the main thread. " << endl;
    sleep(1);
    
    return 0;
}

void * callBackFunc(void * arg)
{
    cout << "This is the child thread, arg num is: " << *(int *)arg << endl;
    cout << "Child thread id is " << pthread_self() << "." << endl;
    // cout << "Errno is " << errno <<"." << endl;
    pthread_exit((void *)&errno);
    return NULL;
}