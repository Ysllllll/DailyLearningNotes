<a id="base-solution"></a>

## 题目汇总

- **[84. 柱状图中最大的矩形](https://leetcode-cn.com/problems/largest-rectangle-in-histogram/)** [速览题解](#84)
- **[42. 接雨水](https://leetcode-cn.com/problems/trapping-rain-water/)** [速览题解](#42)
- **[739. 每日温度](https://leetcode-cn.com/problems/daily-temperatures/)** [速览题解](#739)
- **[496. 下一个更大元素 I](https://leetcode-cn.com/problems/next-greater-element-i/)** [速览题解](#496)
- **[503. 下一个更大元素 II](https://leetcode-cn.com/problems/next-greater-element-ii/)** [速览题解](#503)
- **[402. 移掉K位数字](https://leetcode-cn.com/problems/remove-k-digits/)** [速览题解](#402)
- **[316. 去除重复字母](https://leetcode-cn.com/problems/remove-duplicate-letters/)** [速览题解](#316)
- **[1673. 找出最具竞争力的子序列](https://leetcode-cn.com/problems/find-the-most-competitive-subsequence/)** [速览题解](#1673)
- **[901. 股票价格跨度](https://leetcode-cn.com/problems/online-stock-span/)** [速览题解](#901)
- **[581. 最短无序连续子数组](https://leetcode-cn.com/problems/shortest-unsorted-continuous-subarray/submissions/)** [速览题解](#581)

<a id="84"></a>

### [84. 柱状图中最大的矩形](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int res = 0;

        vector<int> s;
        heights.insert(heights.begin(), 0);
        heights.push_back(0);
        for(int i = 0; i < heights.size(); i++)
        {
            while(!s.empty() && heights[s.back()] > heights[i])
            {
                int cur = s.back();
                s.pop_back();
                int left = s.back() + 1;
                int right = i - 1;
                res = max(res, (right - left + 1) * heights[cur]);
            }
            s.push_back(i);
        }
        return res;
    }
};


// 删掉冗余的步骤
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int res = 0;

        vector<int> s;
        heights.push_back(0);
        for(int i = 0; i < heights.size(); i++)
        {
            while(!s.empty() && heights[s.back()] > heights[i])
            {
                int cur = s.back();
                s.pop_back();
                int left;
                if(s.empty())
                    left = 0;
                else
                    left = s.back() + 1;
                int right = i - 1;
                res = max(res, (right - left + 1) * heights[cur]);
            }
            s.push_back(i);
        }
        return res;
    }
};
```

<a id="42"></a>

### [42. 接雨水](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    int trap(vector<int>& height) {
        int res = 0;

        vector<int> s;
        for(int i = 0; i < height.size(); i++)
        {
            while(!s.empty() && height[s.back()] < height[i])
            {
                int cur = s.back();
                s.pop_back();

                if(s.empty()) break;

                res = res + (i - s.back() - 1) * (min(height[i], height[s.back()]) - height[cur]);
            }
            s.push_back(i);
        }

        return res;
    }
};
```

<a id="739"></a>

### [739. 每日温度](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& T) {
        vector<int> res(T.size(), 0);

        vector<int> s;
        for(int i = 0; i < T.size(); i++)
        {
            while(!s.empty() && T[s.back()] < T[i])
            {
                res[s.back()] = i - s.back();
                s.pop_back();
            }

            s.push_back(i);
        }

        return res;
    }
};
```

<a id="496"></a>

### [496. 下一个更大元素 I](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        vector<int> s;
        unordered_map<int, int> map;
        for(int i = 0; i < nums2.size(); i++)
        {
            while(!s.empty() && nums2[s.back()] < nums2[i])
            {
                map[nums2[s.back()]] = nums2[i];
                s.pop_back();
            }
            s.push_back(i);
        }
        vector<int> res;
        for(int num1: nums1)
        {
            if(map[num1] == 0)
                res.push_back(-1);
            else
                res.push_back(map[num1]);
        }
        return res;
    }
};
```

<a id="503"></a>

### [503. 下一个更大元素 II](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int sz = nums.size();
        vector<int> res(sz, -1);
        vector<int> s;

        for(int i = 0; i < sz * 2; i++)
        {
            while(!s.empty() && nums[s.back()] < nums[i % sz])
            {
                res[s.back()] = nums[i % sz];
                s.pop_back();
            }
            s.push_back(i % sz);
        }

        return res;
    }
};
```

<a id="402"></a>

### [402. 移掉 K 位数字](#base-solution)

```C++{.line-numbers}
// 栈中有前导0的处理
class Solution {
public:
    string removeKdigits(string num, int k) {
        int sz = num.size();
        if(sz == 0 || sz == k) return "0";

        vector<int> s;
        for(int i = 0; i < sz; i++)
        {
            while((!s.empty()) && (num[s.back()] > num[i]) && (num[s.back()] != 0) && (k != 0))
            {
                s.pop_back();
                k--;
            }
            s.push_back(i);
        }
        // 此时s中可能有前导0的存在
        while(k != 0)
        {
            s.pop_back();
            k--;
        }

        string res;
        bool flag = true;
        for(int i = 0; i < s.size(); i++)
        {
            for(; i < s.size() - 1 && flag; i++)// 处理前导0
            {
                if(num[s[i]] != '0' && flag)
                {
                    flag = false;
                    break;
                }
            }

            res = res + num[s[i]];
        }
        return res;
    }
};

// 栈中没有前导0的处理
class Solution {
public:
    string removeKdigits(string num, int k) {
        int sz = num.size();
        if(sz == 0 || sz == k) return "0";

        vector<int> s;
        for(int i = 0; i < sz; i++)
        {
            while((!s.empty()) && (num[s.back()] > num[i]) && (k != 0))
            {
                s.pop_back();
                k--;
            }

            if(s.empty() && num[i] == '0')// 跳过前导0
                continue;
            s.push_back(i);
        }

        string res;
        while(!s.empty() && k != 0)
        {
            k--;
            s.pop_back();
        }

        for(int index: s)
        {
            res += num[index];
        }
        return res == "" ? "0": res;
    }
};

// 栈中没有前导0的处理，且直接令string为栈
class Solution {
public:
    string removeKdigits(string num, int k) {
        int sz = num.size();
        if(sz == 0 || sz == k) return "0";

        string res;
        for(int i = 0; i < sz; i++)
        {
            while((res.size() != 0) && (res.back() > num[i]) && (k != 0))
            {
                res.pop_back();
                k--;
            }

            if((res.size() == 0) && num[i] == '0')
                continue;
            res += num[i];
        }

        while((res.size() != 0) && k != 0)
        {
            k--;
            res.pop_back();
        }

        return res == "" ? "0": res;
    }
};
```

<a id="316"></a>

### [316. 去除重复字母](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    string removeDuplicateLetters(string s) {
        vector<int> count(26, 0);
        vector<bool> vis(26, false);

        for(char c: s)
            count[c - 'a']++;

        string res;
        for(int i = 0; i < s.size(); i++)
        {
            if(vis[s[i] - 'a'] == false)
            {
                while((res.size() != 0) && (res.back() > s[i]) && (count[res.back() - 'a'] > 1))
                {
                    count[res.back() - 'a']--;
                    vis[res.back() - 'a'] = false;
                    res.pop_back();
                }
                res.push_back(s[i]);
                vis[s[i] - 'a'] = true;
            }
            else
                count[s[i] - 'a']--;
        }

        return res;
    }
};
```

<a id="1673"></a>

### [1673. 找出最具竞争力的子序列](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    vector<int> mostCompetitive(vector<int>& nums, int k) {
        vector<int> res;
        int len = nums.size();
        for(int i = 0; i < len; ++i) {
            while(res.size() && res.back() > nums[i] && res.size() + len - i > k)
                res.pop_back();
            if(res.size() < k) 
                res.push_back(nums[i]);
        }
        return res;
    }
};
```

<a id="901"></a>

### [901. 股票价格跨度](#base-solution)

```C++{.line-numbers}
class StockSpanner {
public:
    StockSpanner() {

    }
    int next(int price) {
        int weight = 1;
        
        while(!tedium.empty() && price >= tedium.top().first){
            weight += tedium.top().second;
            tedium.pop();   
        }

        tedium.push({price,weight});
        return weight;
    }
    stack<pair<int,int>>tedium;
};
```

<a id="581"></a>

### [581. 最短无序连续子数组](#base-solution)

```C++{.line-numbers}
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size();
        int mn = nums[n-1], l = n;
        for (int i = n - 1; i >= 0; --i) {
            if (nums[i] > mn) {
                l = i;
            }
            mn = min(nums[i], mn);
        }
        int mx = nums[0], r = 0;
        for (int i = 0; i < n; ++i) {
            if (nums[i] < mx) {
                r = i;
            }
            mx = max(nums[i], mx);
        }
        return max(r - l + 1, 0);
    }
};
```