#ifndef __LOCKER__
#define __LOCKER__

#include "define.h"

// 线程同步封装类

class mutex
{
    public:
        mutex();
        ~mutex();
        
        bool lock();
        bool unlock();

        pthread_mutex_t * get();

    private:
        pthread_mutex_t m_mutex;
};


class cond
{
    public:
        cond();
        ~cond();

        bool wait(pthread_mutex_t * mutex);
        bool timewait(pthread_mutex_t * mutex, struct timespec t);
        bool signal();
        bool broadcast();

    private:
        pthread_cond_t m_cond;
};


class sem
{
    public:
        sem();
        sem(int num);
        ~sem();

        bool wait();
        bool post();


    private:
        sem_t m_sem;
};


#endif