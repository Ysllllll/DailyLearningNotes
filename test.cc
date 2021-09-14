#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    int num;
    cin >> num;
    num = num / 3;
    int res = 0;
    for (int i = 1; i < num - 1; i++)
    {
        int left = i, right = num - i - left;
        while (left <= right)
        {
            if (i + left + right == num)
            {
                if (i == left && left == right)
                    res++;
                else if (i != left && left != right && right != i)
                    res += 6;
                else
                    res += 3;
                left++;
                right--;
            }
            if (i + left + right > num)
                right--;
            if (i + left + right < num)
                left++;
        }
    }
    cout << res << endl;

    return 0;
}

// int getNum(int num)
// {
//     num -= 10;
//     int res = -1;
//     if (num >= 0)
//     {
//         res = 4;
//         res += (num / 4 + (num % 4 == 0 ? 0 : 1));
//     }
//     return res;
// }
// int main()
// {
//     int num;
//     cin >> num;
//     vector<int> nums(num);
//     vector<int> res(num);
//     for (int i = 0; i < num; i++)
//         cin >> nums[i];
//     for (int i = 0; i < num; i++)
//     {
//         res[i] = getNum(nums[i]);
//         cout << res[i] << " ";
//     }
//     return 0;
// }

// int main()
// {

//     string s;
//     getline(cin, s);
//     vector<vector<int>> intervals;

//     for (int i = 0; i < s.size(); i++)
//     {
//         if (isdigit(s[i]))
//         {
//             intervals.push_back({s[i] - '0', s[i + 2] - '0'});
//             i = i + 2;
//         }
//     }

//     if (intervals.size() == 0)
//     {
//         cout << 0;
//         return 0;
//     }
//     sort(intervals.begin(), intervals.end(), [](const vector<int> &a, const vector<int> &b)
//          { return a[1] < b[1]; });
//     int count = 1;
//     int end = intervals[0][1];
//     for (int i = 1; i < intervals.size(); i++)
//     {
//         if (end <= intervals[i][0])
//         {
//             end = intervals[i][1];
//             count++;
//         }
//     }
//     cout << intervals.size() - count;

//     return 0;
// }

// #include <string.h>
// #include <stdio.h>
// #include <limits.h>
// #include <stdlib.h>
// #include <string>
// #include <vector>
// #include <iostream>
// #include <string>
// #include <iterator>
// #include <algorithm>
// #include <queue>
// #include <stack>
// #include <numeric>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <queue>
// #include <unordered_map>

// using namespace std;

// int main()
// {

// }

// int getSum(int x)
// {
//     int sum = 0;

//     while (x > 0)
//     {
//         sum += x % 10;
//         x = x / 10;
//     }
//     return sum;
// }

// struct cmp
// {
//     bool operator()(pair<int, int> &a, pair<int, int> &b)
//     {
//         return a.second < b.second;
//     }
// };

// int main()
// {
//     int num;
//     cin >> num;
//     unordered_map<int, int> count;
//     for (int i = 1; i <= num; i++)
//         count[getSum(i)]++;

//     priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> q;
//     for (auto &temp : count)
//         q.push(temp);

//     int res = 0;
//     int maxValue = q.top().second;
//     while (!q.empty() && maxValue == q.top().second)
//     {
//         res++;
//         q.pop();
//     }
//     cout << res << endl;

//     return 0;
// }
