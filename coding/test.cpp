#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution
{
public:
    vector<string> permutation(string s)
    {
        if (s.empty())
            return {};

        // 对字符串进行排序
        sort(s.begin(), s.end());
        vector<string> res;
        // 标记字符是否遍历过
        vector<bool> visit(s.size(), false);
        string track;
        backtrack(res, s, track, visit);

        return res;
    }

    void backtrack(vector<string> &res, string s, string &track, vector<bool> &visit)
    {
        // 回溯结束条件
        if (track.size() == s.size())
        {
            res.push_back(track);
            return;
        }

        // 选择和选择列表
        for (int i = 0; i < s.size(); i++)
        {
            // 排除不合法的选择
            if (visit[i])
                continue;

            if (i > 0 && !visit[i - 1] && s[i - 1] == s[i])
                continue;

            visit[i] = true;

            // 做选择
            track.push_back(s[i]);
            // 进入下一次决策树
            backtrack(res, s, track, visit);
            // 撤销选择
            track.pop_back();
            visit[i] = false;
        }
    }
};

int main()
{
    int a = (0 % 2 - 1);
    printf("%d\n", a);
    return 0;
}
