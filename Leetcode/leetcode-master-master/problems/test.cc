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
#include <sstream>
using namespace std;

struct TreeNode
{
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution
{
public:
	vector<int> mostCompetitive(vector<int> &nums, int k)
	{
		vector<int> res;
		k = nums.size() - k;

		for (int i = 0; i < nums.size(); i++)
		{
			while ((res.size() != 0) && (res.back() > nums[i]) && (k != 0))
			{
				res.pop_back();
				k--;
			}
			res.push_back(nums[i]);
		}

		while (k != 0)
			res.pop_back();

		return res;
	}
};
int main()
{
	// TreeNode *root = new TreeNode(4);
	// root->left = new TreeNode(3);
	// root->left->left = new TreeNode(1);
	// root->left->right = new TreeNode(2);
	// root->right = nullptr;
	// Solution().maxSumBST(root);

	vector<int> res{2, 4, 3, 3, 5, 4, 9, 6};
	Solution().mostCompetitive(res, 4);

	return 0;
}