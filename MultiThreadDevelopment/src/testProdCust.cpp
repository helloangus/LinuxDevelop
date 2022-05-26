#include <iostream>
using namespace std;
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "multiThreadDev.h"

void * producerFunc(void *);
void * customerFunc(void *);
extern pthread_mutex_t mutex;
pthread_cond_t cond;

// 定义产品链表头节点
Node * head = new Node;

int testProdCust()
{
    // 初始化锁
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // 创建五个生产者和五个消费者线程
    pthread_t ptid[5], ctid[5];
    for (size_t i = 0; i < 5; i++)
    {
        int ret = pthread_create(&ptid[i], NULL, producerFunc, NULL);
        if(ret != 0)
        {
            char * errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            return -1;
        }
        ret = pthread_create(&ctid[i], NULL, customerFunc, NULL);
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
    

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);    

    pthread_exit(NULL);


    return 0;
}


void * producerFunc(void * arg)
{
    // 不断生产产品
    while(true)
    {
        pthread_mutex_lock(&mutex);
        struct Node* p = new struct Node;
        p->num = rand() % 1000;
        printf("Add node, num: %d, tid: %ld.\n", p->num, pthread_self());
        p->next = head->next;
        head->next = p;

        // 生产了一个就通知消费者消费一个
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }

    return NULL;
}


void * customerFunc(void * arg)
{
    // 不考虑消费完的情况
    while(true)
    {
        pthread_mutex_lock(&mutex);
        struct Node *q = head->next;
        if(q != NULL)
        {
            head->next = head->next->next;
            printf("Del node, num: %d, tid: %ld.\n", q->num, pthread_self());
            delete q;
            pthread_mutex_unlock(&mutex);
            usleep(100);
        }
        // 没有数据存在，需要等待
        else
        {
            pthread_cond_wait(&cond, &mutex);   // 该函数调用，阻塞时会解锁，重新执行会加锁
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}