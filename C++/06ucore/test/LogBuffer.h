#ifndef LOGBUFFER_H
#define LOGBUFFER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <string>

class LogBuffer
{
private:
    unsigned int BufferLen;
    unsigned int UsedLen;
    std::vector<char> BufferData;

public:
    enum Buffer_Status
    {
        FREE,
        FULL
    };
    LogBuffer *Prev;
    LogBuffer *Next;
    Buffer_Status Status;

public:
    LogBuffer(unsigned int buf_len) : BufferLen(buf_len), Status(FREE),
                                      Prev(nullptr), Next(nullptr), UsedLen(0)
    {
        BufferData.resize(BufferLen);
        BufferData.shrink_to_fit();
    }
    ~LogBuffer();

    bool Empty() const
    {
        return UsedLen == 0;
    }

    unsigned int Available()
    {
        return BufferLen - UsedLen;
    }

    void Reset()
    {
        UsedLen = 0;
        Status = FREE;
    }

    void SaveToFile(std::string save_path)
    {
        if (UsedLen == 0)
            return;

        std::ofstream out_file(save_path, std::ios_base::app);
        for (size_t i = 0; i < UsedLen; i++)
        {
            out_file << BufferData[i];
        }
        std::cout << "保存日志到: " << save_path << std::endl;
        std::cout << "保存了" << UsedLen << "字节" << std::endl;
        out_file << std::flush;
        out_file.close();
        Reset();
    }

    void Append(const char *log, size_t len)
    {
        std::cout << "len:" << len << std::endl;
        if (Available() < len)
            return;

        std::cout << "保存前UsedLen:" << UsedLen << std::endl;
        std::cout << std::string(log, log + len) << std::endl;

        std::cout << "in the save: " << std::endl;
        for (size_t i = 0; i < len; i++)
        {
            BufferData[UsedLen + i] = *(log + i);
            std::cout << BufferData[UsedLen + i];
            UsedLen++;
        }

        std::cout << "in the buffer: " << std::endl;
        for (size_t i = 0; i < 72; i++)
        {
            std::cout << BufferData[i];
        }

        std::cout << "保存后UsedLen:" << UsedLen << std::endl;
    }
};

#endif