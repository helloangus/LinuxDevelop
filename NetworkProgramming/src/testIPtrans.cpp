#include "networkProgramming.h"

#include <arpa/inet.h>
#include <string.h>

int testIPtrans()
{

    // 创建一个点分十进制的IP字符串
    char buf[] = "192.168.1.8";
    unsigned int num = 0;

    // IP字符串转化为网络字节序的整数
    inet_pton(AF_INET, buf, (void *)&num);
    unsigned char * p = (unsigned char *)&num;
    cout << "Transform result is " << (int)*p << " " << (int)*(p+1) << " " << (int)*(p+2) << " " << (int)*(p+3) << "." << endl;

    // 网络字节数的整数转化为IP字符串
    char ipStr[16];
    const char * retStr = inet_ntop(AF_INET, &num, ipStr, 16);
    cout << "retStr: " << retStr << endl;
    cout << "ipStr: " << ipStr << endl;


    return 0;
}