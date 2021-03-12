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
	int jump(vector<int> &nums)
	{
		int ans = 0;

		int curDistance = 0;
		int nextDistance = 0;

		for (int i = 0; i < nums.size() - 1; i++)
		{
			nextDistance = max(nums[i] + i, nextDistance);
			if (i == curDistance)
			{
				curDistance = nextDistance;
				ans++;
			}
		}

		return ans;
	}
};
int main()
{
	vector<int> test{2, 3, 1, 1, 4};
	cout << Solution().jump(test) << endl;

	return 0;
}