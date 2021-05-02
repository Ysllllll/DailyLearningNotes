#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <sys/time.h>

#include <iostream>
#include <sstream>

#include "Logging.h"

int                 Logging::Buffer_LEN =   25 * 1024 * 1024;
pthread_mutex_t     Logging::Mutex      =   PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t      Logging::Condition  =   PTHREAD_COND_INITIALIZER;
Logging*            Logging::LoggingPtr =   nullptr;
pthread_once_t      Logging::Once       =   PTHREAD_ONCE_INIT;
const int           BUFF_WAIT_TIME      =   1;

Logging::Logging(int buf_num) : BufferNum(buf_num), WritePtr(nullptr), SavePtr(nullptr)
{
    LogBuffer *head = new LogBuffer(Buffer_LEN);
    if (!head)
    {
        std::cerr << "No space to allocate for logging thread" << std::endl;
        exit(1);
    }

    LogBuffer *curr;
    LogBuffer *prev = head;

    for (size_t i = 0; i < BufferNum; i++)
    {
        curr = new LogBuffer(Buffer_LEN);
        if (!head)
        {
            std::cerr << "No space to allocate for logging thread" << std::endl;
            exit(1);
        }
        //assert(!curr);

        curr->Prev = prev;
        prev->Next = curr;
        prev = curr;
    }

    prev->Next = head;
    head->Prev = prev;

    WritePtr = head;
    SavePtr = head;
}

// 加锁后不能写入和保存，然后更改日志保存路径
void Logging::SetSavePath(std::string save_path)
{
    pthread_mutex_lock(&Mutex);
   

    mkdir(save_path.c_str(), S_IRWXU);
    if (access(save_path.c_str(), F_OK | W_OK) == -1)
    {
        std::cerr << "Can't access to the folder" << std::endl;
    }

    auto time = std::time(nullptr);
    auto localTime = *std::localtime(&time);
    std::ostringstream timeStream;
    timeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M.txt");
    
    if (save_path.empty())
        LogSavePath = timeStream.str();
    else
        LogSavePath = save_path + "/" + timeStream.str();
    pthread_mutex_unlock(&Mutex);
}

void Logging::Save()
{
    while (true)
    {
        //std::cout << "在保存中" << std::endl;
        pthread_mutex_lock(&Mutex);
        //std::cout << "获得了锁" << std::endl;
        // if (SavePtr->Status==LogBuffer::Buffer_Status::FREE)
        // {
        //     struct timespec tsp;
        //     struct timeval now;
        //     gettimeofday(&now, NULL);
        //     tsp.tv_sec = now.tv_sec;
        //     tsp.tv_nsec = now.tv_usec * 1000;//nanoseconds
        //     tsp.tv_sec += BUFF_WAIT_TIME;//wait for 1 seconds
        //     pthread_cond_timedwait(&Condition, &Mutex, &tsp);
        // }

        //std::cout << "here" << std::endl;
        if(SavePtr->Empty())
        {
            pthread_mutex_unlock(&Mutex);
            SavePtr = SavePtr->Next;
            continue;
        }

        // if (WritePtr->Status == LogBuffer::Buffer_Status::FREE)
        // {
        //     SavePtr->Status == LogBuffer::Buffer_Status::FULL;
        // }
        //std::cout << "开始保存" << std::endl;
        SavePtr->SaveToFile(LogSavePath);
        SavePtr = SavePtr->Next;

        pthread_mutex_unlock(&Mutex);
    }
}

void Logging::AppendLog(const char *log, size_t len)
{
    std::cout << "AppendLog:" << len << "字节" << std::endl;
    pthread_mutex_lock(&Mutex);
    if (WritePtr->Status == LogBuffer::Buffer_Status::FREE && WritePtr->Available() >= len)
    {
        std::cout << "添加日志" << std::endl;
        WritePtr->Append(log, len);
    }
    else if (WritePtr->Status == LogBuffer::Buffer_Status::FREE) // 不满足保存条件
    {
        WritePtr->Status == LogBuffer::Buffer_Status::FULL;
    }

    WritePtr = WritePtr->Next;

    pthread_mutex_unlock(&Mutex);
}

void* run(void* args)
{
    std::cout << "run中" << std::endl;
    if (auto it = Logging::InitOnce())
    {
        it->Save();
    }

    //Logging::InitOnce()->Save();
    return NULL;
}