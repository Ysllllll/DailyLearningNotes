#include <iostream>

using namespace std;

namespace shao_smart_point
{

	//智能指针内部引用计数类实现
	class smart_cout
	{
	private:
		int use_count;

	public:
		smart_cout() : use_count(0)
		{
		}

		smart_cout(int count) : use_count(count)
		{
		}

		int addRef()
		{
			++use_count;
			return use_count;
		}

		int releaseRef()
		{
			--use_count;
			return use_count;
		}

		int getCount()
		{
			return use_count;
		}
	};

	//智能指针实现原理
	template <class T>
	class smart_point
	{

	private:
		T *pref;
		smart_cout *smartCount;

	public:
		//构造函数
		smart_point() : pref(nullptr), smartCount(nullptr)
		{
		}

		//构造函数
		smart_point(T *data) : pref(data), smartCount(new smart_cout())
		{
			if (data != nullptr && smartCount != nullptr)
			{
				smartCount->addRef();
			}
		}

		//拷贝构造
		smart_point(const smart_point<T> &smartPoint)
		{
			//进行浅拷贝
			this->pref = smartPoint.pref;
			this->smartCount = smartPoint.smartCount;

			//引用加1
			if (this->smartCount != nullptr)
			{
				this->smartCount->addRef();
			}
		}

		//赋值重载实现
		void operator=(const smart_point<T> &smartPoint)
		{

			//清理自己之前的指向
			if (this->pref != nullptr && this->smartCount->releaseRef() <= 0)
			{
				delete this->pref;
				delete this->smartCount;
				this->pref = nullptr;
			}

			//浅拷贝
			this->pref = smartPoint.pref;
			this->smartCount = smartPoint.smartCount;
			this->smartCount->addRef();
		}

		//重新指向
		void reset(T *data)
		{

			if (this->pref != nullptr && this->smartCount->releaseRef() <= 0)
			{
				delete this->pref;
				delete this->smartCount;
				this->pref = nullptr;
			}

			this->pref = data;
			if (this->pref != nullptr)
			{

				this->smartCount = new smart_cout(1);
			}
			else
			{

				this->smartCount = nullptr;
			}
		}

		void reset(const smart_point<T> &smartPoint)
		{

			if (this->pref != nullptr && this->smartCount->releaseRef() <= 0)
			{
				delete this->pref;
				delete this->smartCount;
				this->pref = nullptr;
			}

			this->pref = smartPoint.pref;
			this->smartCount = smartPoint.smartCount;
			if (this->pref != nullptr)
			{
				this->smartCount->addRef();
			}
		}

		bool operator==(const smart_point<T> &smartPoint)
		{
			return this->pref == smartPoint.pref;
		}

		T &operator*()
		{
			return *pref;
		}

		T *get()
		{
			return pref;
		}

		int useCount()
		{
			if (smartCount != nullptr)
			{
				return smartCount->getCount();
			}
		}

		//析构函数
		~smart_point()
		{
			if (pref != nullptr && smartCount->releaseRef() <= 0)
			{
				//释放指针
				delete pref;
				delete smartCount;
				pref = nullptr;
				cout << "delete" << endl;
			}
		}
	};

} // namespace shao_smart_point

void test()
{

	shao_smart_point::smart_point<int> mSmart(new int);
	shao_smart_point::smart_point<int> mSmart2(mSmart);
	mSmart2.reset(new int);

	cout << mSmart.useCount() << endl;
	cout << mSmart2.useCount() << endl;
}

int main()
{
	test();
	cin.get();
	return 0;
}