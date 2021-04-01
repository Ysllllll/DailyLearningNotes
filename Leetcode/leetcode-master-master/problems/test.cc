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

class Solution
{
public:
	int maxProfit(vector<int> &prices)
	{
		if (prices.size() == 0)
			return 0;
		vector<int> dp(prices.size(), 0);

		for (int i = 1; i < prices.size(); i++)
		{
			dp[i] = max(dp[i - 1], dp[i - 1] + prices[i] - prices[i - 1]);
		}
		return dp[prices.size() - 1];
	}
};

int main()
{
	vector<int> weight = {7, 1, 5, 3, 6, 4};

	cout << Solution().maxProfit(weight) << endl;

	return 0;
}