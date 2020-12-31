
### C++基础：static详解
参考：

- [C/C++ 中 static 的用法全局变量与局部变量](https://www.runoob.com/w3cnote/cpp-static-usage.html)
- [C++ static静态成员变量详解](http://c.biancheng.net/view/2227.html)
- [C++ static静态成员函数详解](http://c.biancheng.net/view/2228.html)

> 先看static的第一个作用：将static修饰的变量和函数**隐藏**（static全局变量）

在同时编译多个文件时，这些文件中的所有全局变量和全局函数都具有全局可见性。
举例来说明。同时编译两个源文件，一个是a.c，另一个是main.c。
首先在linux模式下使用```sudo gedit a.c```和```sudo gedit main.c```，分别将下面两段代码复制进去。

```C++
//a.c
#include <stdio.h>
char a = 'A'; // global variable
void msg()
{
     printf("Hello\n");
}
 
//main.c
#include <stdio.h>
int main()
{
     extern char a; // extern variable must be declared before use
     printf("%c ", a);
     (void)msg();
     return 0;
}
```

然后进行编译：```sudo gcc a.c main.c -o main```  
最后输入：```./main```  
得到的运行结果是：```A Hello```  
这表明a.c文件中的char a和void msg(){}在main.c是可见的，显然这是一种“**暴露**”。现在我们来看“**暴露**”的反面“**隐藏**”。  
如果我们在char a和void msg()前面均加上static修饰，如下：

```C++
//a.c
#include <stdio.h>
static char a = 'A'; // global variable
static void msg()
{
     printf("Hello\n");
}
 
//main.c
#include <stdio.h>
int main()
{
     extern char a; // extern variable must be declared before use
     printf("%c ", a);
     (void)msg();
     return 0;
}
```

然后进行编译：```sudo gcc a.c main.c -o main```  
执行上面这一行我们将得到一个错误如下：

```shell
In function `main':
main.c:(.text+0x7): undefined reference to `a'
main.c:(.text+0x25): undefined reference to `msg'
collect2: error: ld returned 1 exit status
```

这表明a.c文件中的static char a和static void msg(){}在main.c不可见了。对于a.c来说它将这两个变量隐藏了起来，使得其它源文件无法访问它们而只能在声明定义的源文件中被使用。  

总结下：所有未加static前缀的全局变量和函数都具有全局可见性，其它的源文件也能访问。此例中，char a是全局变量，msg是函数，并且都没有加static前缀，因此对于另外的源文件main.c是可见的。
如果加了static，就会对其它源文件隐藏。例如在a和msg的定义前加上static，main.c就看不到它们了。利用这一特性可以在不同的文件中定义同名函数和同名变量，而不必担心命名冲突。static可以用作函数和变量的前缀，对于函数来讲，static的作用仅限于隐藏。

> static的第二个作用：保持变量内容的持久（static局部变量）

static变量和全局变量都被存储在静态数据区，存储在静态数据区的变量会在程序刚开始运行时就完成初始化，也是唯一的一次初始化。和全局变量比起来，static可以控制变量的可见范围，说到底static还是隐藏起来了。上面我们讨论了将**全局变量**变为**static全局变量**后的情况，接下来我们看看将**局部变量**变为**static局部变量**后的情况。

如果作为static局部变量在函数内定义，它的生存期为整个源程序，但是其作用域仅在这个函数内，只能在定义该static局部变量的函数内使用该变量。退出该函数后，尽管该变量还继续存在，但不能使用它。考虑如下程序：

```C++
#include <stdio.h>
int fun(){
    static int count = 10; //在第一次进入这个函数的时候，变量a被初始化为10！并接着自减1，以后每次进入该函数，a 就不会被再次初始化了，仅进行自减1的操作；在static发明前，要达到同样的功能，则只能使用全局变量：
    return count--; 
}
 
int count = 1;
 
int main(void)
{
     printf("global\t\tlocal static\n");
     for(; count <= 10; ++count)
               printf("%d\t\t%d\n", count, fun());
     return 0;
}
```

输出的结果为：

```C++
global      local static
1           10
2           9
3           8
4           7
5           6
6           5
7           4
8           3
9           2
10          1
```

>基于以上两点可以得出一个结论：把局部变量改变为静态局部变量后是改变了它的存储方式即改变了它的生存期。把全局变量改变为静态全局变量后是改变了它的作用域，限制了它的使用范围。因此 static 这个说明符在不同的地方所起的作用是不同的。
> static的第三个作用是默认初始化为0（static变量）

其实全局变量也具备这一属性，因为全局变量也存储在静态数据区。在静态数据区，内存中所有的字节默认值都是0x00，某些时候这一特点可以减少程序员的工作量。比如初始化一个稀疏矩阵，我们可以一个一个地把所有元素都置0，然后把不是0的几个元素赋值。如果定义成静态的，就省去了一开始置0的操作。再比如要把一个字符数组当字符串来用，但又觉得每次在字符数组末尾加‘\0’;太麻烦。如果把字符串定义成静态的，就省去了这个麻烦，因为那里本来就是‘\0’;不妨做个小实验验证一下。

```C++
#include <stdio.h>
 
int a;
 
int main()
{
     int i;
     static char str[10];
     printf("integer: %d; string: (begin)%s(end)", a, str);
     return 0;
}
```

程序的运行结果是：```integer: 0; string: (begin) (end)```

> 先对上述static的三条作用做一句话总结：**首先static的最主要功能是隐藏，其次因为static变量存放在静态存储区，所以它具备持久性和默认值0。**  
> 接下来我们考虑class中的静态成员，包括**静态成员变量**和**静态成员函数**。  
> class中的静态成员变量（static data member）

对象的内存中包含了成员变量，同时不同的对象占用不同的内存空间，所以不同对象之间的成员变量互不影响。也就是说改变对象a的count不会影响对象b c d...的count。但是此时我们有另外一个需求，要求多个对象之间能够共享一个成员变量，对象a对该共享成员变量的改变，b c d...等对象都能够看见。这时我们只需要在声明自定义class时将该共享成员变量用static关键字修饰即可，这就是静态成员变量（static成员变量）。

在声明自定义class（自定义class的声明式）时，类中的static成员变量也是一个声明而非定义，而编译器会要求static成员函数需要一份初始化（条款2说如果是static常量成员，类型为整型，且没有取地址操作，有些编译器就可以不要一份初始化，但是显然现在的情况没有那么特殊），此时就必须在自定义class的声明之外对其中的static成员变量初始化，不论这个static成员变量是public、protected还是private。

由于static成员变量是所有对象共享的，所有它并不存在于为对象分配的内存中，而是在所有对象之外单独开辟一块内存，这时即使不创建对象也可以访问它，如何访问在后面说。现在来考虑类的static成员变量的实际内存分配的时机，static成员变量的内存既不是在声明类时分配，也不是在创建对象时分配，而是在类外定义时分配，这也是编译器如此重视static成员变量的定义式的原因。

```C++
class A//声明式，声明自定义class
{
public:
    static int a_public;
protected:
    static int a_protected;
private:
    static int a_private; //声明式，静态成员变量
}
int A::a_public = 10;    //声明式之外的定义式，此时分配实际内存
int A::a_protected = 10; //声明式之外的定义式，此时分配实际内存
int A::a_private = 10;   //声明式之外的定义式，此时分配实际内存
```

现在考虑static成员变量的的访问问题，static成员变量既可以通过对象来访问，也可以通过类作用域来访问，此时就要遵循 private、protected 和 public 关键字的访问权限限制了：

```C++
A::a_public = 10;    //通过类作用域访问 static 成员变量

A aa;
aa.a_public = 20;    //通过对象来访问 static 成员变量

A *bb = new A;
bb -> a_public = 20; //通过对象指针来访问 static 成员变量
```

针对static成员变量总结如下：

1) 一个类中可以有一个或多个静态成员变量，所有的对象都共享这些静态成员变量，都可以引用它。

2) static 成员变量和普通 static 变量一样，都在内存分区中的全局数据区分配内存，到程序结束时才释放。这就意味着，static 成员变量不随对象的创建而分配内存，也不随对象的销毁而释放内存。而普通成员变量在对象创建时分配内存，在对象销毁时释放内存。

3) 静态成员变量必须初始化，而且只能在类体外进行。例如：
int Student::m_total = 10;  
初始化时可以赋初值，也可以不赋值。如果不赋值，那么会被默认初始化为 0。全局数据区的变量都有默认的初始值 0，而动态数据区（堆区、栈区）变量的默认值是不确定的，一般认为是垃圾值。

4) 静态成员变量既可以通过对象名访问，也可以通过类名访问，但要遵循 private、protected 和 public 关键字的访问权限限制。当通过对象名访问时，对于不同的对象，访问的是同一份内存。

> class中的静态成员函数（static function member）

类中有了静态成员变量，引用前面的观点，我们实际上是拥有了一个作用域为该类的变量，它属于类的同时又与任何对象无关。我们同样设计了一类函数，它们的作用域在类中，同时又与任何对象无关，这就是静态成员函数。静态成员函数的出现理所当然的是操作那些属于类但与任何对象无关的静态成员变量。

下面我们来考虑编译器对普通成员函数和静态成员函数的处理：

- 编译器在编译一个普通成员函数的时候，会在参数列隐式的添加一个this指针形参，并将对象的地址赋值给this指针，所以普通成员函数只能通过对象来调用。

```C++
class A{
    void print(){};//编译器在编译的时候会给它添加一个this指针，此时声明式就是void print(A *this){};
}
A a = new A;
a.print();//编译器会将它转化为print(&a)，
```

- 而编译器在编译一个静态成员函数的时候，编译器不会在它的参数列中添加一个this指针形参，所以就算没有对象也可以通过类来调用静态成员函数。

```C++
class A{
    static void print(){};//编译器在编译的时候不会会给它添加一个this指针，此时声明式还是static void print(){};
}
A a = new A;
a.print();//编译器会将它转化为print()，
```

这就导致了如果我们在没有任何对象时调用了静态成员函数，同时这个静态成员函数中要访问普通成员函数或者普通成员变量，此时编译器就会报错，因为根本就没有一个对象的地址传给这些普通成员函数的形参this，同时对应的普通成员变量也不存在。但是我们的静态成员函数还是可以访问静态成员变量的，因为静态成员变量就算没有对象也是存在的。

所以静态成员函数与普通成员函数之间的区别是：

- 普通成员函数有this指针，可以访问类中的任意成员（普通成员变量、静态成员变量、静态成员函数）。而静态成员函数没有this指针，只能访问静态成员（静态成员变量、静态成员函数）。
- 普通成员函数与对象相关，必须通过对象引用。静态成员函数是作用域在类中但是与任何对象无关的一个普通函数，可以通过类作用域调用，也可以通过对象进行调用。
- 再次强调静态成员函数实际上就是个作用域在类中但是与任何对象无关的一个普通函数。所以，从最开始，我们将一个全局变量和一个全局函数的作用域慢慢收紧：将一个跨源文件的变量或函数限制在一个源文件中，将一个全局变量限制在一个函数中（变换下视角），将一个全局变量和全局函数限制在一个类中。
