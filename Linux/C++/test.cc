//testGdb.c
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

namespace test1 //测试仿函数
{
    class stringAppend
    {
    public:
        explicit stringAppend(const string &str = "") : ss(str) {}

        void operator()(const string &str) const
        {
            cout << str << ' ' << ss << endl;
        }

    private:
        const string ss;
    };

    int main(void)
    {
        stringAppend my("and world!");
        my("hello");
        stringAppend()("Yes");
        return 0;
    }
} // namespace test1

namespace test2 //测试模板特化
{
    template <class key>
    struct Hash
    {
    };

    template <>
    struct Hash<char>
    {
        size_t operator()(char x) const { return x; }
    };

    template <>
    struct Hash<int>
    {
        size_t operator()(int x) const { return x; }
    };

    template <>
    struct Hash<long>
    {
        size_t operator()(long x) const { return x; }
    };

    int main()
    {
        cout << hash<long>()(1000) << endl;
        return 0;
    }
} // namespace test2

class A
{
public:
    A() : m_data1(0), m_data2(0) {}
    virtual void vfunc1()
    {
        cout << "A::vfunc1()" << endl;
    }
    virtual void vfunc2()
    {
        cout << "A::vfunc2()" << endl;
    }
    void fun1()
    {
        cout << "A::func1()" << endl;
    }
    void fun2()
    {
        cout << "A::vfunc2()" << endl;
    }

private:
    int m_data1, m_data2;
};

class B : public A
{
public:
    B() : m_data3(0) {}
    virtual void vfunc1()
    {
        cout << "B::vfunc1()" << endl;
    }
    void fun2()
    {
        cout << "B::fun2()" << endl;
    }

private:
    int m_data3;
};

class C : public B
{
public:
    C() : m_data1(0), m_data4(0) {}
    virtual void vfunc1()
    {
        cout << "C::vfunc1()" << endl;
    }
    void fun2()
    {
        cout << "C::fun2()" << endl;
    }

private:
    int m_data1, m_data4;
};

void fun(A *p)
{
    p->vfunc1();
}

int main()
{
    A *pa = new A;
    B *pb = new B;
    C *pc = new C;
    fun(pa);
    fun(pb);
    fun(pc);

    B b;
    A a = (A)b;
    a.vfunc1();

    A *pa1 = &b;
    pa1->vfunc1();

    A *pa2 = new B;
    pa2->vfunc1();
    A *pa3 = new C;
    pa3->vfunc1();

    return 0;
}
