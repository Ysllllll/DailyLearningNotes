#include <iostream>
#include <math.h>
#include <vector>
#include <stack>
#include <string>
using namespace std;

class Solution
{
public:
    vector<int> mostCompetitive(vector<int> &nums, int k)
    {
        vector<int> res;

        for (int i = 0; i < nums.size(); i++)
        {
            while ((!res.empty()) && (res.back() > nums[i]) || (res.size() > k))
            {
                res.pop_back();
            }
            res.push_back(nums[i]);
        }

        return res;
    }
};

int main()
{
    vector<int> res{3, 5, 2, 6};
    Solution().mostCompetitive(res, 2);
    return 0;
}