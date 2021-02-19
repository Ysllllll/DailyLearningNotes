#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class singleton
{
public:
    static singleton &createSingleTon() //如果只有这一个create，没有下面这个create，则表明不允许外界创建对象，且外界只能使用一个对象。
    {
        static singleton a;
        return a;
    }
    static singleton *create() //这个create表明只能创建堆对象，不能创建栈对象
    {
        return new singleton;
    }
    void destory()
    {
        delete this;
    }

private:
    singleton() {}
    ~singleton() {}
};

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
        //cout << "ctor without parameter, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    complex(double r, double i) : real(r), img(i)
    {
        //cout << "ctor with parameter, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    complex(const complex &rhs) : real(rhs.real), img(rhs.img)
    {
        //cout << "copy ctor, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    ~complex()
    {
        //cout << "dtor, this = " << this << ", real = " << real << ", img = " << img << endl;
    } //使用编译器自动生成的析构函数

    complex &operator+=(const complex &r);

    double get_real() const
    {
        return real;
    }
    double get_img() const
    {
        return img;
    }

private:
    double real;
    double img;
    friend complex &__doapl(complex *, const complex &); //可以拿到参数类对象的私有数据成员
};

complex &complex::operator+=(const complex &r)
{
    return __doapl(this, r);
}
complex &__doapl(complex *ths, const complex &r)
{
    ths->real += r.real;
    ths->img += r.img;
    return *ths;
}

double img(complex const x)
{
    return x.get_img();
}
double real(complex const x)
{
    return x.get_real();
}

//双目运算符：+
complex operator+(const complex &x, const complex &y)
{
    return complex(real(x) + real(y), img(x) + img(y));
}
complex operator+(const complex &x, const double &y)
{
    return complex(real(x) + y, img(x));
}
complex operator+(const double &x, const complex &y)
{
    return complex(real(y) + x, img(y));
}
//双目运算符：-
complex operator-(const complex &x, const complex &y)
{
    return complex(real(x) - real(y), img(x) - img(y));
}
complex operator-(const complex &x, const double &y)
{
    return complex(real(x) - y, img(x));
}
complex operator-(const double &x, const complex &y)
{
    return complex(x - real(y), -img(y));
}
//双目运算符：*
complex operator*(const complex &x, const complex &y)
{
    return complex(real(x) * real(y) - img(x) * img(y), real(x) * img(y) + img(x) * real(y));
}
complex operator*(const complex &x, const double &y)
{
    return complex(real(x) * y, img(x) * y);
}
complex operator*(const double &x, const complex &y)
{
    return complex(x * real(y), x * img(y));
}
//双目运算符：/
complex operator/(const complex &x, const double &y)
{
    return complex(real(x) / y, img(x) / y);
}
complex operator/(const double &x, const complex &y)
{
    double den = real(y) * real(y) + img(y) * img(y);
    return complex(real(y) / den, -img(y) / den);
}
complex operator/(const complex &x, const complex &y)
{
    return x * (1 / y);
}
//双目运算符：==
bool operator==(const complex &x, const complex &y)
{
    return (abs(real(x) - real(y)) < 1e-7) && (abs(img(x) - img(y)) < 1e-7);
}
bool operator==(const complex &x, double &y)
{
    return (abs(real(x) - y) < 1e-7) && (abs(img(x)) < 1e-7);
}
bool operator==(double &x, const complex &y)
{
    return (abs(x - real(y)) < 1e-7) && (abs(img(y)) < 1e-7);
}
//双目运算符：!=
bool operator!=(const complex &x, const complex &y)
{
    return !(x == y);
}
bool operator!=(const complex &x, double &y)
{
    return !(x == y);
}
bool operator!=(double &x, const complex &y)
{
    return !(x == y);
}

//单目运算符：+、-、conj、norm
complex operator+(const complex &x) //主要是靠参数的个数来判断是“正”还是“加”运算
{
    return x;
}
complex operator-(const complex &x)
{
    return complex(-real(x), -img(x));
}
complex conj(const complex &x)
{
    return complex(real(x), -img(x));
}
double norm(const complex &x)
{
    return real(x) * real(x) + img(x) * img(x);
}

ostream &
operator<<(ostream &os, const complex &rhs)
{
    return os << "(" << real(rhs) << "," << img(rhs) << ")";
}
