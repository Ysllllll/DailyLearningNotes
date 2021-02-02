#

主要是记录侯捷老师的《内存管理》课程中关于primitive operator的讲解。

#### new和delete


#### array new和array delete

在使用类似于`A* buf = new A[size]` 或 `A a[3]` 时，A必须有default ctor，否则就会出错，因为这里没法对数组中的每一个元素指定初始参数。