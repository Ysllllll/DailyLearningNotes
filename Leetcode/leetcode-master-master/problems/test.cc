#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <bitset>
using namespace std;
/*
class Solution
{
public:
	static bool cmp(pair<int, int> a, pair<int, int> b)
	{
		return a.first < b.first;
	}

	int numbers(vector<int> &a)
	{
		int base = 1, res = 0, n = a.size();

		unordered_map<int, int> tmp_record;
		sort(a.begin(), a.end());
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i] > base * base)
			{
				while (a[i] > base * base)
					base++;
			}

			if (a[i] < base * base && a[i] > (base - 1) * (base - 1))
			{
				int min_diff = min(a[i] - (base - 1) * (base - 1), base * base - a[i]);
				tmp_record[min_diff]++;
			}
			else if (a[i] == base * base)
			{
				tmp_record[0]++;
			}
		}

		vector<pair<int, int>> record;
		for (auto &tmp : tmp_record)
			record.push_back(tmp);

		sort(record.begin(), record.end(), cmp);

		n = n / 2;
		for (auto it = record.begin(); it != record.end() && n > 0; it++)
		{
			if (n - it->second > 0)
			{
				n = n - it->second;
				res += it->first * it->second;
			}
			else
			{
				res += it->first * n;
				break;
			}
		}

		return res;
	}
};
*/

class Solution1
{
public:
	//测试集：{0,0,0} {6,12} {6, 12, 60} {1, 7, 3}
	int findMaxForm(vector<int> &nums)
	{
		if (nums.size() == 0)
			return -1;

		int max_sum = 0;
		helper(nums, 0, 0, max_sum);
		if (max_sum == 0)
			return -1;
		else
			return max_sum;
	}
	void helper(vector<int> &nums, int i, int sum, int &max_sum)
	{
		if (i == nums.size())
		{
			if (sum > max_sum && (sum % 6 == 0))
				max_sum = sum;
		}
		else
		{
			helper(nums, i + 1, sum + nums[i], max_sum);
			helper(nums, i + 1, sum, max_sum);
		}
	}
};

class Solution
{
public:
	//测试集：{0,0,0} {6,12} {6, 12, 60} {1, 7, 3}
	int findMaxForm(vector<int> &nums)
	{
		int sum = 0;
		for (int e : nums)
			sum += e;
		vector<int> dp(sum + 1, 0);

		for (int e : nums)
			for (int j = sum; j >= e; j--)
				dp[j] = max(dp[j], dp[j - e] + e);

		for (int i = sum; i > 0; i--)
			if (dp[i] % 6 == 0 && (dp[i] != 0))
				return dp[i];

		return -1;
	}
};

int main()
{

	int n;
	cin >> n;
	vector<int> nums(n, 0);
	for (int i = 0; i < n; i++)
		cin >> nums[i];
	cout << Solution().findMaxForm(nums) << endl;

	return 0;
}