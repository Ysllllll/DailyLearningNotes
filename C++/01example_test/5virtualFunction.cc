#include <iostream>

struct A
{
	virtual void foo0();
	virtual void foo1();
};

struct B : public A
{
	virtual void foo0();
};

int main()
{
	B b{};

	sizeof(b);

	return 0;
}