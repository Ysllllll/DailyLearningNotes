#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
using namespace std;

int main()
{

	int bag = 1;
	vector<int> nums = {1, 2, 3};

	int sum = 0;
	for (int e : nums)
		sum += e;
	vector<int> dp(sum + 1, 0);
	dp[0] = 0;

	if (bag > sum)
		cout << -1 << endl;

	for (int e : nums)
	{
		for (int i = nums.size(); i >= e; i--)
		{
			dp[i] = dp[i - e] + 1;
		}
		for (int c : dp)
			cout << c << " ";
		cout << endl;
	}

	int res = INT_MAX;
	for (int i = bag; i <= sum; i += bag)
		res = min(res, dp[i]);
	if (res == INT_MAX)
		cout << -1 << endl;
	else
		cout << res << endl;
	return -1;
}