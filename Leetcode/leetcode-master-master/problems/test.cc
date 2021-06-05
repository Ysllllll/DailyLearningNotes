#include "test.h"

class MedianFinder
{
private:
	vector<int> maxHeap;
	vector<int> minHeap;
	static bool Greater(int a, int b)
	{
		return a >= b;
	}
	static bool Less(int a, int b)
	{
		return a <= b;
	}
	void shiftDown(vector<int> &vec, int left, int right, bool (*op)(int, int))
	{
		int temp = vec[left];

		for (int i = left * 2 + 1; i <= right; i = i * 2 + 1)
		{
			if (i + 1 <= right && op(vec[i + 1], vec[i]))
				i++;
			if (op(temp, vec[i]))
				break;
			vec[left] = vec[i];
			left = i;
		}
		vec[left] = temp;
	}
	void shiftUp(vector<int> &vec, int left, int right, bool (*op)(int, int))
	{
		int temp = vec[right];

		for (int i = (right - 1) / 2; i >= left; i = (i - 1) / 2)
		{
			if (op(vec[i], temp))
				break;
			vec[right] = vec[i];
			right = i;
			if (right == left)
				break;
		}
		vec[right] = temp;
	}

public:
	/** initialize your data structure here. */
	MedianFinder()
	{
	}

	void addNum(int num)
	{

		maxHeap.push_back(num);
		shiftUp(maxHeap, 0, maxHeap.size() - 1, Greater);

		int maxSize = maxHeap.size();
		int minSize = minHeap.size();
		if (maxSize - minSize >= 2)
		{
			minHeap.push_back(maxHeap[0]);
			shiftUp(minHeap, 0, minSize, Less);
			maxHeap[0] = maxHeap[maxSize - 1];
			maxHeap.pop_back();
			shiftDown(maxHeap, 0, maxSize - 2, Greater);
		}

		if (minHeap.size() > 0 && minHeap[0] < maxHeap[0])
		{
			swap(minHeap[0], maxHeap[0]);
			shiftDown(minHeap, 0, minHeap.size() - 1, Less);
			shiftDown(maxHeap, 0, maxHeap.size() - 1, Greater);
		}
	}

	double findMedian()
	{
		if (maxHeap.size() == minHeap.size())
			return (double(maxHeap[0] + minHeap[0])) / 2;
		else
			return maxHeap[0];
	}
};

int main()
{

	MedianFinder *obj = new MedianFinder();
	obj->addNum(1);
	obj->addNum(2);
	obj->addNum(3);
	obj->addNum(4);
	obj->addNum(5);
	obj->addNum(6);
	obj->addNum(7);
	cout << obj->findMedian() << endl;
	obj->addNum(8);
	cout << obj->findMedian() << endl;
	obj->addNum(9);
	cout << obj->findMedian() << endl;
	obj->addNum(10);

	return 0;
}