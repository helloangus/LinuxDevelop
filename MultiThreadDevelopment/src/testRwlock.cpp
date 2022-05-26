#include <iostream>
using namespace std;

#include <pthread.h>
#include <unistd.h>
#include <string.h>

void * writeCallBackFunc(void *);
void * readCallBackFunc(void *);


// 创建一个全局共享数据
extern int testNum;

extern pthread_mutex_t mutex;

pthread_rwlock_t rwlock;


int testRwlock()
{
    // 初始化互斥量
    // pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);

    // 创建三个写线程，五个读线程
    pthread_t wtid[3];
    pthread_t rtid[5];

    for (size_t i = 0; i < 3; i++)
    {
        int ret = pthread_create(&wtid[i], NULL, writeCallBackFunc, NULL);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
    }

    for (size_t i = 0; i < 5; i++)
    {
        int ret = pthread_create(&rtid[i], NULL, readCallBackFunc, NULL);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
    }

    // 设置线程分离
    for (size_t i = 0; i < 3; i++)
    {
        int ret = pthread_detach(wtid[i]);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
    }
    
    for (size_t i = 0; i <5; i++)
    {
        int ret = pthread_detach(rtid[i]);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
    }

    // 销毁互斥量
    pthread_rwlock_destroy(&rwlock);

    pthread_exit(0);
    return 0;
}


void * writeCallBackFunc(void * arg)
{

    while (true)
    {
        pthread_rwlock_wrlock(&rwlock);
        ++testNum;
        cout << "Write thread ++num, tid: " << pthread_self() << ", testNum: " << testNum << "." << endl;
        pthread_rwlock_unlock(&rwlock);
        usleep(1000);
    }
    
    return NULL;
}


void * readCallBackFunc(void * arg)
{
    while (true)
    {
        pthread_rwlock_rdlock(&rwlock);
        cout << "Read thread, tid: " << pthread_self() << ", testNum: " << testNum << "." << endl;
        pthread_rwlock_unlock(&rwlock);
        usleep(1000);
    }
    

    return NULL;
}