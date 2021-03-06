#

主要是记录侯捷老师的《内存管理》课程中关于 primitive operator 的讲解。

#### new 和 delete

#### array new 和 array delete

在使用类似于`A* buf = new A[size]` 或 `A a[3]` 时，A 必须有 default ctor，否则就会出错，因为这里没法对数组中的每一个元素指定初始参数。

[C++中堆（heap）和栈(stack)的区别（面试中被问到的题目）](https://blog.csdn.net/qq_34175893/article/details/83502412)
[常规 new 和布局 new](https://blog.csdn.net/qq_28306361/article/details/52747936)
[布局 new 操作符引发的有关析构函数的探索与总结](https://www.cnblogs.com/sunrunner/p/3716134.html)
简评：将内存分配和构造函数调用分离开。可以显式调用析构函数
[new 和 malloc 内部的实现方式有什么区别？](https://www.runoob.com/note/26635)
简评：主要区别在于是否调用构造函数和析构函数
[浅谈 C++ 中的 new/delete 和 new[]/delete[]](https://blog.csdn.net/hazir/article/details/21413833)
简评：描述了为何需要配对使用。new 无法被重载，operator new()不是代表重载 new
[new 与 malloc 的区别，以及内存分配浅析](https://www.cnblogs.com/huhuuu/p/3432371.html)
简评：指针在栈空间中，会被自动销毁，指针指向的是堆空间，需要主动销毁。free 源码表面传入的参数必须是待释放内存空间的首地址，而不是这个内存空间中的某一个地址，即先释放后面一部分，之后在释放前面一部分。
[细说 new 与 malloc 的 10 点区别](http://www.linuxidc.com/Linux/2016-01/127591.htm)
简评：比较细致的归纳，从参数传递、内部实现、返回类型等角度区分开
[显式调用构造函数和析构函数](https://www.cnblogs.com/fangyukuan/archive/2010/08/28/1811119.html)
简评：用 placement new 创建的对象时，会自动调用构造函数但是不分配内存，同时在销毁对象时要显示调用析构函数，而不能使用 delete，因为内存管理权不在 placement new 身上
[C++中的 new、operator new 与 placement new](https://www.cnblogs.com/luxiaoxun/archive/2012/08/10/2631812.html)
[C++ 网易面试题“让 new 操作符不分配内存，只调用构造函数”](https://www.jianshu.com/p/b52a5df69c88)
简评：中规中矩的描述
