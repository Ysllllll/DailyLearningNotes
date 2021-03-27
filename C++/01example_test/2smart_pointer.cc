#include <iostream>

using namespace std;

namespace MySmartPoint
{
	class SmartCount
	{
	private:
		int use_count;

	public:
		SmartCount() : use_count(0) {}
		SmartCount(int count) : use_count(count) {}
		int addRef()
		{
			use_count++;
			return use_count;
		}
		int releaseRef()
		{
			use_count--;
			return use_count;
		}
		int getCount()
		{
			return use_count;
		}
	};

	template <class T>
	class SmartPoint
	{
	private:
		T *data;
		SmartCount *smart_count;

	public:
		SmartPoint() : data(nullptr), smart_count(nullptr) {}
		SmartPoint(const SmartPoint<T> &smart_point)
		{
			this->data = smart_point.data;
			this->smart_count = smart_point.smart_count;

			if (this->smart_count != nullptr)
				this->smart_count->addRef();
		}
		SmartPoint(T *pdata) : data(pdata), smart_count(new SmartCount())
		{
			if (data != nullptr && smart_count != nullptr)
				smart_count->addRef();
		}
		void operator=(const SmartPoint<T> &smart_point)
		{
			if (this->data != nullptr && this->smart_count->releaseRef() <= 0)
			{
				delete this->data;
				delete this->smart_count;
				this->data = nullptr;
			}

			this->data = smart_point.data;
			this->smart_count = smart_point.smart_count;
			this->smart_count->addRef();
		}

		void reset(T *pdata)
		{
			if (this->data != nullptr && this->smart_count->releaseRef() <= 0)
			{
				delete this->data;
				delete this->smart_count;
				this->data = nullptr;
			}

			this->data = pdata;
			if (this->data != nullptr)
			{
				this->smart_count = new SmartCount(1);
			}
			else
			{
				this->smart_count = nullptr;
			}
		}

		void reset(const SmartPoint<int> smart_point)
		{
			if (this->data != nullptr && this->smart_count->releaseRef() <= 0)
			{
				delete this->data;
				delete this->smart_count;
				this->data = nullptr;
			}
			this->data = smart_point.data;
			this->smart_count = smart_point.smart_count;
			if (this->data != nullptr)
				this->smart_count->addRef();
		}

		T &operator*()
		{
			return *data;
		}
		T *get()
		{
			return data;
		}
		int useCount()
		{
			if (smart_count != nullptr)
				return smart_count->getCount();
		}
		~SmartPoint()
		{
			if (data != nullptr && smart_count->releaseRef() <= 0)
			{
				delete data;
				delete smart_count;
				data = nullptr;
				smart_count = nullptr;
				cout << "delete smart point" << endl;
				return;
			}
			cout << "delete smart ref" << endl;
		}
	};
} // namespace MySmartPoint

int main()
{
	MySmartPoint::SmartPoint<int> mSmart1(new int(10));
	MySmartPoint::SmartPoint<int> mSmart2(mSmart1);
	mSmart1.reset(new int);

	cout << mSmart1.useCount() << endl;
	cout << mSmart2.useCount() << endl;

	return 0;
}