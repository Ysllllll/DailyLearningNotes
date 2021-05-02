#ifndef LOGGING_H
#define LOGGING_H
#include <pthread.h>

#include <cstring>

#include "LogBuffer.h"
class Logging
{
private:
    static Logging*             LoggingPtr;
    static int                  Buffer_LEN;
    static pthread_mutex_t      Mutex;
    static pthread_cond_t       Condition;
    static pthread_once_t       Once;
    int                         BufferNum;
    LogBuffer*                  WritePtr;
    LogBuffer*                  SavePtr;
    std::string                 LogSavePath;

public:
    static Logging* InitOnce()
    {
        // 如果写成pthread_once(&Once, Logging::Init())会一直报错，多了那两个()☺
        pthread_once(&Once, Logging::Init);
        return LoggingPtr;
    }

    static void Init()
    {
        while(!LoggingPtr)
        {
            LoggingPtr = new Logging();
        }
    }

    void SetSavePath(std::string save_path);
    void Save();
    void AppendLog(const char *log, size_t len);

public:
    Logging(int buf_num = 12);
    ~Logging();
};

void *run(void *args);

#define LOG_INIT(save_path)                          \
    do                                               \
    {                                                \
        Logging::InitOnce()->SetSavePath(save_path); \
        pthread_t tid;                               \
        pthread_create(&tid, NULL, run, NULL);       \
        pthread_detach(tid);                         \
    }                                                \
    while(0)

#define LOG(str)\
    do\
    {\
        Logging::InitOnce()->AppendLog(str, strlen(str));\
    }\
    while(0)

#endif