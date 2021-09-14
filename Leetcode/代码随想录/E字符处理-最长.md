## 字符串中的 "最长" 总结

<a id="longest"></a>

> **题目汇总**
>
> - **子数组**
>
>   - **[5. 最长回文子串](https://leetcode-cn.com/problems/longest-palindromic-substring/)**
>     - 简评：回文板块
>   - **[3. 无重复字符的最长子串](https://leetcode-cn.com/problems/longest-substring-without-repeating-characters/)**
>     - 简评：滑动窗口
>   - **[424. 替换后的最长重复字符](https://leetcode-cn.com/problems/longest-repeating-character-replacement/)**
>     - 简评：滑动窗口
>   - **[674. 最长连续递增序列](https://leetcode-cn.com/problems/longest-continuous-increasing-subsequence/)**
>     - 简评：动态规划、滑动窗口-循环不变量
>   - **[845. 数组中的最长山脉](https://leetcode-cn.com/problems/longest-mountain-in-array/)**
>     - 简评：双指针
>   - **[1438. 绝对差不超过限制的最长连续子数组](https://leetcode-cn.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/)**
>     - 简评：滑动窗口
>   - **[53. 最大子序和](https://leetcode-cn.com/problems/maximum-subarray/)**
>     - 简评：动态规划
>   - **[152. 乘积最大子数组](https://leetcode-cn.com/problems/maximum-product-subarray/)**
>     - 简评：动态规划
>   - ++++++++++++++++++++++++
>   - **[718. 最长重复子数组](https://leetcode-cn.com/problems/maximum-length-of-repeated-subarray/)**
>     - 简评：动态规划
>
> - **子序列**
>
>   - **[516. 最长回文子序列](https://leetcode-cn.com/problems/longest-palindromic-subsequence/)**
>     - 简评：回文串板块
>   - ++++++++++++++++++++++++
>   - **[300. 最长递增子序列](https://leetcode-cn.com/problems/longest-increasing-subsequence/)**
>     - 简评：动态规划
>   - **[1027. 最长等差数列！！](https://leetcode-cn.com/problems/longest-arithmetic-subsequence/)**
>   - **[873. 最长的斐波那契子序列的长度！！](https://leetcode-cn.com/problems/length-of-longest-fibonacci-subsequence/)**
>   - **[594. 最长和谐子序列](https://leetcode-cn.com/problems/longest-harmonious-subsequence/)**
>     - 简评：是窗口中的元素差值为 1
>   - ++++++++++++++++++++++++
>   - **[1143. 最长公共子序列](https://leetcode-cn.com/problems/longest-common-subsequence/)** [速览题解](#1143)
>     - 简评：动态规划
>
> - **其它**
>
>   - **[32. 最长有效括号](https://leetcode-cn.com/problems/longest-valid-parentheses/)** [速览题解](#32)<a id="l32"></a>
>     - 简评：**动态规划**。这道题要读懂题意，说的是**最长的连续的**有效括号，不是说的有效的括号有多少。即注意这种情况：`")()))((()))"` 结果为6，而非8
>   - **[1392. 最长快乐前缀](https://leetcode-cn.com/problems/longest-happy-prefix/)** [速览题解](#1392)
>     - 简评：**KMP算法**，很简单，[KMP 的动态规划解释参考这个链接](https://leetcode-cn.com/problems/longest-happy-prefix/solution/c-dp-5367-zui-chang-kuai-le-qian-zhui-by-yuexiwen/)。另外一方面也可以使用字符串哈希做法
>   - **[521. 最长特殊序列 Ⅰ](https://leetcode-cn.com/problems/longest-uncommon-subsequence-i/)** [速览题解](#521)
>     - 简评：这道题考察阅读能力，522 可不做
>   - **[128. 最长连续序列](https://leetcode-cn.com/problems/longest-consecutive-sequence/)** [速览题解](#128)
>     - 简评：**哈希法**。这道题算是**无序的最长连续序列**，之前的经典题型是**有序的最长连续序列**。先排序，然后再搜索最长连续递增子串，这样做的复杂度就主要取决于排序算法了。但是需要优化到 O(n)的时间复杂度。可以使用哈希表，然后随机选取哈希表中的一个数，**中心扩散**的寻找周围的数是否在哈希表中。时间复杂度和空间复杂度均为O(N)。
>   - **[14. 最长公共前缀](https://leetcode-cn.com/problems/longest-common-prefix/)** [速览题解](#14)
>     - 简评：**模拟**。这道题比较简单，可以横向扫描也可以纵向扫描，寻找 `vector<string>` 中所有字符串的公共前缀，注意 `flag`。
>   - **[1405. 最长快乐字符串](https://leetcode-cn.com/problems/longest-happy-string/)** [速览题解](#1405)
>     - 简评：**贪心策略**。给定一定数量的字符串，在**满足排放规则**的前提下构成最长的字符串。使用贪心算法，先**在最多的字符里面选取两个**，然后**在次多的字符里选取一个**。
>   - **[687. 最长同值路径](https://leetcode-cn.com/problems/longest-univalue-path/)** [速览题解](#687)
>     - 简评：**二叉树的后序遍历**。这道题注意分析思路，递归实现分别计算 `left, right, lm, lmr, rm`。**先计算出 `left` 和 `right`，然后再分别计算三条路径**。其中：`lmr` 只参与判断可以组成的最长长度，不用于返回，只有 `lm` 和 `rm` 用于返回。递归的过程是后序遍历，所以也可以用迭代来实现。
>   - **[549. 二叉树中最长的连续序列](https://leetcode-cn.com/problems/binary-tree-longest-consecutive-sequence-ii/)** [速览题解](#549)
>     - 简评：**树上的动态规划**，返回的数组为 `[inc, dec]`

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
                    // "()"  "()()"
                    dp[i] = (i >= 2? dp[i - 2]: 0) + 2;
                }
                // "(()))))()(()())"
                //        ^ 之前的不管，求的是连续最长的
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
        unordered_set<int> q;
        for (int i = 0; i < nums.size(); i++) {
            q.insert(nums[i]);
        }
        int ans = 0;
        while (!q.empty()) {
            int now = *q.begin();
            q.erase(now);
            int l = now - 1, r = now + 1;
            while (q.find(l) != q.end()) {
                q.erase(l);
                l--;
            }
            while(q.find(r) != q.end()) {
                q.erase(r);
                r++;
            }
            l = l + 1, r = r - 1;
            ans = max(ans, r - l + 1);
        }
        return ans;
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
// 使用 KMP 做法
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

        vector<int> next(s.size() + 1);
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
        return s.substr(0,next[s.size()]);
    }
};

// 使用字符哈希的做法
class Solution {
public:
    string longestPrefix(string s) {
        long long prefix = 0, suffix = 0;
        long long base = 131, mod = 1000000007, mul = 1;
        int happy = 0;
        for(int i = 1; i < s.size(); i++)
        {
            prefix = (prefix * base + s[i - 1]) % mod;
            suffix = (suffix + s[s.size() - i] * mul) % mod;
            if(prefix == suffix)
                happy = i;
            
            mul = mul *base % mod;
        }
        return s.substr(0, happy);
    }

    string longestPrefix1(string s)
    {
        // "123456" -> 123456
        int base = 10, ret1 = 0;
        for(int i = 0; i < s.size(); i++)
        {
            ret1 = ret1 * base + s[i] - '0';
        }

        // "123456" -> 123456
        int mul = 1, ret2 = 0;
        for(int i = s.size() - 1; i >= 0; i--)
        {
            ret2 = ret2 + (s[i] - '0') * mul;
            mul = mul * base;
        }

        return "";
    }
};
```

<a id="14"></a>

### [14. 最长公共前缀](#longest)

```C++{.line-numbers}
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {

        int i = 0;
        int flag = true;
        while(flag == true)
        {
            // 注意这里 && flag
            for(int j = 0; j < strs.size() && flag; j++)
            {
                if(i > strs[j].size()) flag = false;
                if(j > 0 && strs[j][i] != strs[j - 1][i]) flag = false;
            }
            i++;
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
    int longestUnivaluePath(TreeNode* root, int &res) {
        if(root == nullptr) return 0;

        int left = longestUnivaluePath(root->left, res); // 这里注意函数调用
        int right = longestUnivaluePath(root->right, res);

        int lm = 0, rm = 0, lmr = 0;
        if(root->left != nullptr && root->left->val == root->val && root->right != nullptr && root->right->val == root->val)
            lmr = left + right + 2;
        if(root->left != nullptr && root->left->val == root->val)
            lm = left + 1;
        if(root->right != nullptr && root->right->val == root->val)
            rm = right + 1;
        
        res = max({res, lmr, lm, rm}); // 这里注意 res 也要参与运算
        return max({lm, rm});
    }
    int longestUnivaluePath(TreeNode* root) {
        int res = 0;
        longestUnivaluePath(root, res);
        return res;
    }
};

class Solution {
public:
    int longestUnivaluePath(TreeNode* root, int &res) {
        if(root == nullptr) return 0;

        int left = longestUnivaluePath(root->left, res); // 这里注意函数调用
        int right = longestUnivaluePath(root->right, res);
        
        int lm = (root->left != nullptr && root->left->val == root->val)? left + 1: 0;
        int mr = (root->right != nullptr && root->right->val == root->val)? right + 1: 0;
        
        res = max(res, lm + mr);
        return max({lm, mr});
    }
    int longestUnivaluePath(TreeNode* root) {
        int res = 0;
        longestUnivaluePath(root, res);
        return res;
    }
};
```

<a id="549"></a>

### [549. 二叉树中最长的连续序列](#longest)

```C++{.line-numbers}
class Solution {
public:
    int longestConsecutive(TreeNode *root) {
        int res = 0;
        traversal(root, res);
        return res;
    }
    
    pair<int, int> traversal(TreeNode *root, int &res) {
        if (root == nullptr) return {0, 0};
        
        auto left = traversal(root->left, res);
        auto right = traversal(root->right, res);
        
        int inc = 1, dec = 1;
        if (root->left != nullptr) {
            if (root->left->val + 1 == root->val) inc = left.first + 1;
            if (root->left->val - 1 == root->val) dec = left.second + 1;
        }
        if (root->right != nullptr) {
            // 注意定义的递增和递减的方向与left不一样
            if (root->val - 1 == root->right->val) inc = max(inc, right.first + 1);
            if (root->val + 1 == root->right->val) dec = max(dec, right.second + 1);
        }
        // left[1, 2, 3] root[4] right[5, 6, 7]
        //     inc                    dec
        res = max(res, inc + dec - 1);
        
        return {inc, dec};
    }
};
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
