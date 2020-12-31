### 2、C++基础：typedef

> 1.参考《Essential C++》P122：typedef 可以为某个型别设定另一个不同的名称，其通用形式为：typedef existing_type new_name;  
> 2.参考 [C 语言 typedef：给类型起一个别名](http://c.biancheng.net/cpp/html/100.html)  
> 重点主要是弄明白  
> - 1.写法：哪一部分是别名，哪一部分是别名所代表的数据；
> - 与 #define 的区别。

写法简单点的如：

```
typedef int INTEGER;
INTEGER a,b;//等效于int a,b;
```

写法复杂点的如：

```
typedef char ARRAY20[20];//表示ARRAY20是类型为char[20]的别名
ARRAY20 a1,a2,a3;//等效于char a1[20],a2[20],a3[20]

typedef struct stu{
    char name[20];
    int age;
    char sex;
} STU;
STU body1,body2;//等效于struct stu body1, body2;
```
下面是参考2链接中提供的示例
```
#include <stdio.h>

typedef char (*PTR_TO_ARR)[30];
typedef int (*PTR_TO_FUNC)(int, int);

int max(int a, int b){
    return a>b ? a : b;
}

char str[3][30] = {
    "http://c.biancheng.net",
    "C语言中文网",
    "C-Language"
};

int main(){
    PTR_TO_ARR parr = str;
    PTR_TO_FUNC pfunc = max;
    int i;
   
    printf("max: %d\n", (*pfunc)(10, 20));
    for(i=0; i<3; i++){
        printf("str[%d]: %s\n", i, *(parr+i));
    }

    return 0;
}

运行结果：
max: 20
str[0]: http://c.biancheng.net
str[1]: C语言中文网
str[2]: C-Language
```
>typedef是赋予现有类型一个新的名字，而不是创建新的类型。

typedef与#define的区别体现为如下两点：  
- 可以使用其它类型说明符对宏类型名进行扩展，但对typedef所定义的类型名却不能这么做。例如：
```
#define INTERGE int
unsigned INTERGE n;  //使用unsigned类型说明符对宏进行扩展，没问题

typedef int INTERGE;
unsigned INTERGE n;  //错误，不能在 INTERGE 前面添加unsigned
```
- 在连续定义几个变量的时候typedef能够保证定义的所有变量均为同一类型，而#define则无法保证。例如：
```
#define PTR_INT int *
PTR_INT p1, p2;//等效为int *p1,p2; p1是int*型，而p2是int型

typedef int * PTR_INT
PTR_INT p1, p2;//等效为int *p1,p2; p1和p2都是int*型
```

这里又让我回忆起了数组指针和指针数组，一直都是模模糊糊的感觉。链接中也出现了<数组和指针绝不等价，数组是另外一种类型>的说法，针对这两个这里检索了重新整理下：
### 3、C++基础：sizeof(array)和sizeof(pointer)
>参考[数组和指针绝不等价，数组是另外一种类型](https://blog.csdn.net/qq_38325803/article/details/102751865)。  
这里主要是通过对数组和指针进行sizeof运算来说明数组与指针不等价。
```
#include <stdio.h>
 
int main(){
    int a[6] = {0, 1, 2, 3, 4, 5};
    int *p = a;
    int len_a = sizeof(a) / sizeof(int);
    int len_p = sizeof(p) / sizeof(int);
    printf("len_a = %d, len_p = %d\n", len_a, len_p);
    return 0;
}
运行结果：
len_a = 6, len_p = 1
```
先解析sizeof这个运算符，注意sizeof()并不是一个函数，而是一个操作符。编译器在编译的过程中会创建一张专门的表格，用来保存名字以及名字对应的数据类型、地址、作用域等信息。使用sizeof()这个操作符我们可以从这张表格中查询到符号的长度。  
那么上面代码中 **符号a** 和 **符号p** 在这个表格中的数据类型会是什么呢？  
对于**符号a**，它的类型是int[6]，表示这是一个拥有 6 个 int 数据的集合，1 个 int 的长度为 4，6 个 int 的长度为 4×6 = 24，sizeof 很容易求得；  
对于**符号p**，它的类型是int*，在 32 位环境下长度为 4，在 64 位环境下长度为 8；  
**数组类型**：这里就有一个我们经常忽视的细节：数组也是有类型的，它的类型不是数组名指针和数组长度的叠加。它是基本类型派生而来的稍微复杂点的类型，同样我们也不能认为这种派生是简单的叠加。  
**数组名**：与普通变量名相比，数组名既有一般性也有特殊性：一般性表现在数组名也用来代指特定的内存块，也有类型和长度；特殊性表现在数组名有时候会转化为指针。

### 4、C++基础：数组指针和指针数组
>参考[数组指针和指针数组的区别，C语言数组指针和指针数组区别详解](http://c.biancheng.net/view/335.html)。  
>重点是在如何记忆并快速的判断，方法当然因人而异。每次面对那些听得很熟的东西，要半天才能反应过来是一件令人烦躁的事情，这种事情能少一件是一件。

关于数组指针和指针数组的区别，链接里面的图例解释得非常清楚，这里就不贴图示了。
```
int *p1[5];//类型为数组，数组里面的元素是指针。
int (*p2)[5];//类型为指针，指向一个int[5]类型的元素。

int *p1[5];  //指针数组：(int*)   p1[5]：(int*)指针   p1[5]数组
int (*p2)[5];//数组指针：(int[5]) (*p2)：(int[5])数组 (*p2)指针
```

这里还有一个比较重要的点：
```
int arr[5]={1，2，3，4，5};
int (*p1)[5] = &arr;
/*下面是错误的*/
int (*p2)[5] = arr;
```
&arr是值整个数组的首地址，而arr是指数组首元素的首地址，所表示的意义不同，但是两者的值是一样的。  
但是在上面的代码片段中最后一个赋值语句在 Microsoft Visual Studio 2010 （以VS举例，其它编译器也会报错）中编译的时候，将会提示如下的错误信息：“a value of type"int*"cannot be used to initialize an entity of type"int(*)[5]"”。  
具体而言，在 C 语言中，赋值符号“ = ”两边的数据类型必须相同，不能只是值相同。如果数据类型不同，则需要显式或者隐式类型转换。在这里，p1 和 p2 都是数组指针，指向的是整个数组。p1 这个定义的“=”号两边的数据类型完全一致，而 p2 这个定义的“=”号两边的数据类型就不一致了（左边的类型是指向整个数组的指针，而右边的数据类型是指向单个字符的指针），因此会提示如上的错误信息。

### C++基础：C++对象模型

> [C++对象模型](https://www.cnblogs.com/skynet/p/3343726.html)
