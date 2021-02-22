//testGdb.c
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

template <class T>
class example
{
public:
    example(T a) : m_a(a)
    {
    }
    T get() const
    {
        return m_a;
    }
    static void func();

private:
    T m_a;
};

template <class T>
void example<T>::func()
{
    m_a = 0;
}

int main(void)
{
    example<int> a(5);
    example<int>::func();
    cout << a.get() << endl;

    return 0;
}