#include "test.h"

#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

int main()
{
	vector<int> arr{1, 3};

	int target = 2;
	int left = 0;
	int right = arr.size() - 1;
	while (left < right)
	{
		int mid = left + (right - left) / 2;
		if (arr[mid] < target)
			left = mid + 1;
		else
			right = mid;
	}

	cout << "find: " << left << endl;

	return 0;
}