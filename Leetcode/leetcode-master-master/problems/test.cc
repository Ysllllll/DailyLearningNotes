#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
using namespace std;

class Solution
{
public:
	void qsort(vector<int> &nums, int left, int right, int k)
	{
		if (left >= right)
			return;

		int picked = rand() % (right - left + 1) + left;
		swap(nums[left], nums[picked]);

		int temp = nums[left];
		int l = left;
		int r = right;

		while (l < r)
		{
			while (l < r && nums[r] < temp)
				r--;
			nums[l] = nums[r];
			while (l < r && temp <= nums[l])
				l++;
			nums[r] = nums[l];
		}
		nums[l] = temp;
		if (l == k)
			return;
		else if (k < l)
			qsort(nums, left, l - 1, k);
		else
			qsort(nums, l + 1, right, k);
	}

	int findKthLargest(vector<int> &nums, int k)
	{
		qsort(nums, 0, nums.size() - 1, k);
		return nums[k - 1];
	}
};

int main()
{
	vector<int> test{1, 1, 1, 1, 1, 1, 1, 1};
	cout << Solution().findKthLargest(test, 5) << endl;

	return 0;
}

class Solution
{
public:
	void quickSort(vector<pair<int, int>> &rate, int left, int right, int k)
	{
		if (left >= right)
			return;
		int l = left;
		int r = right;
		int pivot = rate[left].second;
		while (l < r)
		{
			while (l < r && rate[r].second <= pivot)
				r--;
			while (l < r && rate[l].second >= pivot)
				l++;
			if (l < r)
				swap(rate[r], rate[l]);
		}
		swap(rate[left], rate[l]);
		if (l - left + 1 == k)
			return;
		else if (l - left + 1 > k)
			quickSort(rate, left, l - 1, k);
		else
			quickSort(rate, l + 1, right, k - (l - left + 1));
	}
	void quickSort(vector<pair<int, int>> &rate, int left, int right, int k)
	{
		if (left >= right)
			return;
		int l = left;
		int r = right;
		int pivotf = rate[left].first;
		int pivots = rate[left].second;

		while (l < r)
		{
			while (l < r && rate[r].second < pivots)
				r--;
			rate[l] = rate[r];
			while (l < r && rate[l].second >= pivots)
				l++;
			rate[r] = rate[l];
		}
		rate[l] = pair<int, int>{pivotf, pivots};
		if (l == k)
			return;
		else if (l > k)
			quickSort(rate, left, l - 1, k);
		else
			quickSort(rate, l + 1, right, k);
	}
	vector<int> topKFrequent(vector<int> &nums, int k)
	{
		unordered_map<int, int> map;
		for (auto &num : nums)
		{
			map[num]++;
		}
		vector<pair<int, int>> rate;
		for (auto &keyVaule : map)
		{
			rate.push_back(keyVaule);
		}
		quickSort(rate, 0, rate.size() - 1, k);
		vector<int> ans(k, 0);
		for (int i = 0; i < k; i++)
		{
			ans[i] = rate[i].first;
		}
		return ans;
	}
};