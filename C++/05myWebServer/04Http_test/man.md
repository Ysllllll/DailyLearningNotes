MimeType 相关的知识点：

- [pthread_once 详解和使用](https://blog.csdn.net/derr96677169/article/details/101332791)
- [HTTP content-type](https://www.runoob.com/http/http-content-type.html)

---

shared_ptr 相关知识点：

一共有四类智能指针：

- auto_ptr
- unique_ptr
- shared_ptr：构造函数必须显式调用、一共有三种初始化方式，但是只推荐其中的两种初始化方式
- weak_ptr

[C++干货系列——你该如何优雅地构造 shared_ptr？](https://zhuanlan.zhihu.com/p/259423497)
[窥见 C++11 智能指针](https://zhuanlan.zhihu.com/p/78123220)
[C++ 智能指针 shared_ptr 详解与示例](https://blog.csdn.net/shaosunrise/article/details/85228823?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.baidujs&dist_request_id=&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.baidujs)

下面两篇文章实现了一个智能指针，可以参考：

- [C++ 智能指针 shared_ptr 模板实现原理](https://blog.csdn.net/szqsdq/article/details/71330652?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-4.baidujs&dist_request_id=1328690.8533.16165592206838795&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-4.baidujs)
- [智能指针原理分析与自己的 shared_ptr 实现](https://blog.csdn.net/huoyefeiwu/article/details/46138093?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-0&spm=1001.2101.3001.4242)

---

关于 enable_shared_from_this 的使用

[C++11 新特性之十：enable_shared_from_this](https://blog.csdn.net/caoshangpa/article/details/79392878)

---

关于 weak_ptr 的 lock 函数

---

munmap()函数
mmap()函数
[Linux 进程间通信——内存共享映射](https://zhuanlan.zhihu.com/p/266813445)
[Linux 的内存共享映射(mmap 和 munmap)](https://cloud.tencent.com/developer/article/1594757)
[进程间通信---共享内存（Shared Memory）](https://blog.csdn.net/Qiana_/article/details/80150565)
[Linux 的内存共享映射(mmap 和 munmap)](https://cloud.tencent.com/developer/article/1594757)

c_str()函数
[c_str()的用法](https://blog.csdn.net/fnjejkd/article/details/83006632?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.baidujs&dist_request_id=1328689.11655.16165953099383391&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.baidujs)

四种转型

- static_cast<>
  ...
