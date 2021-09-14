#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <mutex>
#include <map>
#include <limits.h>
#include <queue>
//#include <test.h>
using namespace std;

class Solution1
{
public:
	int repeatedSubstringPattern(string s)
	{
		vector<int> count0(s.size(), 0);
		vector<int> count1(s.size(), 0);
		count0[0] = s[0] == '0' ? 1 : 0;
		count1[0] = s[0] == '1' ? 1 : 0;
		for (int i = 1; i < s.size(); i++)
		{
			count0[i] = s[i] == '0' ? count0[i - 1] + 1 : count0[i - 1];
			count1[i] = s[i] == '1' ? count1[i - 1] + 1 : count1[i - 1];
		}

		vector<int> dp(s.size(), 1);
		for (int i = 1; i < s.size(); i++)
		{
			for (int j = 0; j < i; j++)
			{
				if (count0[j] * (count1[i] - count1[j]) == (count0[i] - count0[j]) * count1[j])
					dp[i] = max({dp[i], dp[j] + 1});
			}
		}

		for (auto c : dp)
			cout << c << " ";
		cout << endl;
	}
};

class Solution2
{
public:
	string mostCompetitive(string nums, int k)
	{

		string res;
		for (int i = 0; i < nums.size(); i++)
		{
			while (!res.empty() && (res.back() < nums[i]) && (nums.size() - i > k - res.size()))
			{
				res.pop_back();
			}
			if (res.size() < k)
				res.push_back(nums[i]);
		}
		return res;
	}
};

class Solution3
{
public:
	int repeatedSubstringPattern(string s)
	{
		vector<int> count0(s.size() + 1, 0);
		vector<int> count1(s.size() + 1, 0);
		for (int i = 1; i <= s.size(); i++)
		{
			count0[i] = s[i - 1] == '0' ? count0[i - 1] + 1 : count0[i - 1];
			count1[i] = s[i - 1] == '1' ? count1[i - 1] + 1 : count1[i - 1];
		}
		for (auto c : count0)
			cout << c << " ";
		cout << endl;
		for (auto c : count1)
			cout << c << " ";
		cout << endl;

		int res = 0;
		for (int i = 1; i <= s.size(); i++)
		{
			for (int j = 0; j < i; j++)
				res = max(res, (count0[i] - count0[j] - count1[i] + count1[j]));
		}

		return res;
	}
};

class Solution
{
public:
	int repeatedSubstringPattern(vector<int> vec)
	{
		int res = 0, mod = 1000000007, mul = 1, base = 2;
		sort(vec.begin(), vec.end());
		for (int i = 0; i < vec.size(); i++)
		{
			res = ((long)vec[i] * mul + res) % mod;
			mul = base * mul % mod;
		}

		return res;
	}
};
class Solution10
{
public:
	int repeatedSubstringPattern(vector<int> &vec1, vector<int> &vec2)
	{

		vector<int> ret;
		int main1 = vec1.size() - 1;
		int main2 = vec2.size() - 1;

		int i = vec2.size() - 1, j = vec1.size() - 2;
		while (true)
		{
			int v1 = main1 >= 0 ? vec1[main1] + vec2[i] : INT_MIN;
			int v2 = main2 >= 0 ? vec2[main2] + vec1[j] : INT_MIN;

			if (v1 == INT_MIN || v2 == INT_MIN)
				break;
			if (v1 >= v2)
			{
				ret.push_back(v1);
				i--;
				if (i < 0)
				{
					i = main2 - 1;
					main1--;
				}
			}
			if (v1 < v2)
			{
				ret.push_back(v2);
				j--;
				if (j < 0)
				{
					j = main1 - 1;
					main2--;
				}
			}
		}
		for (auto c : ret)
			cout << c << " ";
		cout << endl;

		return 0;
	}
};

struct cmp
{
	bool operator()(int a, int b)
	{
		return a < b;
	}
};

int main()
{
	// vector<int> a = {9, 4, 5, 6, 8, 1, 2, 4, 0, 5};
	// priority_queue<int, vector<int>, cmp> q(a.begin(), a.end());
	// while (!q.empty())
	// {
	// 	cout << q.top() << " ";
	// 	q.pop();
	// }
	// cout << endl;

	//char a = 'sdfwe';
	char *a = "sadg";
	//cout << a[1];
	//base().callprint();
	//sub().callprint();
	return 0;
}
