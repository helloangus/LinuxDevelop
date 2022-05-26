#include <iostream>
using namespace std;
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "multiThreadDev.h"

void * producerFunc_sem(void *);
void * customerFunc_sem(void *);
extern pthread_mutex_t mutex;
sem_t psem, csem;

// 定义产品链表头节点
extern Node * head;

int testSemaphore()
{
    // 初始化锁
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

    // 创建五个生产者和五个消费者线程
    pthread_t ptid[5], ctid[5];
    for (size_t i = 0; i < 5; i++)
    {
        int ret = pthread_create(&ptid[i], NULL, producerFunc_sem, NULL);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
        ret = pthread_create(&ctid[i], NULL, customerFunc_sem, NULL);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
    }


    // 设置线程分离
    for (size_t i = 0; i < 5; i++)
    {
        int ret = pthread_detach(ptid[i]);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
        ret = pthread_detach(ctid[i]);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }     
    }

    sem_destroy(&psem);
    sem_destroy(&csem);
    pthread_mutex_destroy(&mutex);    

    pthread_exit(NULL);


    return 0;
}


void * producerFunc_sem(void * arg)
{
    // 不断生产产品
    while(true)
    {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct Node* p = new struct Node;
        p->num = rand() % 1000;
        printf("Add node, num: %d, tid: %ld.\n", p->num, pthread_self());
        p->next = head->next;
        head->next = p;

        // 生产了一个就通知消费者消费一个
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        usleep(100);
    }

    return NULL;
}


void * customerFunc_sem(void * arg)
{
    // 不考虑消费完的情况
    while(true)
    {
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node *q = head->next;
        head->next = head->next->next;
        printf("Del node, num: %d, tid: %ld.\n", q->num, pthread_self());
        delete q;
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        usleep(100);
    }

    return NULL;
}