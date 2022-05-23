#include <iostream>
using namespace std;

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>


int testWriteSHM()
{
    // 1、创建一个共享内存
    int shm_id = shmget(100, 4096, IPC_CREAT | 0664);
    if(shm_id == -1)
    {
        perror("shmget");
        return -1;
    }
    else
    {
        cout << "shm_id is " << shm_id << " ." << endl;
    }

    // 2、和当前进程关联
    void * ptr = shmat(shm_id, NULL, 0);

    // 3、写共享内存
    const char * str = "Hello World";
    memcpy(ptr, str, strlen(str)+1);

    cout << "Press any key..." << endl;
    getchar();
    cout << "Write func get key..." << endl;

    // 4、解除共享
    int ret = shmdt(ptr);
    if(ret == -1)
    {
        perror("shmdt");
        return -1;
    }

    // 5、删除共享内存
    ret = shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}