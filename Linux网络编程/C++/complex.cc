#include <iostream>
using namespace std;

class complex
{
    //成员函数、非成员函数
    //操作符函数：重载为成员函数、重载为非成员函数
    //const运用
    //pass by value / pass by reference
    //return by value / return by reference
    //ctor / copy ctor/ copy assignment operator
public:
    complex(double r, double i) : real(r), img(i)
    {
        cout << "ctor, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    complex(const complex &rhs) : real(rhs.real), img(rhs.img)
    {
        cout << "copy ctor, this = " << this << ", real = " << real << ", img = " << img << endl;
    }
    ~complex()
    {
        cout << "dtor, this = " << this << ", real = " << real << ", img = " << img << endl;
    } //使用编译器自动生成的析构函数

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
};

ostream &operator<<(ostream &os, complex &rhs)
{
    os << "(" << rhs.get_real() << "," << rhs.get_img() << ")" << endl;
    return os;
}

int main()
{
    complex tmp = complex(5.0, 4.0);
    cout << tmp << endl;
    return 0;
}