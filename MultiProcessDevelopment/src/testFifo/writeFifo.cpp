#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main()
{
    // 判断文件是否存在
    int ret = access("testFifoFile", F_OK);
    if(ret == -1)
    {
        cout << "Fifo is not existed, creating fifo file..." << endl;

        // 创建管道
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

    // 以只写的方式打开管道
    int fd = open("testFifoFile", O_WRONLY);    // 管道文件只有写端或者只有读端都会阻塞
    if(fd == -1)
    {
        perror("open");
        exit(0);
    }

    // 写入数据
    for (int i = 0; i < 100; i++)
    {
        char buf[BUFSIZ];
        sprintf(buf, "hello, %d\n", i);
        cout << "Write data: " << buf << endl;
        write(fd, buf, strlen(buf));
        sleep(2);
    }

    close(fd);
    

    return 0;
}