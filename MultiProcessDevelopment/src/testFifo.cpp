#include <iostream>
using namespace std;

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int testFifo()
{

    // 判断文件是否存在
    int ret = access("testFifo", F_OK);
    if(ret != -1)
    {
        cout << "Fifo is not existed, creating fifo file..." << endl;
        int ret = mkfifo("testFifoFile", 0664);

        if(ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
    }
    else
    {
        cout << "Fifo is existed." << endl;
    }

    

    return 0;
}