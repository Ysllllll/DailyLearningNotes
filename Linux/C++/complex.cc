#include <iostream>
#include <vector>
using namespace std;

class singleton
{
public:
    static singleton &createSingleTon(); //如果只有这一个create，没有下面这个create，则表明不允许外界创建对象，且外界只能使用一个对象。
    static singleton *create();          //这个create表明只能创建堆对象，不能创建栈对象
    void destory();

private:
    singleton() {}
    ~singleton() {}
};
singleton &singleton::createSingleTon()
{
    static singleton a;
    return a;
}
singleton *singleton::create()
{
    return new singleton;
}
void singleton::destory()
{
    delete this;
}

class complex
{
    //成员函数、非成员函数
    //操作符函数：重载为成员函数、重载为非成员函数
    //const运用
    //pass by value / pass by reference
    //return by value / return by reference
    //ctor / copy ctor/ copy assignment operator
public:
    complex() : real(0), img(0)
    {
        cout << "ctor without parameter, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    complex(double r, double i) : real(r), img(i)
    {
        cout << "ctor with parameter, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    complex(const complex &rhs) : real(rhs.real), img(rhs.img)
    {
        cout << "copy ctor, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    ~complex()
    {
        cout << "dtor, this = " << this << ", real = " << real << ", img = " << img << endl;
    } //使用编译器自动生成的析构函数

    double get_real() const;
    double get_img() const;

private:
    double real;
    double img;
    friend complex &__doapl(complex *, const complex &); //可以拿到参数类对象的私有数据成员
};

double complex::get_real() const //const与static关键字不同，它也是重载的一个标识，在类外定义的时候要加上。而static不能加上
{
    return real;
}
double complex::get_img() const
{
    return img;
}

ostream &operator<<(ostream &os, complex &rhs)
{
    os << "(" << rhs.get_real() << "," << rhs.get_img() << ")" << endl;
    return os;
}
complex &__doapl(complex *ths, const complex &r)
{
    ths->real += r.real;
    ths->img += r.img;
}

int main()
{
    singleton::createSingleTon();

    //complex tmp1(2.0, 3.0);
    cout
        << "sizeof complex = " << sizeof(complex) << endl;

    complex tmp = complex(5.0, 4.0);

    complex *p1 = new complex(2, 2);

    complex *p2 = new complex();

    complex *p3 = new complex[3];

    complex *temp = p3;

    cout << "p3 = " << p3 << " temp = " << temp << endl;
    for (int i = 0; i < 3; i++)
        new (temp++) complex(i, i);
    cout << "p3 = " << p3 << " temp = " << temp << endl;

    delete p1;
    delete p2;
    delete[] p3;

    return 0;
}