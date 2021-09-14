### 常用头文件

```C++{.line-numbers}
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string>
#include <limits.h>
#include <memory>

using namespace std;
```

### 常用内置函数

```C++{.line-numbers}
isalpha(nums[i]) ? 1: 0; // 判断是否为字符（不区分大小写）
islower(nums[i]) ? 1: 0; // 判断是否为字符（小写）
isupper(nums[i]) ? 1: 0; // 判断是否为字符（大写）

isdigit(nums[i]) ? 1: 0; // 判断是否为数字
isalnum(nums[i]) ? 1: 0; // 判断是否为数字或字符

nums[i] = tolower(nums[i]); // 转换为小写
nums[i] = toupper(nums[i]); // 转换为大写

// 需要自己处理输入时需要添加如下语句
ios::sync_with_stdio(false);
cin.tie(nullptr);
// 原因：
// cin，cout之所以效率低，是因为先把要输出的东西存入缓冲区，再输出，导
// 致效率降低，而这段语句可以来打消iostream的输入 输出缓存，可以节省许
// 多时间，使效率与scanf与printf相差无几，还有应注意的是scanf与
// printf使用的头文件应是stdio.h而不是 iostream

// copy函数
//将 A[begin, end] 移动到 B[begin, begin + (A.end() - A.begin())]
copy(A.begin(), A.end(), B.end());
// 将 A 使用 cout 输出
copy(A.begin(), A.end(), ostream_iterator<int>(cout, " ")); cout<<endl; 
// 将 A 使用 cout 有条件输出
copy_if(A.begin(), A.end(), ostream_iterator<int>(cout, " "), [](int x){(return x % 2) != 0});
```

### 常用模板

```C++{.line-numbers}
// 求最大公约数
int gcd(int a,int b)
{
    if(b==0)
        return a;
    else
        return gcd(b,a%b);
}
```

### 输入类型一：从字符串中提取单词，字符串中用 " " 分隔每个单词

- 要用 `getline` 获取字符串，然后自己提取当中的**单词**，不能用 `cin`，会在空格处截断

```C++{.line-numbers}
int main()
{
    string s;
    // 输入的字符串中包含有 " "
    getline(cin, s); 
    // 输入的字符串中不包含 " "，会自动截断
    // cin >> s; 
    vector<string> vec;
    int start = 0;
    int i = 0;
    while (i < s.size())
    {
        if (s[i] == ' ')
        {
            vec.push_back(s.substr(start, i - start));
            start = i + 1;
        }
        i++;
    }
    vec.push_back(s.substr(start, i - start));

    float res = 0;
    for (auto temp : vec)
        res += temp.size();
    printf("%.2f\n", (res / vec.size()));

    return 0;
}
```

- 一些常见的内置函数要记住，方便使用

```C++{.line-numbers}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;
// we have defined the necessary header files here for this problem.
// If additional header files are needed in your program, please import here.

int main()
{
    string s;
    getline(cin, s);
    
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u')
        {
            s[i] = toupper(s[i]);
            
        }
            
        if(s[i] == 'A' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U')
        {
            continue;
        }
        s[i] = tolower(s[i]);
    }
    cout << s << endl;
    
    return 0;
}
```

### 输入类型二：输入不说明有多少个Input Block，以EOF为结束标志

```C{.line-numbers}
while(scanf("%d %d",&a, &b) != EOF)
{ 
//放入计算和的语句即可
}
```

```C++{.line-numbers}
while( cin >> a >> b ) 
{ 
    //放入计算和的语句即可
}
```

### 输入类型三：输入不说明有多少个Input Block,但以某个特殊输入为结束标志

```C{.line-numbers}
#include <stdio.h>
int main()
{
    int a,b;
    while(scanf("%d %d",&a, &b) &&(a!=0 && b!=0))
        printf("%d\n",a+b);
}
```

```C++{.line-numbers}

#include<iostream>
using namespace std;
int main()
{
    int a,b;
    while(cin>>a>>b)
    {
        if(a==0 && b==0)
            break;
        cout<<a+b<<endl;
    }
    return 0;
}
```
