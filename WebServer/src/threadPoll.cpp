#include "threadPool.h"

template <typename T>
threadPool<T> :: threadPool(int thread_num, int max_requests) :
    m_thread_num(thread_num), m_max_requests(max_requests), m_stop(false), m_threads(NULL) 
{
    // 非法数量，抛出异常
    if((thread_num <= 0) || (max_requests <= 0))
    {
        throw exception();
    }

    m_threads = new pthread_t[m_thread_num];
    if(!m_threads)
    {
        throw exception();
    }

    // 创建m_thread_num个线程，并设置线程分离
    for (size_t i = 0; i < m_thread_num; i++)
    {
        cout << "Creating the " << i << "th thread..." << endl;

        int ret = pthread_create(m_threads+i, NULL, worker, this);
        if(ret != 0)
        {
            char *errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            delete [] m_threads;
            throw exception(); 
        }
        
        ret = pthread_detach(m_threads[i]);
        if(ret != 0)
        {
            char *errmsg = strerror(ret);
            cout << "Error message: " << *errmsg << endl;
            delete [] m_threads;
            throw exception();
        }   
    } 
}


template <typename T>
threadPool<T> :: ~threadPool()
{
    delete [] m_threads;
    m_stop = true;
}

template <typename T>
bool threadPool<T> :: append(T* request)
{
    m_queue_lock.lock();
    if(m_work_queue.size() > m_max_requests)
    {
        m_queue_lock.unlock();
        return false;
    }

    m_work_queue.push_back(request);
    m_queue_lock.unlock();
    m_queue_stat.post();
    return true;
}

template <typename T>
void * threadPool<T> :: worker(void * arg)
{
    threadPool * pool = (threadPool *) arg;
    pool->run();
    return NULL;
}

template <typename T>
void threadPool<T> :: run()
{
    while(!m_stop)
    {
        m_queue_stat.wait();
        m_queue_lock.lock();
        // if(m_work_queue.empty())
        // {
        //     m_queue_lock.unlock();
        //     continue;
        // }

        // 取出任务
        T* request = m_work_queue.front();
        m_work_queue.pop_front();
        m_queue_lock.unlock();

        if(!request)
        {
            continue;
        }
        request->process();
        return;

    }
    
}