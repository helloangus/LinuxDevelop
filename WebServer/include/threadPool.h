#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__


#include <iostream>
using namespace std;
#include <pthread.h>
#include <list>
#include "locker.h"
#include <exception>
#include <string.h>

// 线程池，定义成模板类，模板参数就是任务类
template <typename T>
class threadPool
{
    public:
        threadPool(int, int);
        ~threadPool();

        // 添加任务
        bool append(T* request);

    private:
        //  C++中的线程执行函数必须为静态函数
        static void * worker(void * arg);
        void run();

    private:
        // 线程的数量
        int m_thread_num;

        // 线程池的数组
        pthread_t * m_threads;

        // 请求队列中最多允许的等待处理的请求数量
        int m_max_requests;

        // 请求队列
        list<T*> m_work_queue;

        // 互斥锁
        mutex m_queue_lock;

        // 信号量
        sem m_queue_stat;

        // 是否需要结束线程
        bool m_stop;
};

#include "threadPool.cpp"

#endif // __THREADPOOL_H__
