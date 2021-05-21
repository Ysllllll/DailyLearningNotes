## 字符串中的 "最长" 总结

<a id="longest"></a>

> **题目汇总**
>
> - **子数组**
>
>   - **[5. 最长回文子串](./E字符处理-回文串.md#palindrome_string)**
>   - **[3. 无重复字符的最长子串](https://leetcode-cn.com/problems/longest-substring-without-repeating-characters/)** [速览题解](#3)
>     - 简评：注意动态规划的分析，dp[i]是什么含义。
>     - 案例："pwwkew"
>   - **[424. 替换后的最长重复字符](https://leetcode-cn.com/problems/longest-repeating-character-replacement/submissions/)** [速览题解](#424)
>   - ++++++++++++++++++++++++
>   - **[674. 最长连续递增序列](https://leetcode-cn.com/problems/longest-continuous-increasing-subsequence/)** [速览题解](#674)
>   - **[845. 数组中的最长山脉](https://leetcode-cn.com/problems/longest-mountain-in-array/)** [速览题解](#845)
>     - 简评：单串、要求连续
>   - **[1438. 绝对差不超过限制的最长连续子数组](https://leetcode-cn.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/)** [速览题解](#1438)
>   - **53. 最大子序和**
>   - **152. 乘积最大子数组**
>   - ++++++++++++++++++++++++
>   - **[718. 最长重复子数组](https://leetcode-cn.com/problems/maximum-length-of-repeated-subarray/submissions/)** [速览题解](#718)
>     - 简评：双串、要求连续
>
> - **子序列**
>
>   - **[5. 最长回文子序列](./E字符处理-回文串.md#palindrome_string)**
>   - ++++++++++++++++++++++++
>   - **[300. 最长递增子序列](https://leetcode-cn.com/problems/longest-increasing-subsequence/submissions/)** [速览题解](#300)
>     - 简评：单串、不要求连续
>   - **[1027. 最长等差数列](https://leetcode-cn.com/problems/longest-arithmetic-subsequence/)** [速览题解](#1027)
>   - **[873. 最长的斐波那契子序列的长度](https://leetcode-cn.com/problems/length-of-longest-fibonacci-subsequence/)** [速览题解](#873)
>   - **[594. 最长和谐子序列](https://leetcode-cn.com/problems/longest-harmonious-subsequence/)** [速览题解](#594)
>     - 简评：是窗口中的元素差值为 1
>   - **No.组合最长山脉**
>   - **No.组合差值小于 limit 的子序列**
>   - ++++++++++++++++++++++++
>   - **[1143. 最长公共子序列](https://leetcode-cn.com/problems/longest-common-subsequence/submissions/)** [速览题解](#1143)
>     - 简评：双串、不要求连续
>
> - **其它**
>
>   - **[32. 最长有效括号](https://leetcode-cn.com/problems/longest-valid-parentheses/)** [速览题解](#32)
>     - 简评：匹配问题，匹配法则是括号成对
>   - **[1392. 最长快乐前缀](https://leetcode-cn.com/problems/longest-happy-prefix/)** [速览题解](#1392)
>     - 简评：匹配问题，匹配法则是前缀后缀相同
>   - **[521. 最长特殊序列 Ⅰ](https://leetcode-cn.com/problems/longest-uncommon-subsequence-i/)** [速览题解](#521)
>     - 简评：这道题考察阅读能力，522 可不做
>   - **[128. 最长连续序列](https://leetcode-cn.com/problems/longest-consecutive-sequence/)** [速览题解](#128)
>     - 简评：先排序，然后再搜索最长连续递增子串，这样做的复杂度就主要取决于排序算法了。但是需要优化到 O(n)的时间复杂度
>   - **[14. 最长公共前缀](https://leetcode-cn.com/problems/longest-common-prefix/)** [速览题解](#14)
>     - 简评：这道题很简单，就是简单的模拟，有多个字符串，看前缀相同的是什么
>   - **[1405. 最长快乐字符串](https://leetcode-cn.com/problems/longest-happy-string/)** [速览题解](#1405)
>     - 简评：这里用到了贪心策略
>   - **[687. 最长同值路径](https://leetcode-cn.com/problems/longest-univalue-path/)** [速览题解](#687)
>   - **[549. 二叉树中最长的连续序列](https://leetcode-cn.com/problems/binary-tree-longest-consecutive-sequence-ii/)** [速览题解](#549)

<a id="3"></a>

### [3. 无重复字符的最长子串](#longest)

```C++{.line-numbers}
//
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int l = 0, r = 0;
        int maxlen = 0;

        for( ; r<s.size(); r++)
        {
            for(int k = l; k < r; k++)
            {
                if(s[r] == s[k])
                {
                    l = k + 1;
                    break;
                }
            }

            if(r - l + 1 > maxlen)
                maxlen = r - l + 1;
        }
        return maxlen;
    }
};

// 该写法直接跳到重复字符的下一个字符处，节省了循环
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> m(128, 0);
        int ans = 0;
        int i = 0;
        for (int j = 0; j < s.size(); j++) {
            i = max(i, m[s[j]]);
            m[s[j]] = j + 1;
            ans = max(ans, j - i + 1);
        }
        return ans;
    }
};

// 动态规划
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if(s.size() == 0) return 0;

        vector<int> dp(s.size(), 0);
        dp[0] = 1;
        for(int i = 1; i < s.size(); i++)
        {
            int j = i - 1;
            for(; (j >= 0) && (s[i] != s[j]); j--);
            dp[i] = min(dp[i - 1] + 1, i - j);
        }
        return *max_element(dp.begin(), dp.end());
    }
};
```

<a id="424"></a>

### [424. 替换后的最长重复字符](#longest)

```C++{.line-numbers}
class Solution {
public:
    int characterReplacement(string s, int k) {
        vector<int> num(26,0);
        int res = 0;
        int left = 0, right = 0;
        while(right < s.size())
        {
            num[s[right] - 'A']++;
            res = max(res, num[s[right] - 'A']);

            if(right - left + 1 - res > k)
            {
                num[s[left] - 'A']--;
                left ++;
            }
            right++;
        }
        return right - left;
    }
};
```

<a id="32"></a>

### [32. 最长有效括号](#longest)

```C++{.line-numbers}
class Solution {
public:
    int longestValidParentheses(string s) {
        if(s.size() <= 1) return 0;

        vector<int> dp(s.size(), 0);
        int res = 0;

        for(int i = 1; i < s.size(); i++)
        {
            if(s[i] == ')')
            {
                if(s[i - 1] == '(')
                {
                    dp[i] = (i >= 2? dp[i - 2]: 0) + 2;
                }
                else if((i - dp[i - 1] > 0) && s[i - 1 - dp[i - 1]] == '(')
                {
                    dp[i] = dp[i - 1] + 2 + (i - 2 - dp[i - 1] >= 0 ? dp[i - 2 - dp[i - 1]]: 0);
                }
            }
            res = max(res, dp[i]);
        }

        return res;
    }
};
```

<a id="521"></a>

### [521. 最长特殊序列 Ⅰ](#longest)

```C++{.line-numbers}
class Solution {
public:
    int findLUSlength(string a, string b) {
        if(a == b) return -1;
        return a.size() > b.size()? a.size(): b.size();
    }
};
```

<a id="128"></a>

### [128. 最长连续序列](#longest)

```C++{.line-numbers}
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if(nums.size() == 0) return 0;
        sort(nums.begin(), nums.end());
        int maxValue = 1;
        for(int i = 0 ; i < nums.size() - 1; i++)
        {
            int tempValue = 1;
            for(int j = i + 1; j < nums.size(); j++)
            {
                if(nums[j] - nums[j - 1] == 1)
                {
                    tempValue++;
                }
                else if(nums[j] == nums[j - 1])
                    continue;
                else
                {
                    i = j -  1; //for循环里有i++，所以需要-1
                    break;
                }
            }
            maxValue = max(maxValue, tempValue);
        }
        return maxValue;
    }
};

// 哈希表
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> nums_set;
        for(int num: nums)
            nums_set.insert(num);

        int res = 0;
        for(int i = 0; i < nums.size(); i++)
        {
            int temp = 0;
            if(nums_set.find(nums[i] - 1) == nums_set.end() )
            {
                int j = 0;
                while(nums_set.find(nums[i] + j) != nums_set.end())
                {
                    temp++;
                    j++;
                }
            }
            res = max(res, temp);
        }
        return res;
    }
};

// 动态规划
class Solution {
public:

    int longestConsecutive(vector<int>& nums) {
        int n = nums.size();
        if(n < 2) return n;
        //先把数组排序
        sort(nums.begin(), nums.end());
        vector<int> dp(n, 1);
        int max_len = 1;
        for(int i = 1; i < n; i++){
            if(nums[i] == nums[i - 1] + 1)
            {
                dp[i] = dp[i - 1] + 1;
            }
            else if(nums[i] == nums[i - 1])
            {
                dp[i] = dp[i - 1];
            }
            max_len = max(max_len, dp[i]);
        }
        return max_len;
    }
};
```

<a id="1392"></a>

### [1392. 最长快乐前缀](#longest)

```C++{.line-numbers}
// dp[i]含义是：以 i 结尾的最长前缀匹配个数
class Solution {
public:
    string longestPrefix(string s) {
        vector<int> dp(s.size(), 0);
        for(int i = 1; i < s.size(); i++)
        {
            int k = dp[i - 1]; // k偏移一位，刚好是下一个待匹配的字符
            while(k != 0 && s[i] != s[k]) k = dp[k - 1];
            dp[i] = s[i] == s[k]? k + 1: 0;
        }

        return s.substr(0, dp[s.size() - 1]);
    }
};

// 另外一种写法，使用纯数组比使用 vector 快一点
class Solution {
public:
    string longestPrefix(string s) {
        if(s.length() <= 1) return "";

        int n = s.length();
        int next[n + 1];
        next[0] = -1;
        int k = -1;
        int j = 0;
        while(j <= n-1){
            if(k == -1 || s[k] == s[j])
            {
                ++j;
                ++k;
                next[j] = k;
            }else
            {
                k = next[k];
            }
        }
        return s.substr(0,next[n]);
    }
};
```

<a id="14"></a>

### [14. 最长公共前缀](#longest)

```C++{.line-numbers}
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(strs.size() == 0) return "";

        int i = 0;
        bool flag = true;
        for(; flag == true; i++)
        {
            if(i > strs[0].size()) flag = false;

            for(int j = 1; j < strs.size() && flag == true; j++)
            {
                if(i > strs[j].size()) flag = false;
                if(strs[j][i] != strs[j - 1][i]) flag = false;
            }
        }

        return strs[0].substr(0, i - 1);

    }
};
```

<a id="594"></a>

### [594. 最长和谐子序列](#longest)

```C++{.line-numbers}
class Solution {
public:
    int findLHS(vector<int>& nums) {
        sort(nums.begin(), nums.end());

        int res = 0, begin = 0;
        for(int end = 0; end < nums.size(); end++)
        {
            while(nums[end] - nums[begin] > 1)
                begin++;
            if(nums[end] - nums[begin] == 1)
                res = max(res, end - begin + 1);
        }

        return res;
    }
};
```

<a id="674"></a>

### [674. 最长连续递增序列](#longest)

```C++{.line-numbers}
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        if(nums.size() <= 1) return nums.size();

        vector<int> dp(nums.size(), 1);
        int res = 1;
        for(int i = 1; i < nums.size(); i++)
        {
            if(nums[i] > nums[i - 1])
                dp[i] = dp[i - 1] + 1;
            res = max(res, dp[i]);
        }

        return res;
    }
};
```

<a id="300"></a>

### [300. 最长递增子序列](#longest)

```C++{.line-numbers}
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if(nums.size() <= 1) return nums.size();

        vector<int> dp(nums.size(), 1);
        int res = 1;
        for(int i = 0; i < nums.size(); i++)
        {
            for(int j = 0; j < i; j++)
            {
                if(nums[i] > nums[j])
                    dp[i] = max(dp[i], dp[j] + 1);
            }
            res = max(res, dp[i]);
        }

        return res;
    }
};
```

<a id="1143"></a>

### [1143. 最长公共子序列](#longest)

```C++{.line-numbers}
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        if((text1.size() == 0) || (text2.size() == 0)) return 0;

        vector<vector<int>> dp(text1.size() + 1, vector<int>(text2.size() + 1, 0));

        for(int i = 1; i <= text1.size(); i++)
        {
            for(int j = 1; j <= text2.size(); j++)
            {
                if(text1[i - 1] == text2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                else
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }

        return dp[text1.size()][text2.size()];
    }
};
```

<a id="718"></a>

### [718. 最长重复子数组](#longest)

```C++{.line-numbers}
class Solution {
public:
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        if(nums1.size() == 0 || nums2.size() == 0) return 0;

        vector<vector<int>> dp(nums1.size() + 1, vector<int>(nums2.size() + 1, 0));
        int res = 0;
        for(int i = 1; i <= nums1.size(); i++)
        {
            for(int j = 1; j <= nums2.size(); j++)
            {
                if(nums1[i - 1] == nums2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                res = max(res, dp[i][j]);
            }
        }

        return res;
    }
};
```

<a id="845"></a>

### [845. 数组中的最长山脉](#longest)

```C++{.line-numbers}
// 第一种方法：分别求出每个点左右两边的连续递增和递减数组，然后再遍历一次数组即可
class Solution {
public:
    int longestMountain(vector<int>& arr) {
        if(arr.size() <= 1) return 0;
        int res = 0;

        vector<int> left(arr.size(), 0); // 连续递增子数组
        vector<int> right(arr.size(), 0); // 连续递减子数组
        for(int i = 1; i < arr.size(); i++)
        {
            if(arr[i - 1] < arr[i])
                left[i] = left[i - 1] + 1;
        }
        for(int j = arr.size() - 1; j > 0; j--)
        {
            if(arr[j - 1] > arr[j])
                right[j - 1] = right[j] + 1;
        }

        for(int i = 1; i < arr.size() - 1; i++)
        {
            if(left[i] != 0 && right[i] != 0 && (left[i] + right[i] + 1) >= 3)
                res = max(res, (left[i] + right[i] + 1));
        }

        return res;
    }
};

// 第二种方法：空间复杂度更优
class Solution
{
public:
    int longestMountain(vector<int> &A)
    {
        int maxLength = 0;
        int i = 1;

        while (i < A.size())
        {
            int increasing = 0, decreasing = 0;

            while (i < A.size() && A[i - 1] < A[i])
                i++, increasing++;
            while (i < A.size() && A[i - 1] > A[i])
                i++, decreasing++;

            if (increasing > 0 && decreasing > 0)
                maxLength = max(maxLength, increasing + decreasing + 1);

            while (i < A.size() && A[i - 1] == A[i])
                i++;
        }
        return maxLength;
    }
};
```

<a id="687"></a>

### [687. 最长同值路径](#longest)

```C++{.line-numbers}
class Solution {
public:
    int longestUnivaluePathHelper(TreeNode* root, int &res) {
        if(root == nullptr) return 0;
        int temp = 0;

        int left = longestUnivaluePathHelper(root->left, res);
        int right = longestUnivaluePathHelper(root->right, res);
        if((root->left != nullptr) && (root->left->val == root->val) && (root->right != nullptr) && (root->right->val == root->val))
            res = max(res, left + right + 2);

        if((root->left != nullptr) && (root->left->val == root->val))
            temp = left + 1;

        if((root->right != nullptr) && (root->right->val == root->val))
            temp = max(temp, right + 1);
        res = max(res, temp);
        return temp;
    }
    int longestUnivaluePath(TreeNode* root) {
        int res = 0;
        longestUnivaluePathHelper(root, res);
        return res;
    }
};
```

<a id="549"></a>

### [549. 二叉树中最长的连续序列](#longest)

```C++{.line-numbers}

class Solution {
public:
    /**
     * @param root: the root of binary tree
     * @return: the length of the longest consecutive sequence path
     */
    int longestConsecutive(TreeNode * root) {
        // write your code here
        return lengthcal(root,NULL,0);
    }

    int lengthcal(TreeNode * root,TreeNode * parent,int lengthwitgoutroot){
        if(root==NULL) return 0;
        int length=(parent!=NULL&&parent->val+1==root->val)?lengthwitgoutroot+1:1;
        int left=lengthcal(root->left,root,length);
        int right=lengthcal(root->right,root,length);
        return max(length,max(left,right));
    }
```

<a id="1405"></a>

### [1405. 最长快乐字符串](#longest)

```C++{.line-numbers}
class Solution {
public:
    string longestDiverseString(int a, int b, int c) {

        vector<pair<int, char>> v;
        v.push_back(make_pair(a, 'a'));
        v.push_back(make_pair(b, 'b'));
        v.push_back(make_pair(c, 'c'));
        string ans;
        while (v[0].first || v[1].first || v[2].first) {
            sort(v.rbegin(), v.rend());
            if (ans.empty() || ans.back() != v[0].second) {
                if (v[0].first-- > 0) ans = ans + v[0].second;
                if (v[0].first-- > 0) ans = ans + v[0].second;
            }
            else {
                if (v[1].first-- > 0) ans = ans + v[1].second;
                else break;
            }
        }
        return ans;
    }
};

```

<a id="1438"></a>

### [1438. 绝对差不超过限制的最长连续子数组](#longest)

```C++{.line-numbers}
class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        deque<int> queMax, queMin;
        int n = nums.size();
        int left = 0, right = 0;
        int ret = 0;
        while (right < n) {
            while (!queMax.empty() && queMax.back() < nums[right]) {
                queMax.pop_back();
            }
            while (!queMin.empty() && queMin.back() > nums[right]) {
                queMin.pop_back();
            }
            queMax.push_back(nums[right]);
            queMin.push_back(nums[right]);
            while (!queMax.empty() && !queMin.empty() && queMax.front() - queMin.front() > limit) {
                if (nums[left] == queMin.front()) {
                    queMin.pop_front();
                }
                if (nums[left] == queMax.front()) {
                    queMax.pop_front();
                }
                left++;
            }
            ret = max(ret, right - left + 1);
            right++;
        }
        return ret;
    }
};

class Solution {
public:

    //解法二：使用双指针构造滑动窗口。使用单调队列计算窗口内任意两元素的绝对差的最大值，若该最大值大于 limit 则将窗口左端右移一位，否则将窗口右端右移一位；满足要求的滑动窗口的最大长度即为所求。
    int longestSubarray(vector<int>& nums, int limit)
    {
        deque<int> maxQueue; //最大值队列。存储可能成为滑动窗最大值的元素值。单调减
        deque<int> minQueue; //最小值队列。存储可能成为滑动窗最小值的元素值。单调增
        int n = nums.size();
        int windowRight = 0;
        int windowLeft = 0;
        int maxLength = 0;
        int currentLength;

        //以 [windowLeft,windowRight] 为滑动窗口
        while (windowRight < n)
        {
            while (!maxQueue.empty() && nums[windowRight] > maxQueue.back())
                maxQueue.pop_back(); //这些元素值已不可能成为滑动窗口的最大值
            while (!minQueue.empty() && nums[windowRight] < minQueue.back())
                minQueue.pop_back(); //这些元素值已不可能成为滑动窗口的最小值

            maxQueue.push_back(nums[windowRight]);
            minQueue.push_back(nums[windowRight]);

            //找到以 windowRight 为右端点的满足要求的最长子数组
            while (maxQueue.front()- minQueue.front() > limit) //不断移出子数组左端的元素
            {
                if (nums[windowLeft] == maxQueue.front()) //移出的是原窗口的最大值
                    maxQueue.pop_front();
                if (nums[windowLeft] == minQueue.front()) //移出的是原窗口的最小值
                    minQueue.pop_front();
                windowLeft++;
            }
            currentLength = windowRight - windowLeft + 1;
            maxLength = maxLength > currentLength ? maxLength : currentLength;
            windowRight++;
        }
        return maxLength;
    }
};
```
