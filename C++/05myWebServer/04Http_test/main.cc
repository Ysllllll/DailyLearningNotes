#include "HttpData.h"
#include <iostream>
#include <memory>

class A
{
public:
	~A()
	{
		std::cout << "dealloc A" << std::endl;
	}
};

class B : public A
{
public:
	~B()
	{
		std::cout << "dealloc B" << std::endl;
	}
};

class D;
class C
{
public:
	std::shared_ptr<D> d;
};
class D
{
public:
	std::shared_ptr<C> c;
};

int main()
{
	//第一种构造shared_ptr的方法
	//int *p = new int(10);
	//std::shared_ptr<int> sp = p;

	//第二种构造方法
	std::shared_ptr<int> sp(new int(10));

	//第三种构造方法
	std::shared_ptr<int> sp1 = std::make_shared<int>(10);
	std::shared_ptr<int> spp = sp1;
	std::cout << spp.use_count() << std::endl;

	A *a = new B; //若A中的析构函数是虚函数，输出和下面用shared_ptr是一样的
	delete a;

	std::shared_ptr<A> sp2 = std::make_shared<B>();

	//创建数组智能指针
	//std::shared_ptr<int> sp3 = std::make_shared_array(10); 不可使用
	std::unique_ptr<int[]> sp3(new int[10]);

	//测试循环引用
	std::shared_ptr<C>
		spc = std::make_shared<C>();
	std::shared_ptr<D> spd(new D());
	spc->d = spd;
	spd->c = spc;
	return 0;
}