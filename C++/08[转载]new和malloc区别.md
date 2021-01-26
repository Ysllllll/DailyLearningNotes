#

这里先对 `new` 和 `delete` 简单进行一下总结，然后再细说 `new` 和 `malloc` 的区别。

#### 一、`new` 和`delete`

C语言提供了 `malloc` 和 `free` 两个系统函数，完成对堆内存的申请和释放。而C++则提供了两个关键字 `new` 和 `delete`；

##### 1.1 规则

- `new` / `delete` 是关键字，效率高于 `malloc` 和 `free`。
- 配对使用，避免内存泄漏和多重释放。
- 避免交叉使用，比如 `malloc` 申请空间 `delete` 释放，`new` 出的空间被 `free`。
- `new` / `delete` 主要是用在类对象的申请和释放。申请的时候会调用构造器完成初始化，释放的时候，会调用析构器完成内存清理。

##### 1.2 `new` / `new[]` 用法

```c++{.line-numbers}
    //开辟单地址空间
    int *p = new int;  //开辟大小为sizeof(int)空间
    int *q = new int(5); //开辟大小为sizeof(int)的空间，并初始化为5。
    //开辟数组空间
    //一维
    int *a = new int[100]{0};//开辟大小为100的整型数组空间，并初始化为0。
    //二维
    int (*a)[6] = new int[5][6];
    //三维
    int (*a)[5][6] = new int[3][5][6];
    //四维及以上以此类推。
```

##### 1.3 delete/delete[] 用法

```C++{.line-numbers}
    //释放单个int空间
    int *a = new int;
    delete a;
    //释放int数组空间
    int *b = new int[5];
    delete []b;
```

#### 二、new和malloc的区别

##### 2.1 属性

`new` 和 `delete` 是C++关键字，需要编译器支持；`malloc` 和 `free` 是库函数，需要头文件支持。

##### 2.2 参数

使用 `new` 操作符申请内存分配时无须指定内存块的大小，编译器会根据类型信息自行计算。而 `malloc` 则需要显式地指出所需内存的尺寸。

##### 2.3 返回类型

`new` 操作符内存分配成功时，返回的是对象类型的指针，类型严格与对象匹配，无须进行类型转换，故 `new` 是符合类型安全性的操作符。而 `malloc` 内存分配成功则是返回 `void *`，需要通过强制类型转换将 `void*` 指针转换成我们需要的类型。

##### 2.4 自定义类型

`new` 会先调用 `operator new` 函数，申请足够的内存（通常底层使用 `malloc` 实现）。然后调用类型的构造函数，初始化成员变量，最后返回自定义类型指针。`delete` 先调用析构函数，然后调用 `operator delete` 函数释放内存（通常底层使用 `free` 实现）。

malloc/free是库函数，只能动态的申请和释放内存，无法强制要求其做自定义类型对象构造和析构工作。

##### 2.5 “重载”

C++允许自定义 `operator new` 和 `operator delete` 函数控制动态内存的分配。

##### 2.6 内存区域

`new` 做两件事：分配内存和调用类的构造函数，`delete` 是：调用类的析构函数和释放内存。而 `malloc` 和 `free` 只是分配和释放内存。

`new` 操作符从自由存储区（`free store`）上为对象动态分配内存空间，而 `malloc` 函数从堆上动态分配内存。自由存储区是C++基于 `new` 操作符的一个抽象概念，凡是通过 `new` 操作符进行内存申请，该内存即为自由存储区。而堆是操作系统中的术语，是操作系统所维护的一块特殊内存，用于程序的内存动态分配，C语言使用 `malloc` 从堆上分配内存，使用 `free` 释放已分配的对应内存。自由存储区不等于堆，如上所述，布局 `new` 就可以不位于堆中。

##### 2.7 分配失败

`new` 内存分配失败时，会抛出 `bac_alloc` 异常。`malloc` 分配内存失败时返回 `NULL`。

##### 2.8 内存泄漏

内存泄漏对于 `new` 和 `malloc` 都能检测出来，而 `new` 可以指明是哪个文件的哪一行，`malloc` 确不可以。

#### Reference

- [经典面试题之new和malloc的区别](https://blog.csdn.net/nie19940803/article/details/76358673)
- [C++中，new/delete和malloc/free的区别](https://www.cnblogs.com/litao-tech/p/4318424.html)
