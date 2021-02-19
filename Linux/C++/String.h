#include <iostream>
#include <string.h>
using namespace std;

class String
{

public:
    String(const char *s = NULL)
    {
        if (s)
        {
            m_str = new char[strlen(s) + 1];
            strcpy(m_str, s);
        }
        else
        {
            m_str = new char('\0');
        }
    }
    String(const String &s)
    {
        if (&s == this)
            return;
        m_str = new char[strlen(s.m_str) + 1];
        strcpy(m_str, s.m_str);
    }
    String &operator=(String &s)
    {
        if (&s == this)
            return s;
        char *origin = m_str;
        m_str = new char(strlen(s.m_str) + 1);
        strcpy(m_str, s.m_str);
        delete[] origin;
        return *this;
    }
    ~String()
    {
        delete[] m_str;
    }

private:
    char *m_str;
    friend ostream &operator<<(ostream &os, const String &s);
};

ostream &operator<<(ostream &os, const String &s)
{
    os << s.m_str;
}