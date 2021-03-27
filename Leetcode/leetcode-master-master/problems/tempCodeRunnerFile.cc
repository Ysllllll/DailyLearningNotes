
class Solution
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