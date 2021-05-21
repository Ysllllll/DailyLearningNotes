<a id="One-question-per-day"></a>

> **题目汇总**
>
> - **[27. 移除元素](https://leetcode-cn.com/problems/remove-element)** [速览题解](#27)
> - **[1011. 在 D 天内送达包裹的能力](https://leetcode-cn.com/problems/capacity-to-ship-packages-within-d-days)** [速览题解](#1011)
> - **[875. 爱吃香蕉的珂珂](https://leetcode-cn.com/problems/koko-eating-bananas)** [速览题解](#875)
> - **[633. 平方数之和](https://leetcode-cn.com/problems/sum-of-square-numbers)** [速览题解](#633)
> - **[136. 只出现一次的数字](https://leetcode-cn.com/problems/single-number)** [速览题解](#136)
> - **[137. 只出现一次的数字 II](https://leetcode-cn.com/problems/single-number-ii)** [速览题解](#137)
> - **[260. 只出现一次的数字 III](https://leetcode-cn.com/problems/single-number-iii/)** [速览题解](#260)
> - **[690. 员工的重要性](https://leetcode-cn.com/problems/employee-importance/)** [速览题解](#690)
> - **[7. 整数反转](https://leetcode-cn.com/problems/reverse-integer/)** [速览题解](#7)
> - **[9. 回文数](https://leetcode-cn.com/problems/palindrome-number/)** [速览题解](#9)
> - **[190. 颠倒二进制位](https://leetcode-cn.com/problems/reverse-bits/submissions/)** [速览题解](#190)
> - **[1720. 解码异或后的数组](https://leetcode-cn.com/problems/reverse-bits/submissions/)** [速览题解](#1720)
> - **[1486. 数组异或操作](https://leetcode-cn.com/problems/xor-operation-in-an-array/)** [速览题解](#1486)
> - **[1734. 解码异或后的排列](https://leetcode-cn.com/problems/decode-xored-permutation/)** [速览题解](#1734)
>   - 简评：这道题的perm是一个1~n的数列组成，只是会打乱顺序
> - **[872. 叶子相似的树](https://leetcode-cn.com/problems/leaf-similar-trees/)** [速览题解](#872)
> - **[50. Pow(x, n)](https://leetcode-cn.com/problems/powx-n/submissions/)** [速览题解](#50)

<a id="27"></a>

### [27. 移除元素](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int left = 0;
        int right = nums.size() - 1;
        while(left <= right)
        {
            if(nums[left] == val)
            {
                swap(nums[left], nums[right]);
                right--;
            }
            else
            {
                left++;
            }
        }
        return right + 1;
    }
};
```

<a id="1011"></a>

---

### [1011. 在 D 天内送达包裹的能力](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int D) {
        // 确定二分查找左右边界
        int left = *max_element(weights.begin(), weights.end()), right = accumulate(weights.begin(), weights.end(), 0);
        while (left < right) {
            int mid = (left + right) / 2;
            // need 为需要运送的天数
            // cur 为当前这一天已经运送的包裹重量之和
            int need = 1, cur = 0;
            for (int weight: weights) {
                if (cur + weight > mid) {
                    ++need;
                    cur = 0;
                }
                cur += weight;
            }
            if (need <= D) {
                right = mid;
            }
            else {
                left = mid + 1;
            }
        }
        return left;
    }
};
```

<a id="875"></a>

### [875. 爱吃香蕉的珂珂](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int right = *max_element(piles.begin(), piles.end());
        int left = 1;

        while(left < right)
        {
            int mid = left + (right - left) / 2;

            int need = 0;
            for(int pile: piles)
            {
                if(pile % mid > 0)
                    need++;
                need += pile / mid;
            }

            if(need <= h)
            {
                right = mid;
            }
            else
            {
                left = mid + 1;
            }
        }
        return left;
    }
};
```

---

<a id="633"></a>

### [633. 平方数之和](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    bool judgeSquareSum(int c) {
        long low = 0;
        long high = sqrt(c);
        while(low <= high)
        {
            long sum = low * low + high * high;
            if(sum == c)
                return true;
            else if(sum < c)
                low++;
            else
                high--;
        }
        return false;
    }
};
```

---

<a id="136"></a>

### [136. 只出现一次的数字](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for(int e: nums) res ^= e;
        return res;
    }
};
```

<a id="137"></a>

### [137. 只出现一次的数字 II](#One-question-per-day)

```C++{.line-numbers}
// 状态机思想
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ones = 0, twos = 0;
        for(int num: nums)
        {
            ones = ones ^ num & ~twos;
            twos = twos ^ num & ~ones;
        }
        return ones;
    }
};

// 位运算
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for (int i = 0; i < 32; i++) { // 因为nums[i]是32位整数，
        // 所以针对每一位的对应二进制数值求和
            int sum = 0;
            for (int num : nums) {
                sum += ((num >> i) & 1); // 提取从右往左数第i位的数值，将所有nums[i]
                // 二进制下的第i位数值进行求和
            }
            if (sum % 3 == 1) { // 如果没办法被3整除，那么说明落单的那个数的第i位是1不是0
                res |= (1 << i);
            }
        }
        return res; // 输出结果
    }
};
```

<a id="260"></a>

### [260. 只出现一次的数字 III](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        int x = 0, y = 0, n = 0, m = 1;
        for(int num : nums)         // 1. 遍历异或
            n ^= num;
        while((n & m) == 0)         // 2. 循环左移，计算 m
            m <<= 1;
        for(int num : nums) {       // 3. 遍历 nums 分组
            if(num & m) x ^= num;   // 4. 当 num & m != 0
            else y ^= num;          // 4. 当 num & m == 0
        }
        return vector<int> {x, y};  // 5. 返回出现一次的数字
    }
};
```

---

<a id="690"></a>

### [690. 员工的重要性](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    int getImportance(vector<Employee*> employees, int id) {
        int idFind = 0;
        for(; idFind < employees.size(); idFind++)
        {
            if(employees[idFind]->id == id)
                break;
        }
        if(idFind == employees.size())
            return 0;


        if(employees[idFind]->subordinates.size() == 0) return employees[idFind]->importance;

        int res = employees[idFind]->importance;
        for(int i = 0; i < employees[idFind]->subordinates.size(); i++)
        {
            res += getImportance(employees, employees[idFind]->subordinates[i]);
        }

        return res;
    }
};
```

<a id="7"></a>

### [7. 整数反转](#One-question-per-day)

```C++{.line-numbers}
// 第一版
class Solution {
public:
    int reverse(int x) {
        if(x == 0 || x == -2147483648) return 0;
        int sign = x > 0? 0: 1;
        if(x < 0) x = -x;

        int res = 0;
        int bit = 0;
        while(x > 0)
        {
            res = res * 10 + x % 10;
            x = x / 10;

            bit++;
            if(bit == 9 && x > 0 && res > 214748364)
            {
                return 0;
            }
        }

        return sign == 0? res: -res;
    }
};

// 第二版
class Solution {
public:
    int reverse(int x) {
        if(x == 0 || x == INT_MIN) return 0;
        int sign = x > 0? 0: 1;
        if(x < 0) x = -x;

        int res = 0;
        while(x > 0)
        {
            res = res * 10 + x % 10;
            x = x / 10;

            if( x > 0 && res > INT_MAX / 10)
            {
                return 0;
            }
        }

        return sign == 0? res: -res;
    }
};

//最终简化版
class Solution {
public:
    int reverse(int x) {
        int y = 0; // y用来承接翻转后的整数
        while (x != 0) {
            if (y > INT_MAX / 10 || y < INT_MIN / 10) return 0; // 溢出，那么输出为0(这是题目要求)
            y = y * 10 + x % 10; // y在变化的同时x也在变化，效率高
            // x % 10的目的是得到个位的数值
            x = x / 10;   // x / 10相当于“右移移位运算”，是地板除。
        }
        return y; // 得到结果
    }
};

```

<a id="9"></a>

### [9. 回文数](#One-question-per-day)

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

<a id="190"></a>

### [190. 颠倒二进制位](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {

        uint32_t res = 0, single = 0;

        for(int i = 0; i < 32; i++)
        {
            single = ((n >> i) & 1);
            res = (res | (single<<(31 - i)));
        }
        return res;
    }
};
```

<a id="1720"></a>

### [1720. 解码异或后的数组](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    vector<int> decode(vector<int>& encoded, int first) {
        vector<int> decode(encoded.size() + 1, 0);
        decode[0] = first;

        for(int i = 1; i < decode.size(); i++)
        {
            decode[i] = encoded[i - 1] ^ decode[i - 1];
        }

        return decode;
    }
};
```

<a id="1720"></a>

### [1720. 解码异或数组](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    int xorOperation(int n, int start) {
        int res = 0;
        for(int i = 0; i < n; i++)
            res = res ^ (start + 2 * i);
        return res;
    }
};

class Solution {
public:
    int computeXOR(int n)
    {
        switch(n%4)
        {
            case 0: return n;
            case 1: return 1;
            case 2: return n + 1;
        }
        //case3
        return 0;
    }
    int xorOperation(int n, int start) {
        //最低为表示为b0
        int b0 = n & start & 1;
        int s = start/2;
        int res = computeXOR(s-1)^computeXOR(s+n-1);
        return (res<<1) + b0;
    }
};
```

<a id="872"></a>

### [872. 叶子相似的树](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    void leafSimilarHelper(TreeNode* root, vector<int> &v)
    {
        if(root == nullptr) return ;
        if(root->left == nullptr && root->right == nullptr)
        {
            v.push_back(root->val);
            return;
        }
        if(root->left != nullptr) leafSimilarHelper(root->left, v);
        if(root->right != nullptr) leafSimilarHelper(root->right, v);
    }
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> v1, v2;
        leafSimilarHelper(root1, v1);
        leafSimilarHelper(root2, v2);
        return v1 == v2;
    }
};
```

<a id="50"></a>

### [50. Pow(x, n)](#One-question-per-day)

```C++{.line-numbers}

class Solution {
public:
    double myPow(double x, int n) {
        if(n == 0) return 1;
        if(n == 1) return x;
        if(abs(x - 1) < 1e-10) return x;
        
        if(n < 0) x = 1 / x;

        double res = 1;
        while(n != 0)
        {
            if(n % 2 & 1) res = res * x;
            n = n / 2;
            x = x * x;
        }

        return res;
    }
};
```

<a id="1734"></a>

### [1734. 解码异或后的排列](#One-question-per-day)

```C++{.line-numbers}
class Solution {
public:
    vector<int> decode(vector<int>& encoded) {
        int total = 0;
        for(int i = 1; i <= encoded.size() + 1; i++)
            total = total ^ i;
        for(int j = 1; j < encoded.size(); j += 2)
            total = total ^ encoded[j];

        vector<int> perm(encoded.size() + 1, 0);
        perm[0] = total;

        for(int i = 1; i < perm.size(); i++)
            perm[i] = perm[i - 1] ^ encoded[i - 1];
        
        return perm;
    }
};
```
