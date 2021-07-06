<a id="prefixSum"></a>

> **题目汇总**
>
> - **[862. 和至少为 K 的最短子数组](https://leetcode-cn.com/problems/shortest-subarray-with-sum-at-least-k/)** [速览题解](#862)
>   - 简评：找到和大于等于 k 的最短**子数组大小**
> - **[724. 寻找数组的中心下标](https://leetcode-cn.com/problems/find-pivot-index/)** [速览题解](#724)
>   - 简评：找到左侧元素和与右侧元素和相等的**第一个 index**
> - **[560. 和为 K 的子数组](https://leetcode-cn.com/problems/subarray-sum-equals-k/)** [速览题解](#560)
>   - 简评：找到和为 k 的子数组的**个数**
> - **[930. 和相同的二元子数组](https://leetcode-cn.com/problems/binary-subarrays-with-sum/)** [速览题解](#930)
>   - 简评：找到和为 k 的子数组**个数**
> - **[1248. 统计「优美子数组」](https://leetcode-cn.com/problems/count-number-of-nice-subarrays/)** [速览题解](#1248)
>   - 简评：找到有 k 个奇数的子数组**个数**，就是上面的题的变形
> - **[974. 和可被 K 整除的子数组](https://leetcode-cn.com/problems/subarray-sums-divisible-by-k/)** [速览题解](#974)
>   - 简评：找到和为 m，能被 k 整除的子数组**个数**
> - **[523. 连续的子数组和](https://leetcode-cn.com/problems/continuous-subarray-sum/)** [速览题解](#523)
>   - 简评：能否找到和为 m，能被 k 整除的**子数组**
> - **[59 - II. 队列的最大值](https://leetcode-cn.com/problems/dui-lie-de-zui-da-zhi-lcof/)** [速览题解](#59)

<a id="862"></a>

### [862. 和至少为 K 的最短子数组](#prefixSum)

```C++{.line-numbers}
class Solution {
public:
    int shortestSubarray(vector<int>& A, int K) {
        int n = A.size();
        vector<int> sum(n+1, 0);



        for(int i = 0; i < n; i++) //准备工作
            sum[i+1] = sum[i] + A[i];

        deque<int> de;
        int j = 0;
        int res = n+1;

        while(j<=n){

            while(!de.empty() && sum[j] <= sum[de.back()]){ //我们确定我们前面没有高个，有就踢他
                de.pop_back();
            }
            while(!de.empty() && sum[j] - sum[de.front()] >= K){ //从最前面第一位同学开始，进行比较
                res = min( res, j - de.front());
                de.pop_front();
            }
            de.push_back(j); //最后才坐下去
            j++;
        }
        if(res == n+1)
            return -1;
        else
            return res;
    }
};
```

<a id="523"></a>

### [523. 连续的子数组和](#prefixSum)

```C++{.line-numbers}
// 动态规划，注意这个无法通过，因为时间复杂度太高，会超时。但是逻辑是正确的
class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        if(nums.size() == 0) return false;

        vector<int> sum(nums.size() + 1, 0);
        for(int i = 0; i < nums.size(); i++)
        {
            sum[i + 1] = sum[i] + nums[i];
        }
        for(int left = 0; left <= nums.size(); left++)
        {
            for(int right = left + 2; right <= nums.size(); right++)
            {
                int subSum = sum[right] - sum[left];
                if(subSum == k || ((k!=0) && (subSum % k == 0)))
                    return true;
            }
        }
        return false;
    }
};

class Solution {
public:
// 转换为求sum=0的情况
    bool checkSubarraySum(vector<int>& nums, int k) {
        map<int, int> mp;
        mp[0] = -1;
        int sum = 0;
        for (int i = 0; i < nums.size(); i++) {
            sum += nums[i];
            if (k != 0) sum %= k;
            if (mp.find(sum) != mp.end()) {
                if (i - mp[sum] > 1) return true;
            } else {
                mp[sum] = i;
            }
        }
        return false;
    }
};
```

<a id="724"></a>

### [724. 寻找数组的中心下标](#prefixSum)

```C++{.line-numbers}
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        if(nums.size() <= 2) return -1;
        int sum = 0;
        for(int num: nums)
            sum += num;

        int preSum = 0;
        for(int i = 0; i < nums.size(); i++)
        {
            if(preSum == sum - preSum - nums[i])
                return i;
            preSum += nums[i];
        }
        return -1;
    }
};
```

<a id="560"></a>

### [560. 和为 K 的子数组](#prefixSum)

```C++{.line-numbers}
// 逻辑正确，但是当数据量很大时，会超时
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        vector<int> sum(nums.size() + 1, 0);
        for(int i = 0; i < nums.size(); i++)
            sum[i + 1] = sum[i] + nums[i];

        int res = 0;
        for(int i = 0; i < nums.size(); i++)
        {
            for(int j = i; j < nums.size(); j++)
            {
                if(sum[j + 1] - sum[i] == k)
                    res++;
            }
        }
        return res;
    }
};

// 哈希优化
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> mp;
        mp[0] = 1;

        int res = 0, pre = 0;
        for(int i = 0; i < nums.size(); i++)
        {
            pre += nums[i];
            if(mp.find(pre - k) != mp.end())
                res += mp[pre - k];
            mp[pre]++;
        }
        return res;
    }
};
```

<a id="930"></a>

### [930. 和相同的二元子数组](#prefixSum)

```C++{.line-numbers}
class Solution {
public:
    int numSubarraysWithSum(vector<int>& A, int S) {
        unordered_map<int, int> mp;
        mp[0] = 1;
        int res = 0, pre = 0;
        for(int i = 0; i < A.size(); i++)
        {
            pre += A[i];
            if(mp.find(pre - S) != mp.end())
                res += mp[pre - S];
            mp[pre]++;
        }

        return res;
    }
};
```

<a id="1248"></a>

### [1248. 统计「优美子数组」](#prefixSum)

```C++{.line-numbers}
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        unordered_map<int, int> mp;
        mp[0] = 1;
        int res = 0, pre = 0;

        for(int i = 0; i < nums.size(); i++)
        {
            if(nums[i] & 1) pre++;
            if(mp.find(pre - k) != mp.end())
            {
                res += mp[pre - k];
            }
            mp[pre]++;
        }

        return res;
    }
};

class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        vector<int> mp(nums.size() + 1, 0);
        mp[0] = 1;
        int res = 0, pre = 0;

        for(int i = 0; i < nums.size(); i++)
        {
            if(nums[i] & 1) pre++;
            if(pre - k >= 0)
            {
                res += mp[pre - k];
            }
            mp[pre]++;
        }

        return res;
    }
};
```

<a id="974"></a>

### [974. 和可被 K 整除的子数组](#prefixSum)

```C++{.line-numbers}
class Solution {
public:
    int subarraysDivByK(vector<int>& A, int K) {
        unordered_map<int, int> mp;
        mp[0] = 1;

        int res = 0, pre = 0;
        for(int i = 0; i < A.size(); i++)
        {
            pre += A[i];
            int key = (pre % K + K) % K;
            res += mp[key];
            mp[key]++;
        }

        return res;
    }
};
```

<a id="59"></a>

### [59 - II. 队列的最大值](#prefixSum)

```C++{.line-numbers}
class MaxQueue {
private:
    deque<int> qMax;
    deque<int> q;
public:
    MaxQueue() {

    }

    int max_value() {
        if(qMax.size() != 0) return qMax.front();
        else return -1;
    }

    void push_back(int value) {
        q.push_back(value);
        while((qMax.size() != 0) && (qMax.back() < value))
            qMax.pop_back();
        qMax.push_back(value);
    }

    int pop_front() {
        if(q.size() == 0) return -1;
        int ret = q.front();
        if ( qMax.front() == ret)
            qMax.pop_front();
        q.pop_front();
        return ret;
    }
};
```
