#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    // 打开fifo文件
    int fd = open("testFifoFile", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(0);
    }

    // 读数据
    while (true)
    {
        char buf[BUFSIZ];
        int len = read(fd, buf, sizeof(buf));
        if(len == 0)
        {
            cout << "The writing side is disconnected..." << endl;
            break;
        }
        cout << "Recv buf: " << buf << endl;
        memset(buf, 0, BUFSIZ);
    }

    close(fd);
    

    return 0;
}