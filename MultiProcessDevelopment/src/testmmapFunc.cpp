#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>


int testmmapFunc()
{

    // 使用内存映射实现两个进程间的通信
    // 1、打开同一个文件
    int fd = open("test.txt", O_RDWR | O_CREAT);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }

    // 2、获取文件大小
    int size = lseek(fd, 0, SEEK_END);
    if(size == -1)
    {
        perror("lseek");
        return -1;
    }
    
    // 3、实现内存映射
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
    {
        perror("mmap");
        return -1;
    }

    // 4、创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return -1;
    }

    // 5、子进程向对应内存中写入数据
    if(pid == 0)
    {
        strcpy((char *)addr, "Hello, this is the child process writing...");
    }
    // 6、父进程回收子进程资源，并获取子进程写入的内容
    else
    {
        wait(NULL);
        char buf[BUFSIZ];
        strcpy(buf, (char *)addr);
        cout << "Recv: " << buf << endl;
    }
    // 7、关闭文件， 关闭内存映射区
    close(fd);
    munmap(addr, size);

    return 0;
}