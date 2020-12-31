
### 2、C++基础：虚继承
>[虚继承和虚基类详解](2020-12-9.mdhttp://c.biancheng.net/view/2280.html)  
>虚继承和虚基类的出现是为了解决这样一个问题：在菱形继承中，在派生类中访问继承来的成员变量时产生命名冲突。

```C++
//间接基类A
class A{
protected:
    int m_a;
};

//直接基类B
class B: public A{
protected:
    int m_b;
};

//直接基类C
class C: public A{
protected:
    int m_c;
};

//派生类D
class D: public B, public C{
public:
    void seta(int a){ m_a = a; }  //命名冲突
    void setb(int b){ m_b = b; }  //正确
    void setc(int c){ m_c = c; }  //正确
    void setd(int d){ m_d = d; }  //正确
private:
    int m_d;
};

int main(){
    D d;
    return 0;
}
```

这段代码实现了一种菱形继承，Derived Class D中试图直接访问成员变量 m_a，结果发生了错误，因为Derived Class B 和Derived Class C 中都有成员变量 m_a（从 Base Class A 继承而来），编译器不知道选用哪一个，所以产生了歧义。为了消除歧义，我们可以在 m_a 的前面指明它具体来自哪个类：```void seta(int a){ B::m_a = a; }```，这样表示使用 B 类的 m_a。当然也可以使用 C 类的：  ```void seta(int a){ C::m_a = a; }```。  

为了解决多继承中的命名冲突和冗余数据，C++提出了虚拟继承，使得在派生类中只保留一份间接基类的成员。上面的例子改为如下：

```C++
//间接基类A
class A{
protected:
    int m_a;
};

//直接基类B
class B: virtual public A{  //虚继承
protected:
    int m_b;
};

//直接基类C
class C: virtual public A{  //虚继承
protected:
    int m_c;
};

//派生类D
class D: public B, public C{
public:
    void seta(int a){ m_a = a; }  //正确
    void setb(int b){ m_b = b; }  //正确
    void setc(int c){ m_c = c; }  //正确
    void setd(int d){ m_d = d; }  //正确
private:
    int m_d;
};

int main(){
    D d;
    return 0;
}
```

这段代码使用虚继承重新实现了菱形继承，这样在Derived Class D 中就只保留了一份成员变量 m_a，直接访问就不会再有歧义了。  
虚拟继承是让某一个类做出声明，声明它愿意共享它的基类。其中，这个被共享的基类被称为**虚基类（Virtual Base Class）**，在这里这个虚基类就是Base Class A，在这种情况下，不论继承体系中虚基类出现了多少次，在派生类中都只包含一份虚基类的成员。  

我们可以发现虚继承一个特征：**必须在虚派生的真实需求出现之前就已经完成虚派生的操作**。例如上面的例子中，只有Derived Class D出现时才出现了虚派生的需求，为了满足这个需求，Derived Class B 和Derived Class C才都必须完成虚派生的操作。换个角度讲：**虚派生只影响从指定了虚基类（A）的派生类（B、C）中进一步派生出来的类（D），它不影响虚基类（A）的派生类本身（B、C）。所以我们将位于继承体系中间层次的基类（例如本例中的B、C）将其继承声明为虚继承一般不会带来影响。**
>**虚继承的实例**：C++标准库中的iostream类就是一个虚继承的实际应用，是典型的菱形继承。此时istream类和ostream类必须采取虚继承，否则将导致在iostream类中有两份base_ios类的成员。

```C++
//iostream的间接基类base_ios
class base_ios{};

//iostream的直接基类istream
class istream: virtual public A{};  //虚继承

//iostream的直接基类ostream
class ostream: virtual public A{};  //虚继承

//派生类iostream
class iostream: public istream, public ostream{};
```

以上阐述的是Base Class A中的成员变量m_a在Derived Class D中的访问情况。延伸开来，如果在Derived Class B和Derived Class C中也可能定义有m_a成员变量，此时的实际情况会如何呢？以第一个代码段为例，假设Base Class A定义了一个名为 x 的成员变量，当我们在Derived Class D中直接访问 x 时，会有三种可能性：  

- 如果 B 和 C 中都没有 x 的定义，那么 x 将被解析为 A 的成员，此时不存在二义性。
- 如果 B 或 C 其中的一个类定义了 x，也不会有二义性，派生类的 x 比虚基类的 x 优先级更高。
- 如果 B 和 C 中都定义了 x，那么直接访问 x 将产生二义性问题。

### 3、C++基础：C++类成员访问权限及继承方式

>主要考虑的是：1. 在定义了一个该类的**实体对象**后，通过该实体对象可以访问类中哪些成员；2. 在该类基础上**派生**了一个类后，派生类中可以访问类中哪些成员。
>[C++类成员访问权限及继承方式](https://blog.csdn.net/CYH00_/article/details/110675807)  
>[C++类成员的访问权限以及类的封装](http://c.biancheng.net/view/2217.html)  
>C++通过 public、protected、private 三个关键字来控制成员变量和成员函数的访问权限，它们分别表示公有的、受保护的、私有的，被称为成员访问限定符。  

**在类的内部（定义类的代码内部）**：无论成员别声明为public、protected、private，都是可以互相访问的，没有访问权限的限制。  
**在类的外部（定义类的代码之外）**：只能通过对象访问成员，并且通过对象只能访问public属性的成员，不能访问protected、private属性的成员。
类的成员（**data member** and **function member**）访问权限分为三类：

- **public（公有型成员）***：这一类成员访问权限最为宽松，可以被**类成员函数、类外对象、派生类成员函数**访问；
- ***protected（保护型成员）***：这一类成员的访问权限相比public要严格一些，**不能**在类外通过**对象**进行访问，只**能**是**所在类的成员函数**和**派生类的成员函数**访问；
- ***private（私有型成员）***：这一类成员的访问权限最为严格，**只能**由**所在类的成员函数**访问；
  
>[C++的三种继承方式](http://c.biancheng.net/view/2269.html)  
>public、protected、private 三个关键字除了可以修饰类的成员，还可以指定继承方式。  
>**继承方式**限定了基类成员在派生类中的**访问权限**，继承方式共有三种：

- **public（公有继承）**：
  - 基类中所有 public 成员在派生类中为 public 属性；
  - 基类中所有 protected 成员在派生类中为 protected 属性；
  - 基类中所有 private 成员在派生类中不能使用，只有访问基类中的public成员函数间接访问基类中的private成员变量。
- **protected（保护继承）**：
  - 基类中的所有 public 成员在派生类中为 protected 属性；
  - 基类中的所有 protected 成员在派生类中为 protected 属性；
  - 基类中的所有 private 成员在派生类中不能使用。
- **private（私有继承）**：
  - 基类中的所有 public 成员在派生类中均为 private 属性；
  - 基类中的所有 protected 成员在派生类中均为 private 属性；
  - 基类中的所有 private 成员在派生类中不能使用。

1) 基类成员在派生类中的访问权限不得高于继承方式中指定的权限。例如，当继承方式为 protected 时，那么基类成员在派生类中的访问权限最高也为 protected，高于 protected 的会降级为 protected，但低于 protected 不会升级。再如，当继承方式为 public 时，那么基类成员在派生类中的访问权限将保持不变。也就是说，继承方式中的 public、protected、private 是用来指明基类成员在派生类中的最高访问权限的。
2) 不管继承方式如何，基类中的 private 成员在派生类中始终不能使用（不能在派生类的成员函数中访问或调用）。
3) 如果希望基类的成员能够被派生类继承并且毫无障碍地使用，那么这些成员只能声明为 public 或 protected；只有那些不希望在派生类中使用的成员才声明为 private。
4) 如果希望基类的成员既不向外暴露（不能通过对象访问），还能在派生类中使用，那么只能声明为 protected。
5) 注意，**我们这里说的是基类的 private 成员不能在派生类中使用，并没有说基类的 private 成员不能被继承。实际上，基类的 private 成员是能够被继承的，并且（成员变量）会占用派生类对象的内存，它只是在派生类中不可见，导致无法使用罢了**。private 成员的这种特性，能够很好的对派生类隐藏基类的实现，以体现面向对象的封装性。
6) 使用 using 关键字可以改变基类成员在派生类中的访问权限，例如将 public 改为 private、将 protected 改为 public。注意：using 只能改变基类中 public 和 protected 成员的访问权限，不能改变 private 成员的访问权限，因为基类中 private 成员在派生类中是不可见的，根本不能使用，所以基类中的 private 成员在派生类中无论如何都不能访问。

### C++基础：C++对象模型

>[C++对象模型](https://www.cnblogs.com/skynet/p/3343726.html)
