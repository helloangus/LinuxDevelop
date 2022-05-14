#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int mulChatFunc(const char *w_fifo_path, const char *r_fifo_path)
{
    // 判断两个有名管道是否存在
    int ret = access(w_fifo_path, F_OK);
    if(ret == -1)
    {
        cout << "File " << w_fifo_path << " is not existed, creating..." << endl;
        ret = mkfifo(w_fifo_path, 0664);
        if(ret == -1)
        {
            cout << "Error message: For " << w_fifo_path << ", ";
            perror("mkfifo");
            return -1;
        }
    }

    ret = access(r_fifo_path, F_OK);
    if(ret == -1)
    {
        cout << "File " << r_fifo_path << " is not existed, creating..." << endl;
        ret = mkfifo(r_fifo_path, 0664);
        if(ret == -1)
        {
            cout << "Error message: For " << r_fifo_path << ", ";
            perror("mkfifo");
            return -1;
        }
    }



    int fdw, fdr;
    // 创建子进程
    int pid = fork();
    // 父进程负责打开接收fifo
    if(pid == 0)
    {
        // 以只读的方式打开接收fifo文件
        fdr = open(r_fifo_path, O_RDONLY);
        if(fdr == -1)
        {
            perror("open fdr");
            return -1;
        }
        cout << "Open " << r_fifo_path << " successful, waiting for read..." << endl;
    }
    // 子进程负责打开发送fifo
    else if(pid > 0)
    {
        // 先以只写的方式打开发送fifo文件
        fdw  = open(w_fifo_path, O_WRONLY);
        if(fdw == -1)
        {
            perror("open fdw");
            return -1;
        }
        cout << "Open " << w_fifo_path << " successful, waiting for write..." << endl; 
    }
    else
    {
        perror("fork");
        return -1;
    }


    // 循环写入读取数据
    char buf[BUFSIZ];
    // 父进程负责读
    if(pid == 0)
    {
        while(true)
        {
            // 读取数据
            ret = read(fdr, buf, BUFSIZ);
            if(ret <= 0)
            {
                perror("read fdr");
                break;
            }
            cout << "Recv: " << endl << buf << endl;
            memset(buf, 0 ,BUFSIZ);
        }
    }
    // 子进程负责写
    else
    {
        while(true)
        {
            // 写入数据
            memset(buf, 0, BUFSIZ);
            // 获取标准输入的数据
            cout << "Sending: " << endl;
            fgets(buf, BUFSIZ, stdin);
            cout << endl;

            int ret = write(fdw, buf, strlen(buf));
            if(ret == -1)
            {
                perror("write fdw");
                break;
            }   
        }
    }
        
    // 关闭文件
    close(fdw);
    close(fdr);

    return 0;
}