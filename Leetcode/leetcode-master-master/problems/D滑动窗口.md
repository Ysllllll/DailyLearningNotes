<a id="sliding_window"></a>

> **题目汇总**
>
> - **[3. 无重复字符的最长子串](https://leetcode-cn.com/problems/longest-substring-without-repeating-characters/)** [速览题解](#3)
>   - 简评：注意动态规划的分析，dp[i]是什么含义
> - **[234. 回文链表](https://leetcode-cn.com/problems/palindrome-linked-list/)** [速览题解](#234)

<a id="3"></a>

### [3. 无重复字符的最长子串](#sliding_window)

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
            {
                maxlen = r - l + 1;
            }


        }
        return maxlen;
    }
};

// 
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

<a id="9"></a>

### [9. 回文数](#sliding_window)

```C++{.line-numbers}

```

