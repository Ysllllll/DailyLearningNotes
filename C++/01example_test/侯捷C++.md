## 基于对象编程

- 琐碎前述

  - c++文件布局：头文件`(.h)` + 实现文件`(.cc)`
  - 头文件布局：前置声明、类声明、类定义
  - 核心内容是：**类成员函数设计**
  - 可参考：《Effective C++》、标准库实现

- class without pointer member（complex 实现）

  - 构造函数
    - 实现为 `public` 成员
      - 可重载多个构造函数：`new classtype[n]` 创建一个对象数组，要求 `classtype` 必须有默认构造函数，此时无法给定每个对象初值。`const` 也是重载关键字，`static`不是
      - 默认实参：从右至左
      - 初始列：调用类成员对象的**构造函数**
      - 函数体：调用类成员对象的**拷贝构造函数**和**拷贝赋值函数**
      - 构造函数必须初始化所有类成员对象
    - 实现为 `private` 成员
      - `singleton` 设计模式
      - 无法构造栈对象：`classtype tmp;`报错
      - 可以构造堆对象：`classtype *tmp = classtype::create();`。`create()` 为类中 `static` 函数，函数主体 `{return new classtype;}`
  - 成员函数
    - `const` 成员函数：保证不会改变**对象的数据**，普通成员函数则会改变数据
      - 当成员函数的 `const` 和 `non-const` 版本同时存在，`const object` 只会（只能）调用 `const` 版本，`non-const object` 只会（只能）调用 `non-const` 版本
    - `const` 参数
      - 传入 `pass`：保证在函数体内不会改变**该参数**
      - 传出 `return`：保证在函数体外不会改变**该参数**
    - 参数传递
      - `pass by value (to const)`：非内置类型会调用拷贝构造函数
      - `pass by reference (to const)`：多态时需要注意对象切割问题
      - `return by value (to const)`：非内置类型会调用拷贝构造函数
      - `return by reference (to const)`：不能 `return` 一个局部变量的引用。`return` 的可以是一个指针所指对象
    - `friend`：可直接访问 `private`数据成员
      - 对象之间的互为 `friend`：一个对象的成员函数可以直接访问另一个对象的私有数据成员
      - 具体参考 `complex` 实现
    - 隐含的第一个参数为 `this` 指针，指代当前对象
  - `static` 成员函数
    - 无 `this` 指针，使用范围为**当前类域**
    - 不可操作普通成员变量，只能操作 **`static`成员变量**。考虑类域在最开始时没有一个对象存在，`static` 函数自然不能操作这些不存在的东西。`static` 成员仅仅表明可见域，不表明访问权限
  - 函数重载
    - 操作符重载 为成员函数：有 `this` 指针
    - 操作符重载 为非成员函数：无 `this` 指针
    - 重载标志
      - 函数名、参数列表、函数后缀、`const`是重载标志
      - `static`，引用参数、返回类型不是重载标志

- class with pointer member（String 实现）

  - `big three`
    - 拷贝构造函数：自我验证，深拷贝
    - 拷贝赋值函数：自我验证，深拷贝
    - 析构函数：一定要有，用于 `delete pointer` 指向的对象，有继承体系时一定要写为虚析构函数
  - 小记：注意 `this` 指代的是什么，拷贝赋值和拷贝构造函数里面的自我验证 `if(this == &s) return`

- `object` 的生命周期

  - **栈对象**：离开作用域时 **自动** 调用析构函数
  - **`static local` 对象**：整个程序结束时 **自动** 调用析构函数
  - **`global` 对象**：离开作用域时（是整个程序） **自动** 调用析构函数
  - **堆对象**：调用 `delete` 时 **主动** 调用析构函数（指向堆对象的指针为栈对象）
  - 常量对象：只能调用**常量成员函数**
  - 非常量对象：可以调用常量成员函数，也可以调用非常量成员函数
  - 临时对象
  - 如何禁止产生栈对象，而只能产生堆对象：构造函数私有化，成员函数分配堆对象并将该对象返回给用户
  - 如何禁止产生堆对象，而只能产生栈对象：将 `operator new` 和 `operator delete` 私有化，用户将无法使用 `new`

- `new` 与 `delete`

  - `new` 分解

    ```C++
    complex *pc = new complex(1, 2);
    //分解
    void *mem = operator new(sizeof(complex));//分配内存
    pc = static_cast<complex*>(mem);//转型
    pc->complex::complex(1, 2);//构造函数
    ```

  - `delete` 分解

    ```C++
    delete pc;
    //分解
    complex::~complex(pc);//析构函数
    operator delete(pc);//释放内存
    ```

  - `new` 和 `delete` 是关键字，不是运算符，编译器会将这个关键字修饰的语句分解为三个执行语句。`new` 和 `delete` 操作的对象都堆对象
    - 堆对象和栈对象的构造函数的调用都由编译器产生，并自动调用。具体点，堆对象会在**堆中开辟一个空间**，然后在这个空间上创建对象。而栈对象只需要**移动程序栈的栈顶指针，以预留出能够容纳对象的空间**，然后在这个空间上创建对象
    - 堆对象和栈对象的析构函数的调用存在区别，堆对象需要**主动调用**，如果不调用堆对象会一直存在。而栈对象会**自动调用**，因为**栈回滚**，会在离开这个作用域（程序**自动**移动栈顶指针）的时候清除这个作用域所占用内存的内容
  - `array new` 和 `array delete`
    - 若类含有指针数据成员，同时没有搭配使用，会造成内存泄露。这里的内存泄露在**第二级的 `new`**（第一级 `new` 为用户调用：`string *p = new string[3]`，第二级 `new` 为构造函数内部调用：`char *m_str = new char[strlen(str)+1]`;）
  - 重载 `::operator new/delete，::operator new[]/delete[]`

    - 影响范围：全局
    - 重载理由：实现内存管理

    ```C++
    //注意返回值与参数
    void* operator new(size_t size);
    void* operator new[](size_t size);
    void  operator delete(void* ptr);
    void  operator delete[](void* ptr);
    ```

  - 重载 `member operator new/delete，member operator new[]/delete[]`
    - 影响范围：分配该类对象
    - 重载理由：实现内存池
  - 重载 `new()、delete()`

- 进一步补充
  - static：仅仅表明可见域，不表明访问权限，也可以从共享的角度来看
  - `namespace`
  - 自定义类型转换
    - 将自定义类型通过 **转换函数`conversion function`** 转换为其它内置类型（`double、int、long...`）
    - 将其它类型通过 **隐式/显式调用构造函数** 转换为自定义类型
      - `explicit` 关键字修饰构造函数会将这一动作限定为 **仅显式调用**

## 面向对象编程

- 复合（`has-a`）
  - 数据成员中有**非基本类型**
  - 构造与析构顺序：由编译器来做
    - 构造由内而外
    - 析构由外而内
  - 设计模式：`Adapter`
- 委托
  - 数据成员中有**非基本类型指针**
  - 构造与析构顺序
  - 设计模式：`Handle/Body`
- 继承（`is-a`）
  - 类继承
    - 继承的数据成员：占用内存
    - 继承的函数成员：继承的是调用权，而不能从内存的角度去考虑
    - 继承的核心问题是子类是否可以重新定义继承来的函数。
    - `non-virtual`：不希望 `derived class` 重新定义它
    - `virtual`：希望 `derived class` 重新定义它，且已经有默认定义
    - `pure virtual`：希望 `derived class` 一定要重新定义它，且没有默认定义
  - 构造与析构顺序：由编译器来做
    - 构造由内而外
    - 析构由外而内
  - 设计模式：`Template method` （与虚函数搭配）
- 复合 + 继承
  - 构造与析构顺序
  - 设计模式：无
- 委托 + 继承
  - 构造与析构顺序
  - 设计模式：`Observer、Composite、Prototype`

## 泛型编程

- pointer-like classes
  - 操作符重载： `*` 和 `->`
    - `*`：`T& operator*() const { return*px; }`
    - `->`：`T* operator->() const {return px; }`，注意这里返回的是指针，因为`->`操作符可以一直作用下去
  - 实例：智能指针、迭代器
- function-like classes

  ```C++{.line-numbers}
  class stringAppend
  {
  public:
      explicit stringAppend(const string &str="") : ss(str) {}
      void operator()(const string &str) const
      {
          cout << str << ' ' << ss << endl;
      }
  private:
      const string ss;
  };
  stringAppend my("and world!"); //调用的是构造函数
  my("hello");//调用的是operator()成员函数
  stringAppend()("hello");//注意第一个括号是生成临时对象，第二括号才是函数调用。
  stringAppend("world")("hello");//注意第一个括号是生成临时对象（这个临时对象有初值，会调用构造函数），第二括号才是函数调用。
  ```

- 模板

  - 类模板：需要**显式指定模板参数**，`vector<int> a;`
  - 函数模板：不需要显式指定模板参数，编译器会自动推导，`reverse(a.begin(), a.end());`
  - 成员模板：多用于构造函数。将**构造函数**设计为一个成员模板，在定义一个对象的时候，用于初始化对象的初值的类型将绑定到这个成员模板的模板参数上：`pair<BaseType1, BaseType2> p2(pair<DerivedType1, DerivedType2>())`
  - 缺点：会造成代码膨胀

- 模板特化
  - 全特化：泛化
  - 偏特化：个数的偏
  - 偏特化：范围的偏，仅针对指针
  - 模板模板参数...

## 对象模型（仅谈涉及虚函数的情况）

- 继承
  - 继承的数据成员：占用内存
  - 继承的函数成员：继承的是调用权，而不能从内存的角度去考虑
- 虚函数指针
- 虚函数表
- 静态绑定（非指针运算或传递）
  - 汇编层面：`call xxx`
  - 可能出现的问题：若给一个父类的形参传递一个子类的实参，子类实参将被切割
- 动态绑定（指针运算或传递）
  - 汇编层面：`(*(p->vptr)[n])(p)`
  - 三大条件
    - 指针
    - 向上转型：即给一个父类的指针形参传递了一个子类的指针实参
    - 调用虚函数
  - 可能出现的问题：当 derived class 对象经由一个 base class 指针被删除，而该 base class 带着一个 non-virtual 析构函数，其结果未有定义

## 琐碎

- 数量不定的模板参数

  ```C++
  template<typename T, typename... Type>
  void print(const T& firstArg, const Type&... args);
  ```

- auto

  - 编译器自动推导表达式类型

    ```C++
    vector<int> vec;
    auto ite1 = find(vec.begin(), vec.end(), target);//能够自动推导
    auto ite2;
    ite2 = find(vec.begin(), vec.end(), target);//不能自动推导
    ```

- ranged-base for

  ```C++
  for(int i : {2, 3, 4, 5, 6});
  for(auto  elem : vec)//改变elem时，不会改变vec中的值
  for(auto& elem : vec)//改变elem时，会改变vec中的值
  ```

- reference
  - 必须初始化、不是重载标志（重载标志：函数名 + 参数列表 + 函数后缀）
  - 本质
    - 底层：指针
    - 顶层：对象和其引用的大小相同、地址也相同，这是由编译器给出的假象
  - 用途
    - 不用于常量声明
    - 用于参数类型和返回类型描述
