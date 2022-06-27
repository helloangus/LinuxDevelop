#ifndef __DEFINE__
#define __DEFINE__

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <exception>

#include "threadPool.h"
#include "httpConn.h"

#define MAX_THREAD_NUM 10
#define MAX_REQUESTS 1000

// 添加信号捕捉函数
void addSig(int signum, void (*func)(int));


#endif