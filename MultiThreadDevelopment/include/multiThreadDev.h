#ifndef MULTITHREADDEV
#define MULTITHREADDEV

#include <iostream>
using namespace std;
int testThreadCreat();
int testThreadJoin();
int testSelTickets();
int testRwlock();
int testProdCust();
int testSemaphore();

// 定义产品链表节点结构体
typedef struct Node
{
    int num;
    struct Node *next;
}Node;


#endif