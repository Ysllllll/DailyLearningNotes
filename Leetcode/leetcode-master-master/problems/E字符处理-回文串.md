## 回文串总结

- 类型一：数字
- 类型二：数组 或 字符串
  - 数组中的元素 ==**不能修改**==
    - 数组中的元素能 ==**重排列**==：**是否能够得到一个回文串**，及能够得到的**最长的回文串**
    - 数组中的元素 ==**不能重排列**==：**本身是否是回文串**，有**多少个回文子串**、**最长的回文子串长度**、**最长的回文子序列**
    - 数组中的元素 ==**不能重排列**==：字符串能否**分割一次、两次、三次...** 使得子串都是回文串，**最少要分割几次**
  - 数组中的元素 ==**能够增加、删除、修改**==
    - 在串的前面或后面**添加**多少个元素能够得到一个回文串
    - **删除**串中的一个、两个、三个...元素能否得到一个回文串
    - **修改**串中的一个、两个、三个...元素能否得到一个回文串
- 类型三：链表

<a id="palindrome_string"></a>

> **题目汇总**
>
> - **[9. 回文数](https://leetcode-cn.com/problems/palindrome-number)** [速览题解](#9)
> - **[234. 回文链表](https://leetcode-cn.com/problems/palindrome-linked-list/)** [速览题解](#234)
>   - 简评：这里我们是反转的后半部分的链表，但是可以反转前半部分的链表，这样可以少遍历一趟
> - **[0104. 回文排列](https://leetcode-cn.com/problems/palindrome-permutation-lcci)** [速览题解](#0104)
> - **[409. 最长回文串](https://leetcode-cn.com/problems/longest-palindrome)** [速览题解](#409)
>   - 案例：[a a a]
>   - 简评：这道题和 0104 题一样，不过这题是找字符串中的回文串，而上面是判断字符串本身是不是回文串
> - **[125. 验证回文串](https://leetcode-cn.com/problems/valid-palindrome)** [速览题解](#125)
>
> ---
>
> - **[647. 回文子串](https://leetcode-cn.com/problems/palindromic-substrings/)** [速览题解](#647)
>   - 简评：中心扩展法、动态规划
> - **[5. 最长回文子串](https://leetcode-cn.com/problems/longest-palindromic-substring/)** [速览题解](#5)
>   - 简评：最长回文子串是在回文子串的基础之上，在判断 dp[i][j]的过程中就记录下最大子串，所以不仅能够得到**最长子串的长度**，也能得到这个**最长子串是什么**。另外如同回文子串一样，可以用中心扩展法，空间复杂度优于动态规划
> - **[516. 最长回文子序列](https://leetcode-cn.com/problems/longest-palindromic-subsequence)** [速览题解](#516)
>   - 简评：s 与 s.reverse() 的最长公共子序列即为其最长回文子序列。这道题的升级版是打印出最长回文子序列。目前只能得到**最长回文子序列有多长**，还不能**求出它是哪一个**
>
> ---
>
> - **[214. 最短回文串](https://leetcode-cn.com/problems/shortest-palindrome)** [速览题解](#214)
>   - 案例："abbacba"
>   - 简评：第一反应是中心扩展法，还有 KMP 算法，马拉车算法
>   - 简评：添加最少字符，使得字符串成为一个回文串，和 680 是对偶的
> - **[680. 验证回文字符串 II](https://leetcode-cn.com/problems/valid-palindrome-ii)** [速览题解](#680)
>   - 简评：删除字符串中的字符，使得剩下的字符串是一个回文串。
>   - 扩展：最多删除多少个字符可以组成一个回文串，这样就对偶 516、最长回文子序列，不管要删除多少个，就是求出最长回文子序列，但是可能会出现超时。另外也可以看下[dp 考虑删 k 个 do more, do better](https://leetcode-cn.com/problems/valid-palindrome-ii/solution/c-kao-lu-shan-kge-do-more-do-better-by-wen-mu-yang/)
>
> ---
>
> - **[131. 分割回文串](https://leetcode-cn.com/problems/palindrome-partitioning)** [速览题解](#131)
>   - 简评：求所有的分割方案
> - **[132. 分割回文串 II](https://leetcode-cn.com/problems/palindrome-partitioning-ii)** [速览题解](#132)
>   - 简评：求最小分割次数
> - **[1278. 分割回文串 III](https://leetcode-cn.com/problems/palindrome-partitioning-iii)** [速览题解](#1278)
>   - 简评：求分割 k 次，最小需要改变多少个字符
> - **[1745. 分割回文串 IV](https://leetcode-cn.com/problems/palindrome-partitioning-iv)** [速览题解](#1745)
>   - 简评：求能否分割为 3 个子串，拓展：求能否分割成 k 个子串
>
> ---

<a id="9"></a>

### [9. 回文数](#palindrome_string)

```C++{.line-numbers}
class Solution {
public:
    bool isPalindrome(int x) {
        if(x < 0) return false;

        int temp = x;
        int res = 0;
        while(temp > 0)
        {
            if(temp > 0 && res > INT_MAX / 10)
                return false;
            res = res * 10 + temp % 10;
            temp = temp / 10;
        }
        return x == res;
    }
};
```

<a id="647"></a>

### [647. 回文子串](#palindrome_string)

```C++{.line-numbers}
// 动态规划：稍微不好理解
class Solution {
public:
    int countSubstrings(string s) {
        int res = 0;
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), true));

        for(int i = s.size() - 1; i >= 0; i--)
        {
            for(int j = i + 1; j < s.size(); j++)
            {
                if(dp[i + 1][j - 1] == false || s[i] != s[j])
                    dp[i][j] = false;
                else
                    res++;
            }
        }

        return res + s.size();
    }
};

// 动态规划：比较好理解的写法
class Solution {
public:
    int countSubstrings(string s) {
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), false));
        int res = 0;
        for (int i = s.size() - 1; i >= 0; i--) {
            for (int j = i; j < s.size(); j++) {
                if (s[i] == s[j] && (j - i <= 1 || dp[i + 1][j - 1])) {
                    res++;
                    dp[i][j] = true;
                }
            }
        }
        return res;
    }
};

// 中心扩展法
class Solution {
public:
    int countSubstrings(string s) {
        int res = 0;
        for(int i = 0; i < s.size(); i++)
        {
            res += countSubstrings(s, i, i);
            res += countSubstrings(s, i, i + 1);
        }

        return res;
    }
    int countSubstrings(string &s, int left, int right)
    {
        int res = 0;
        while((left >= 0) && (right <= (s.size() - 1)) && (s[left] == s[right]))
        {
            left--;
            right++;
            res++;
        }
        return res;
    }
};
```

<a id="234"></a>

### [234. 回文链表](#palindrome_string)

```C++{.line-numbers}
// 第一种，先装进数组中，再判断。时间空间复杂度都是O(n)
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        vector<int> vec;
        while(head!= nullptr)
        {
            vec.push_back(head->val);
            head = head->next;
        }
        for(int i = 0, j = vec.size() - 1; i <= j; i++, j--)
        {
            if(vec[i] != vec[j])
                return false;
        }

        return true;
    }
};

// 空间复杂度为O(1)，时间复杂度为O(n)，同时能够复原链表
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if(head == nullptr || head->next == nullptr) return true;

        // 不区分奇偶，slow指针走到中间，此时以slow为分界点
        // headATail是为了辅助还原链表
        ListNode *slow = head, *fast = head, *headATail = head;
        while(fast != nullptr && fast->next != nullptr)
        {
            headATail = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        headATail->next = nullptr;

        // 以slow为头节点，反转链表得到headB
        ListNode *headB = nullptr;
        while(slow != nullptr)
        {
            ListNode *next = slow->next;
            slow->next = headB;
            headB = slow;
            slow = next;
        }
        // 得到headA
        ListNode *headA = head;

        // 对比headA和headB两个链表判断是否为回文串
        // 同时复原链表
        bool res = true;
        ListNode *headBNext = nullptr;
        while(headA != nullptr)
        {
            if(res && (headA->val != headB->val))
                res = false;
            headA = headA->next;

            ListNode *next = headB->next;
            headB->next = headBNext;
            headBNext = headB;
            headB = next;
        }

        if(headB != nullptr)
        {
            headB->next = headBNext;
            headBNext = headB;
        }
        headATail->next = headBNext;


        return res;
    }
};

// 递归
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        ListNode* headA = head;
        ListNode* headB = head;
        return isPalindromeHelp(headA, headB);
    }
    bool isPalindromeHelp(ListNode* &headA, ListNode* headB)
    {
        if(headB == nullptr) return true;

        if(isPalindromeHelp(headA, headB->next) && (headA->val == headB->val))
        {
            headA = headA->next;
            return true;
        }
        else
            return false;
    }
};
```

<a id="0104"></a>

### [0104. 回文排列](#palindrome_string)

```C++{.line-numbers}
// 使用map
class Solution {
public:
    bool canPermutePalindrome(string s) {
        map<char, int> count;
        for(char c:s)
            count[c]++;

        int res = 0;
        for(pair<char, int> i : count)
            res = res + i.second % 2;

        return res <= 1;
    }
};

// 使用数组
class Solution {
public:
    bool canPermutePalindrome(string s) {
        vector<int> count(256, 0);
        for(char c:s)
            count[c]++;

        int res = 0;
        for(int i : count)
            res = res + i % 2;

        return res <= 1;
    }
};

// 使用bitset
class Solution {
public:
    bool canPermutePalindrome(const string &s) {
        bitset<128> flags;
        for(auto ch : s){
            flags.flip(ch);
        }
        return flags.count() < 2; //出现奇数次的字符少于2个
    }
};
```

<a id="409"></a>

### [409. 最长回文串](#palindrome_string)

```C++{.line-numbers}
class Solution {
public:
    int longestPalindrome(string s) {
        vector<int> count(256, 0);

        for(char c: s)
            count[c]++;

        int res = 0;

        for(int i: count)
            res = res + i / 2;// 只取偶数个

        return res * 2 + (s.size() > res * 2? 1:0);
    }
};
```

<a id="5"></a>

### [5. 最长回文子串](#palindrome_string)

```C++{.line-numbers}
// 中心扩展法
class Solution {
public:
    string longestPalindrome(string s) {
        vector<int> boundary(2,0);

        string res;

        for(int i = 0; i < s.size(); i++)
        {
            updateBoundary(boundary, s, i, i);
            updateBoundary(boundary, s, i, i + 1);
        }
        return s.substr(boundary[0], boundary[1] - boundary[0] + 1);
    }
    void updateBoundary(vector<int> &boundary,string &s, int left, int right)
    {
        while((left >= 0) && (right <= s.size()) && (s[left] == s[right]))
        {
            left--;
            right++;
        }
        if((right - left - 2) > (boundary[1] - boundary[0]))
        {
            boundary[0] = left + 1;
            boundary[1] = right - 1;
        }
    }
};

// 动态规划
class Solution {
public:
    string longestPalindrome(string s) {
        vector<int> boundary(2,0);
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), true));

        for(int i = s.size() - 1; i >= 0; i--)
        {
            for(int j = i + 1; j < s.size(); j++)
            {
                if(s[i] != s[j] || dp[i + 1][j - 1] == false)
                {
                    dp[i][j] = false;
                }
                else if(j - i > boundary[1])
                {
                    boundary[1] = j - i;
                    boundary[0] = i;
                }
            }
        }

        return s.substr(boundary[0], boundary[1] + 1);
    }

};
```

<a id="516"></a>

### [516. 最长回文子序列](#palindrome_string)

```C++{.line-numbers}
// 动态规划：二维数组
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        if(s.size() == 0) return 0;

        vector<vector<int>> dp(s.size(), vector<int>(s.size(), 0));
        for(int i = 0; i < s.size(); i++) dp[i][i] = 1;

        for(int i = s.size() - 1; i >= 0; i--)
        {
            for(int j = i + 1; j < s.size(); j++)
            {
                if(s[i] == s[j])
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                else
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);

            }
        }
        return dp[0][s.size() - 1];
    }
};
// 动态规划：一维数组
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        if(s.size() == 0) return 0;

        vector<int> dp(s.size(), 1);

        for(int i = s.size() - 1; i >= 0; i--)
        {
            int pre = 0;
            for(int j = i + 1; j < s.size(); j++)
            {
                int temp = dp[j];
                if(s[i] == s[j])
                    dp[j] = pre + 2;
                else
                    dp[j] = max(dp[j], dp[j - 1]);
                pre = temp;
            }
        }
        return dp[s.size() - 1];
    }
};
```

<a id="214"></a>

### [214. 最短回文串](#palindrome_string)

```C++{.line-numbers}
class Solution {
public:
    string shortestPalindrome(string s) {
        int left = 0, right = s.size() - 1;
        while(right >= 0)
        {
            if(s[left] == s[right])
                left++;
            right--;
        }

        if(left == s.size()) return s;
        string suffix = s.substr(left);
        string reverseSuffix;
        for(int i = s.size() - 1; i >= left; i--)
        {
            reverseSuffix = reverseSuffix + s[i];
        }
        cout<<"reverseSuffix: "<<reverseSuffix<<endl;
        return reverseSuffix + shortestPalindrome(s.substr(0,left)) + suffix;
    }
};
```

<a id="125"></a>

### [125. 验证回文串](#palindrome_string)

```C++{.line-numbers}
class Solution {
public:
    bool isPalindrome(string s) {
        int i = 0 ;
        int j = s.size()-1;
        while(i < j){
            while(i < j && (!isdigit(s[i]) && !isalpha(s[i]))){
                ++i;
            }
            while(i < j && (!isdigit(s[j]) && !isalpha(s[j]))){
                --j;
            }
            if(tolower(s[i]) != tolower(s[j])){
                return false;
            }
            ++i;
            --j;
        }
        return true;
    }
};

class Solution {
public:
    bool isPalindrome(string s) {
        int i = 0 ;
        int j = s.size()-1;
        while(i < j){
            while(i < j && (!isalnum(s[i]))){//学习下小技巧
                ++i;
            }
            while(i < j && (!isalnum(s[j]))){
                --j;
            }
            if(tolower(s[i]) != tolower(s[j])){
                return false;
            }
            ++i;
            --j;
        }
        return true;
    }
};
```

<a id="680"></a>

### [680. 验证回文字符串 II](#palindrome_string)

```C++{.line-numbers}
class Solution {
public:
    bool validPalindrome(string s) {
        int left = 0;
        int right = s.size() - 1;

        for(; (left < right) && (s[left] == s[right]); left++, right--);
        if(left >= right) return true;

        // 注意这种写法只适合删除一个字符的情况，因为情况就只有两种。
        // 删除left指向的字符
        int left1 = left + 1;
        int right1 = right;
        for(; (left1 < right1) && (s[left1] == s[right1]); left1++, right1--);
        if(left1 >= right1) return true;

        // 删除right指向的字符
        left1 = left;
        right1 = right - 1;
        for(; (left1 < right1) && (s[left1] == s[right1]); left1++, right1--);
        if(left1 >= right1) return true;

        return false;
    }
};
```

<a id="131"></a>

### [131. 分割回文串](#palindrome_string)

```C++{.line-numbers}
// 普通回溯
class Solution {
public:
    bool isPalindrome(string &s, int left, int right)
    {
        for(; left < right && s[left] == s[right]; left++, right--);
        return left >= right;
    }
    void backTracking(string &s, vector<vector<string>> &res, vector<string> & path, int startIndex)
    {
        if(startIndex >= s.size())
        {
            res.push_back(path);
            return;
        }

        for(int i = startIndex; i < s.size(); i++)
        {
            if(isPalindrome(s, startIndex, i))
            {
                string subStr = s.substr(startIndex, i - startIndex + 1);
                path.push_back(subStr);
                backTracking(s, res, path, i + 1);
                path.pop_back();
            }
        }
    }
    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> path;
        backTracking(s, res, path, 0);
        return res;
    }
};

// 回溯 + 动态规划预处理
class Solution {
public:
    void backTracking(string &s, vector<vector<string>> &res, vector<string> & path, vector<vector<bool>> &isPalindrome, int startIndex)
    {
        if(startIndex >= s.size())
        {
            res.push_back(path);
            return;
        }

        for(int i = startIndex; i < s.size(); i++)
        {
            if(isPalindrome[startIndex][i])
            {
                string subStr = s.substr(startIndex, i - startIndex + 1);
                path.push_back(subStr);
                backTracking(s, res, path, isPalindrome, i + 1);
                path.pop_back();
            }
        }
    }
    vector<vector<string>> partition(string s) {
        vector<vector<bool>> isPalindrome(s.size(), vector<bool>(s.size(), true));
        for(int i = s.size() - 1; i >= 0; i--)
        {
            for(int j = i + 1; j < s.size(); j++)
                isPalindrome[i][j] = ((s[i] == s[j]) && isPalindrome[i + 1][j - 1]);
        }

        vector<vector<string>> res;
        vector<string> path;
        backTracking(s, res, path, isPalindrome, 0);
        return res;
    }
};

// 回溯 + 记忆化搜索
class Solution {
private:
    vector<vector<int>> memory;
    vector<vector<string>> res;
    vector<string> path;
    int n;

public:
    void backTracking(const string& s, int i) {
        if (i == n) {
            res.push_back(path);
            return;
        }
        for (int j = i; j < n; ++j) {
            if (isPalindrome(s, i, j) == 1) {
                path.push_back(s.substr(i, j - i + 1));
                backTracking(s, j + 1);
                path.pop_back();
            }
        }
    }

    // 记忆化搜索中，f[i][j] = 0 表示未搜索，1 表示是回文串，-1 表示不是回文串
    int isPalindrome(const string& s, int i, int j) {
        if (memory[i][j]) {
            return memory[i][j];
        }
        if (i >= j) {
            return memory[i][j] = 1;
        }
        return memory[i][j] = (s[i] == s[j] ? isPalindrome(s, i + 1, j - 1) : -1);
    }

    vector<vector<string>> partition(string s) {
        n = s.size();
        memory.assign(n, vector<int>(n));

        backTracking(s, 0);
        return res;
    }
};
```

<a id="132"></a>

### [132. 分割回文串 II](#palindrome_string)

```C++{.line-numbers}
// 动态规划
class Solution {
public:
    int minCut(string s) {
            vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), true));

            for(int i = s.size() - 1; i >= 0; i--)
                for(int j = i + 1; j < s.size(); j++)
                    if(s[j] != s[i] || !dp[i + 1][j - 1])
                        dp[i][j] = false;

            vector<int> dpCut(s.size(), INT_MAX);
            dpCut[0] = 0;
            for(int i = 1; i < s.size(); i++)
            {
                if(dp[0][i])
                    dpCut[i] = 0;
                else
                    for(int j = 0; j < i; j++)
                    {
                        if(dp[j + 1][i])
                            dpCut[i] = min(dpCut[i], dpCut[j] + 1);
                    }
            }

            return dpCut[s.size() - 1];
    }
};
```

<a id="1278"></a>

### [1278. 分割回文串 III](#palindrome_string)

```C++{.line-numbers}
// 这一个dp[i][j]的含义是：将前j个字符划i次，最小的改变次数
class Solution {
public:
    int palindromePartition(string s, int k) {
        vector<vector<int>> dpCut(s.size(), vector<int>(s.size(), 0));
        for(int i = s.size() - 1; i >= 0; i--)
            for(int j = i + 1; j < s.size(); j++)
                if(s[j] == s[i])
                    dpCut[i][j] = dpCut[i + 1][j - 1];
                else
                    dpCut[i][j] = dpCut[i + 1][j - 1] + 1;

        vector<vector<int>> dp(k + 1, vector<int>(s.size() + 1, INT_MAX));
        // 将前j个字符划分为0次，最小改变次数显然
        for(int i = 0; i < s.size(); i++) dp[0][i + 1] = dpCut[0][i];
        for(int i = 1; i <= k; i++)
        {
            // j > i才能划分i次，否则无法划分
            for(int j = i + 1; j <= s.size(); j++)
            {
                for(int m = i; m < j; m++)
                    dp[i][j] = min(dp[i][j], dp[i - 1][m] + dpCut[m][j - 1]);
            }
        }
        return dp[k - 1][s.size()]; // 划k - 1次得到k个子串
    }
};

// // 这一个dp[i][j]的含义是：将前j个字符串划分为i个子串，最小的改变次数
class Solution {
public:
    int palindromePartition(string s, int k) {
        vector<vector<int>> dpCut(s.size(), vector<int>(s.size(), 0));
        for(int i = s.size() - 1; i >= 0; i--)
            for(int j = i + 1; j < s.size(); j++)
                if(s[j] == s[i])
                    dpCut[i][j] = dpCut[i + 1][j - 1];
                else
                    dpCut[i][j] = dpCut[i + 1][j - 1] + 1;

        vector<vector<int>> dp(k + 1, vector<int>(s.size() + 1, INT_MAX));
        // 将前j个字符划分为1个子串，最小改变次数显然
        for(int i = 1; i <= s.size(); i++) dp[1][i] = dpCut[0][i - 1];

        for(int i = 2; i <= k; i++)
        {
            // j >= i才能分为i个子串
            for(int j = i; j <= s.size(); j++)
            {
                // m 可以等于 j，i <= m <= j
                for(int m = i; m <= j; m++)
                    // 注意dpCut[i][j] 对应的dp[i + 1][j + 1]，整体偏移一位
                    dp[i][j] = min(dp[i][j], dp[i - 1][m - 1] + dpCut[m - 1][j - 1]);
            }
        }
        return dp[k][s.size()];
    }
};
```

<a id="1745"></a>

### [1745. 分割回文串 IV](#palindrome_string)

```C++{.line-numbers}
class Solution {
public:
    bool checkPartitioning(string s) {
        vector<vector<bool>> dp(s.size(), vector<bool> (s.size(), true));
        for(int i = s.size() - 1; i >= 0; i--)
        {
            for(int j = i + 1; j < s.size(); j++)
            {
                if((s[i] != s[j]) || (dp[i + 1][j - 1] == false))
                {
                    dp[i][j] = false;
                }
            }
        }

        for(int i = 0; i < s.size() - 2; i++)
        {
            for(int j = i + 1; j < s.size() - 1; j++)
            {
                if(dp[0][i] && dp[i + 1][j] && dp[j + 1][s.size() - 1])
                    return true;
            }
        }
        return false;
    }
};

// 一个较为通用的写法
class Solution {
public:
    bool checkPartitioning(string s) {
        int k = 3;
        vector<vector<bool>> isPalindrome(s.size(), vector<bool> (s.size(), true));
        for(int i = s.size() - 1; i >= 0; i--)
        {
            for(int j = i + 1; j < s.size(); j++)
            {
                if((s[i] != s[j]) || (isPalindrome[i + 1][j - 1] == false))
                {
                    isPalindrome[i][j] = false;
                }
            }
        }

        vector<vector<bool>> dp(k + 1, vector<bool>(s.size() + 1, false));
        for(int i = 1; i <= s.size(); i++) dp[1][i] = isPalindrome[0][i - 1];

        for(int i = 2; i <= k; i++)
        {
            for(int j = i; j <= s.size(); j++)
            {
                for(int m = i; m <= j; m++)
                {
                    dp[i][j] = dp[i - 1][m - 1] && isPalindrome[m - 1][j - 1];
                    if(dp[i][j] == true)
                        break;
                }
            }
        }

        return dp[k][s.size()];
    }
};
```
