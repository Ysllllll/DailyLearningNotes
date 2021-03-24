#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
using namespace std;

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
int main()
{
	vector<int> test{4, 16, 37, 82, 122, 123};
	cout << Solution().numbers(test) << endl;

	return 0;
}