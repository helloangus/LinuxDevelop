#include <iostream>
using namespace std;

#include <signal.h>
#include <unistd.h>


int testsignalprocmask()
{


    // 设置2、3信号阻塞
    sigset_t set;
    sigemptyset(&set);
    // 将2、3号信号添加到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    // 检查是否正确设置
    if(sigismember(&set, SIGINT) != 1)
    {
        cout << "ERROR in sigaddset." << endl;
    }
    if(sigismember(&set, SIGQUIT) != 1)
    {
        cout << "ERROR in sigaddset." << endl;
    }

    // 修改内核中的阻塞信号集
    int ret = sigprocmask(SIG_BLOCK, &set, NULL);
    if(ret == -1)
    {
        perror("sigprocmask");
        return -1;
    }
    
    int num = 0;

    // 循环打印未决信号集
    while(true)
    {
        ++num;
        sigset_t pendingSet;
        sigemptyset(&pendingSet);
        int ret = sigpending(&pendingSet);
        if(ret == -1)
        {
            perror("sigpending");
            return -1;
        }

        // 遍历前32位
        for (size_t i = 1; i < 32; i++)
        {
            int ret = sigismember(&pendingSet, i);
            if(ret == 1)
            {
                cout << "1 ";
            }
            else if(ret == 0)
            {   
                cout << "0 ";
            }
            else
            {
                perror("sigismember");
                return -1;
            }
        }
        cout << endl;
        sleep(1);

        if(num == 10)
        {
            // 解除阻塞
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }
    

    return 0;
}