//testGdb.c
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
/*
 * test1：测试仿函数
 * test2：测试模板特化
 * test3：测试继承
 * test4：结合数组测试placement new
 * test5：结合类对象测试placement new
 * test6：内存池测试
 * 
 */

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
		stringAppend my("and world!"); //调用构造函数
		my("hello");				   //调用operator()
		stringAppend()("Yes");		   //创建一个临时函数对象，然后进行函数调用
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

namespace test3 //测试继承
{
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
} // namespace test3

#include <iostream>
#include <new>	// for placement new
namespace test4 //结合数组测试placement new
{
	// newplace.cpp -- using placement new

	const int BUF = 512;
	const int N = 5;
	char buffer[BUF]; // chunk of memory
	int main()
	{
		using namespace std;

		double *pd1, *pd2;
		int i;
		cout << "Calling new and placement new:\n";
		pd1 = new double[N];		  // use heap
		pd2 = new (buffer) double[N]; // use buffer array
		for (i = 0; i < N; i++)
			pd2[i] = pd1[i] = 1000 + 20.0 * i;
		cout << "Memory addresses:\n"
			 << "  heap: " << pd1						//常规new在堆上分配内存的首地址
			 << "  static: " << (void *)buffer << endl; //布局new在buffer上分配的首地址
		cout << "Memory contents:\n";
		for (i = 0; i < N; i++)
		{
			cout << pd1[i] << " at " << &pd1[i] << "; ";
			cout << pd2[i] << " at " << &pd2[i] << endl;
		}

		cout << "\nCalling new and placement new a second time:\n";
		double *pd3, *pd4;
		pd3 = new double[N];		  // find new address
		pd4 = new (buffer) double[N]; // overwrite old data
		for (i = 0; i < N; i++)
			pd4[i] = pd3[i] = 1000 + 40.0 * i;
		cout << "Memory contents:\n";
		for (i = 0; i < N; i++)
		{
			cout << pd3[i] << " at " << &pd3[i] << "; ";
			cout << pd4[i] << " at " << &pd4[i] << endl;
		}

		cout << "\nCalling new and placement new a third time:\n";
		delete[] pd1;
		pd1 = new double[N];
		pd2 = new (buffer + N * sizeof(double)) double[N];
		for (i = 0; i < N; i++)
			pd2[i] = pd1[i] = 1000 + 60.0 * i;
		cout << "Memory contents:\n";
		for (i = 0; i < N; i++)
		{
			cout << pd1[i] << " at " << &pd1[i] << "; ";
			cout << pd2[i] << " at " << &pd2[i] << endl;
		}
		delete[] pd1;
		delete[] pd3;
		// cin.get();
		return 0;
	}
} // namespace test4

#include <iostream>
#include <string>
#include <new>
using namespace std;
namespace test5 //结合类对象测试placement new
{
	const int BUF = 512;

	class JustTesting
	{
	private:
		string words;
		int number;

	public:
		JustTesting(const string &s = "Just Testing", int n = 0)
		{
			words = s;
			number = n;
			cout << words << " constructed\n";
		}
		~JustTesting() { cout << words << " destroyed\n"; }
		void Show() const { cout << words << ", " << number << endl; }
	};
	int main()
	{
		char *buffer = new char[BUF]; // get a block of memory

		JustTesting *pc1, *pc2;

		pc1 = new (buffer) JustTesting;		// place object in buffer
		pc2 = new JustTesting("Heap1", 20); // place object on heap

		cout << "Memory block addresses:\n"
			 << "buffer: "
			 << (void *)buffer << "    heap: " << pc2 << endl;
		cout << "Memory contents:\n";
		cout << pc1 << ": ";
		pc1->Show();
		cout << pc2 << ": ";
		pc2->Show();

		JustTesting *pc3, *pc4;
		// fix placement new location
		pc3 = new (buffer + sizeof(JustTesting))
			JustTesting("Better Idea", 6); //非优化版pc3 = new (buffer) JustTesting("Bad Idea", 6);
		pc4 = new JustTesting("Heap2", 10);

		cout << "Memory contents:\n";
		cout << pc3 << ": ";
		pc3->Show();
		cout << pc4 << ": ";
		pc4->Show();

		delete pc2; // free Heap1
		delete pc4; // free Heap2
		// explicitly destroy placement new objects
		pc3->~JustTesting(); // destroy object pointed to by pc3   非优化版无此行
		pc1->~JustTesting(); // destroy object pointed to by pc1   非优化版无此行
		delete[] buffer;	 // free buffer
		// std::cin.get();
		return 0;
	}
} // namespace test5

#include <string.h>
#include <stdio.h>

namespace test6 //内存池测试
{
#define HEAPSIZE 500
#define malloc_addr heap
#define malloc_size HEAPSIZE

	static unsigned char has_initialized;
	static intptr_t managed_memory_start;
	static intptr_t managed_memory_end;
	static intptr_t managed_memory_size;
	unsigned char heap[HEAPSIZE];

	typedef struct
	{
		unsigned char is_available;
		unsigned int prior_blocksize;
		unsigned int current_blocksize;
	} mem_control_block;

	void malloc_init()
	{
		mem_control_block *mcb = NULL;
		//the type of managed_memory_start must be long, not unsigned int.
		//cast from "unsigned char*" to "unsigned int" lost precision
		//              8 bytes            4 bytes
		managed_memory_start = (intptr_t)malloc_addr;
		managed_memory_size = malloc_size;
		managed_memory_end = managed_memory_start + managed_memory_size;

		mcb = (mem_control_block *)managed_memory_start;
		mcb->is_available = 1;
		mcb->prior_blocksize = 0;
		mcb->current_blocksize = managed_memory_size - sizeof(mem_control_block);
		has_initialized = 1;
	}

	void *malloc(unsigned int numbytes)
	{
		intptr_t current_location, otherbck_loaction;
		mem_control_block *current_location_mcb = NULL, *otherbck_loaction_mcb = NULL;
		void *memory_location = NULL;
		unsigned int process_blocksize;

		if (!has_initialized)
			malloc_init();

		current_location = managed_memory_start;
		while (current_location != managed_memory_end)
		{
			current_location_mcb = (mem_control_block *)current_location;
			if (current_location_mcb->is_available)
			{
				if (current_location_mcb->current_blocksize == numbytes)
				{
					current_location_mcb->is_available = 0;
					memory_location = (void *)(current_location + sizeof(mem_control_block));
					break;
				}
				else if (current_location_mcb->current_blocksize >= numbytes + sizeof(mem_control_block))
				{
					current_location_mcb->is_available = 0;
					process_blocksize = current_location_mcb->current_blocksize;
					current_location_mcb->current_blocksize = numbytes;

					otherbck_loaction_mcb = (mem_control_block *)(current_location + numbytes + sizeof(mem_control_block));
					otherbck_loaction_mcb->is_available = 1;
					otherbck_loaction_mcb->prior_blocksize = numbytes;
					otherbck_loaction_mcb->current_blocksize = process_blocksize - numbytes - sizeof(mem_control_block);
					otherbck_loaction = current_location + numbytes + sizeof(mem_control_block);

					if (otherbck_loaction != managed_memory_end)
					{
						otherbck_loaction_mcb = (mem_control_block *)otherbck_loaction;
						otherbck_loaction_mcb->prior_blocksize = process_blocksize - numbytes - sizeof(mem_control_block);
					}
					memory_location = (void *)(current_location + sizeof(mem_control_block));
					break;
				}
			}
			current_location += current_location_mcb->current_blocksize + sizeof(mem_control_block);
		}
		return memory_location;
	}

	void free(void *firstbyte)
	{
		intptr_t current_location, otherbck_loaction;
		mem_control_block *current_mcb = NULL, *next_mcb = NULL, *prior_mcb = NULL, *other_mcb = NULL;
		current_location = (intptr_t)firstbyte - sizeof(mem_control_block);
		current_mcb = (mem_control_block *)current_location;
		current_mcb->is_available = 1;
		otherbck_loaction = current_location + sizeof(mem_control_block) + current_mcb->current_blocksize;

		if (otherbck_loaction != managed_memory_end) //这里可以改进
		{
			next_mcb = (mem_control_block *)otherbck_loaction;
			if (next_mcb->is_available)
			{
				current_mcb->current_blocksize += (sizeof(mem_control_block) + next_mcb->current_blocksize);

				otherbck_loaction = current_location + current_mcb->current_blocksize + sizeof(mem_control_block);
				if (otherbck_loaction != managed_memory_end)
				{
					other_mcb = (mem_control_block *)otherbck_loaction;
					other_mcb->prior_blocksize = current_mcb->current_blocksize;
				}
			}
		}

		if (current_location != managed_memory_start)
		{
			prior_mcb = (mem_control_block *)(current_location - sizeof(mem_control_block) - current_mcb->prior_blocksize);
			if (prior_mcb->is_available)
			{
				prior_mcb->current_blocksize += (sizeof(mem_control_block) + current_mcb->current_blocksize);

				otherbck_loaction = current_location + sizeof(mem_control_block) + current_mcb->current_blocksize;

				if (otherbck_loaction != managed_memory_end)
				{
					other_mcb = (mem_control_block *)otherbck_loaction;
					other_mcb->prior_blocksize = prior_mcb->current_blocksize;
				}
			}
		}
	}

	int main()
	{
		cout << "sizeof unsigned int " << sizeof(unsigned int) << endl;
		cout << "sizeof long " << sizeof(long) << endl;
		return 0;
	}
} // namespace test6

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <vector>
namespace test7 //一个简单的空间配置器
{
	/*
	 * 这是一个简单的空间配置器
	 * 将对象内存的分配和对象的构造函数的调用分开，反之亦然
	 * allocate()负责内存配置，deallocate()负责内存释放
	 * construct()负责对象构造，destroy()负责对象析构
	 */
	template <class T>
	inline T *_allocate(ptrdiff_t size, T *)
	{
		set_new_handler(0);
		T *tmp = (T *)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == nullptr)
		{
			cout << "allocate error" << endl;
		}
		return tmp;
	}

	template <class T>
	inline void _deallocate(T *buffer)
	{
		::operator delete(buffer);
	}

	template <class T1, class T2>
	inline void _construct(T1 *p, const T2 &value)
	{
		new (p) T1(value);
	}
	template <class T>
	inline void _destroy(T *p)
	{
		p->~T();
	}

	template <class T>
	class allocator
	{
	public:
		typedef T value_type;
		typedef T *pointer;
		typedef const T *const_pointer;
		typedef T &reference;
		typedef const T &const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template <class U>
		struct rebind
		{
			typedef allocator<U> other;
		};

		pointer allocate(size_type size, const void *hint = 0)
		{
			return _allocate((difference_type)size, (pointer)0);
		}
		void deallocate(pointer p, size_type n)
		{
			_deallocate(p);
		}
		void construct(pointer p, const_reference value)
		{
			_construct(p, value);
		}
		void destroy(pointer p)
		{
			_destroy(p);
		}
		pointer address(reference x)
		{
			return (pointer)&x;
		}
		const_pointer const_address(const_reference x)
		{
			return (const_pointer)&x;
		}
	};

	int main()
	{
		int ia[5] = {0, 1, 2, 3, 4};
		vector<int, test7::allocator<int>> iv(ia, ia + 5);
		for (int e : iv)
			cout << e << endl;

		return 0;
	}

} // namespace test7

#include <iostream>
#include <chrono>
#include <thread>
namespace test8 //测试时间复杂度
{
	using namespace std;
	using namespace chrono;
	// O(n)
	void function1(long long n)
	{
		long long k = 0;
		for (long long i = 0; i < n; i++)
		{
			k++;
		}
	}

	// O(n^2)
	void function2(long long n)
	{
		long long k = 0;
		for (long long i = 0; i < n; i++)
		{
			for (long j = 0; j < n; j++)
			{
				k++;
			}
		}
	}
	// O(nlogn)
	void function3(long long n)
	{
		long long k = 0;
		for (long long i = 0; i < n; i++)
		{
			for (long long j = 1; j < n; j = j * 2)
			{ // 注意这里j=1
				k++;
			}
		}
	}
	int main()
	{
		long long n; // 数据规模
		while (1)
		{
			cout << "输入n：";
			cin >> n;
			milliseconds start_time = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch());
			function1(n);
			//        function2(n);
			//        function3(n);
			milliseconds end_time = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch());
			cout << "耗时:" << milliseconds(end_time).count() - milliseconds(start_time).count()
				 << " ms" << endl;
		}
	}
} // namespace test8

namespace test9 // 测试static成员函数是否受public和private的限定
{
	class Solution
	{
	public:
		static int a;

	private:
		static int b;
	};
	int Solution::a = 10;
	int Solution::b = 11;
	int main()
	{
		cout << Solution::a << endl;
		//cout << Solution::b<<endl; // error static成员函数受private限定
		return 0;
	}
}

#include <memory>
#include <string_view>
namespace test10 // 测试智能指针
{
	class myPointer
	{
	private:
		int *m_count;
		int *m_value;

	public:
		myPointer(int v) : m_value(new int(v)), m_count(new int(1))
		{
			cout << "myPointer" << endl;
		}
		myPointer(const myPointer &p)
		{
			m_value = p.getValue();
			m_count = p.getCount();
			(*m_count)++;
		}
		myPointer &operator=(const myPointer &p)
		{
			if (&p == this)
				return *this;
			(*m_count)--;
			m_count = p.getCount();
			(*m_count)++;
			m_value = p.getValue();

			return *this;
		}
		~myPointer()
		{
			(*m_count)--;
			if (*m_count == 0)
			{
				delete m_value;
				delete m_count;
				cout << "~myPointer, delete myPointer" << endl;
				return;
			}
			else
				cout << "~myPointer, decrease pointer reference" << endl;
		}
		int *getValue() const
		{
			return m_value;
		}
		int *getCount() const
		{
			return m_count;
		}
		int &operator*()
		{
			return *m_value;
		}
	};

	void badThing()
	{
		throw(1);
	}

	class Child;
	class Parent
	{
	public:
		shared_ptr<Child> child;

		Parent()
		{
			cout << "Parent()" << endl;
		}
		~Parent()
		{
			cout << "~Parent()" << endl;
		}
	};

	class Child
	{

	public:
		shared_ptr<Parent> parent;

		Child()
		{
			cout << "Child()" << endl;
		}
		~Child()
		{
			cout << "~Child()" << endl;
		}
	};

	int main()
	{
		try
		{
			myPointer t(10);
			myPointer x(t);
			cout << "*t = " << *t << endl;
			cout << "*x = " << *x << endl;
			cout << "*t.getCount() = " << *t.getCount() << endl;
			cout << "*x.getCount() = " << *x.getCount() << endl;
			{
				myPointer y(x);
				cout << "*t = " << *t << endl;
				cout << "*x = " << *x << endl;
				cout << "*y = " << *y << endl;
				cout << "*t.getCount() = " << *t.getCount() << endl;
				cout << "*x.getCount() = " << *x.getCount() << endl;
				cout << "*y.getCount() = " << *y.getCount() << endl;
			}
			cout << "*t.getCount() = " << *t.getCount() << endl;
			cout << "*x.getCount() = " << *x.getCount() << endl;
			badThing();
		}
		catch (int i)
		{
			cerr << "badThing function has happen" << endl;
		}

		shared_ptr<Parent> parent(make_shared<Parent>());
		shared_ptr<Child> child(make_shared<Child>());
		parent->child = child;
		child->parent = parent;

		cout << parent.use_count() << endl;

		return 0;
	}
}
int main()
{

	test10::main();

	string s("123456789");
	string p(s);
	cout << p << endl;

	return 0;
}