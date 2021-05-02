#include "Logging.h"
#include <iostream>
#include <memory>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <string>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

int64_t get_current_millis(void) 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main()
{
    LOG_INIT("LOG");
    uint64_t start_ts = get_current_millis();
    unsigned int num = 1;
    for (auto i = 0; i < num;i++)
    {
        cout << i << endl;
        string str = "This is the " + to_string(i) + "th test message, please ignore the message, it is useless.\n";
        std::cout << str << std::endl;
        cout << "输入大小为：" << strlen(str.c_str()) << "字节" << std::endl;
        LOG(str.c_str());
    }
    uint64_t end_ts = get_current_millis();

    double a = num / 10000;
    double b = (end_ts - start_ts) / 1000;
    double x = a / b;
    //cout << "Rate: " << x << "万次/秒" << endl;

    return 0;
}