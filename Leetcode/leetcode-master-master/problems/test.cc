#include "test.h"

class Solution
{
private:
	vector<int> data;
	int capacity;
	int size;

public:
	int size()
	{
		return size;
	}
	int capacity()
	{
		return capacity;
	}
	bool isEmpty()
	{
		return size == 0;
	}
	int peek()
	{
		if (size == 0)
			return INT_MIN;
		else
			return data[0];
	}
	void shiftUp()
	{
	}
	void shiftDown()
	{
	}
};

int main()
{

	return 0;
}