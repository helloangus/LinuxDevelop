#include <iostream>
using namespace std;

#include <pthread.h>
#include <unistd.h>

void * sellTicket(void *);
int ticketNum = 100;

// 创建互斥量
pthread_mutex_t mutex;

int testSelTickets()
{
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    // 创建三个子线程
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sellTicket, NULL);
    pthread_create(&tid2, NULL, sellTicket, NULL);
    pthread_create(&tid3, NULL, sellTicket, NULL);

    // 回收子线程的资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    // // 设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    // 释放互斥量
    pthread_mutex_destroy(&mutex);
    cout << "Destroy mutex" << endl;

    pthread_exit(NULL);

    return 0;
}

void * sellTicket(void * arg)
{
    // 卖票
    while(true)
    {
        // 加锁
        pthread_mutex_lock(&mutex);
        if(ticketNum > 0)
        {
            // usleep(100);
            cout << "Now " << pthread_self() << " selling the " << ticketNum << "th" << " ticket." << endl;
            --ticketNum;
        }
        else
        {
            // 解锁
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 解锁
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}