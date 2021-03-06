
参考：

- [从编写源代码到程序在内存中运行的全过程解析](https://blog.csdn.net/kang___xi/article/details/79571137)
- 《计算机组成与设计 硬件/软件接口》第2章、附录A

`static` 修饰符可以将一个变量的使用范围（或者说作用域）进行限制，全局变量用 `static` 修饰使得其只能在本源文件中使用，局部变量用 `static` 修饰使得该变量只能在该函数作用域中使用，类成员用 `static` 修饰使得该成员只能在该类作用域中使用。这里就不禁想知道变量在内存中究竟是怎么存储的，什么时候存在什么时候不存在，究竟是什么样的量才叫变量？

为此找到了上面这篇博客，同时也参考了一些其他博客，现在记录如下。

以下内容包含：

- ELF文件类型
- 查看目标文件的 `objdump` 指令
- 编译、汇编、链接、加载过程。

学习的过程中也慢慢发现，完整的路线应该是：
C++源程序 -> 可执行目标文件 -> 进程 -> 内核调度执行。

### 一、基础概念

在学习的过程中，我们经常看见这一句话：“一个程序是由程序代码、程序数据和进程控制块组成”。程序代码我们能够明白，进程控制块也能明白，但是针对这个数据，感觉知道，但是真要说清楚好像也说不清楚。

#### 1. 什么是数据？

先来看一段代码：

```C
#include <stdio.h>
int gdata1 = 10;    //全局变量  初始化
int gdata2 = 0;     //全局变量  初始化为0
int gdata3;         //全局变量  未初始
 
static int gdata4 = 11;     //静态全局变量  初始化
static int gdata5 = 0;      //静态全局变量  初始化为0
static int gdata6;          //静态全局变量  未初始化
 
int main(void)
{
    int a = 12;         //局部变量  初始化
    int b = 0;          //局部变量  初始化为0
    int c;              //局部变量  未初始化
 
    static int d = 13;  //静态局部变量  初始化
    static int e = 0;   //静态局部变量  初始化为0
    static int f;       //静态局部变量  未初始化
    return 0;
}
```

这个代码段中除了类 `static` 成员没有列出来，其它两种情况均包含了，同时这里也做了另外一个维度的区分，也就是是否初始化。所以现在有两个维度：是否初始化、是否添加 `static` 修饰符。

这里主要搞清楚两件事情：

- 什么是数据
- 数据存放在哪里

> 1.什么是数据

**数据**指的是程序中定义的**全局变量**和**静态变量**，同时还有一种特殊的**常量**。所以上面的 `gdata1`、`gdata2`、`gdata3`、`gdata4`、`gdata5`、`gdata6`、`d`、`e` 和 `f` 均是数据。这里注意不包含**局部变量** `a`、`b`、`c`。尽管都有个 **“变量”** 后缀，但是不一定都可以称为是数据。

**所以最开始的疑惑应该改为：什么样的变量是数据，什么样的变量不是数据，以及一个变量是数据时它们存在哪里，一个变量不是数据时它们又存在哪里？**

> 2.数据存放在哪里

一个代码要想运行，必须先变成一个可执行目标文件，然后等待被加载到虚拟内存空间中，CPU调度后即可执行。所以细化一点：**数据存放在可执行目标文件中的哪里 以及 数据存放在虚拟内存空间中的哪个部分**。

先说**数据存放在可执行目标文件中的哪里**：数据存放的区域有三个地方：**`.data`** 节、**`.bss`** 节和 **`.rodata`** 节。等等，什么是节，这里只需要先知道它们存在于可重定位目标文件和可执行目标文件中就行。也就是程序的实体部分在可执行目标文件中就被分解为了 `节1、节2、节3、...`，没一个节描述程序的一个方面。

我们这里想知道的是数据如何放在这三个节中，以及如何区分。

- 对于**初始化不为0**的**全局变量**和**静态变量**存放在 **`.data`** 节，即 `gdata1`、`gdata4` 和 `d` 存放在 `.data` 节；
- 对于**未初始化或者初始化值为0**的数据存放在 **`.bss`** 节，而且不占目标文件的空间，即 `gdata2`、`gdata3`、`gdata5`、`gdata6`、`e` 和 `f`
文章下面有一张关于符号表的图，大家可以看到确实是这样的分布。
- **字符串常量**则存在 **`.rodata`** 节中，而且对于字符串而言它还有一个特殊的地方，就是它在内存中只有一份。如下代码：

  ```C++
  #include<stdio.h>
  int main(void)
  {
    const char *pStr1 = "hello,world";
    const char *pStr2 = "hello,world";
    printf("0x%x\n", pStr1);
    printf("0x%x\n", pStr2);
    return 0;
  }
  ```

  ```shell
  输出结果：
        0x89d6c004
        0x89d6c004
  运行结果是一样的，因为常量字符串“hello,world”在内存中只有一份
  ```

最后，在内核线程将用户程序加载进虚拟内存空间中时，我们的**数据存放在虚拟内存的哪里呢？**

参考最后面给出的解释，这里先不赘述。

#### 2. 什么是指令

刚才说了程序由程序代码、程序数据和进程控制块组成，那么指令是什么呢，它对应的就是程序代码。很简单，程序中除了数据，剩下的就是指令了。这里有一个很容易混淆的地方（**之前疑惑的核心就是它了**），如下代码：

```C
#include<stdio.h>
int main()
{
    int a = 10;
    int b = 20;
    printf("a+b=%d\n", a + b);
    return 0;
}
```

对于上面的代码，`a` 和 `b` 究竟**是数据还是指令**呢？可以看到它是一个**局部变量**，所以它应该是**数据**。嗯，**事实上它并不是数据**，它是一条指令或者说是一条代码。这个**指令的功能**是在函数栈帧上开辟 `2 x 4 = 8` 个字节，并在这 `8` 个字节中写入对应的值。

#### 3. 什么是符号

这里我们没有顺着继续说进程控制块，因为那是OS的课程讲的。这里讲完了数据和指令，我们要转向另外一个概念：**符号**。在我们编写完程序后，在**链接**时有时候会碰到这样的错误：

```C
错误  LNK1169  找到一个或多个多重定义的符号
```

即**符号重定义**，那**什么是符号**，**什么东西会产生符号**呢？

在程序中，所有的**数据**都会产生符号，而对于**代码段**只有**函数名**才会产生符号。而且**符号的作用域有global和local之分**，对于未用 `static` 修饰过的全局变量和函数产生的均是**global符号**，这样的变量和函数可以被其它文件所看见和引用；而使用过 `static` 修饰的变量和函数，它们的作用域仅局限在当前文件，不会被其它文件所看见，即其它文件无法引用**local符号**的变量和函数。

对于上面的“找到一个或多个多重定义的符号”的错误原因有可能是：**在多个文件中定义同一个全局变量或函数，即函数名和全局变量重名了。**

#### 4. 虚拟地址空间布局

对于32位操作系统，每个进程都有2^32字节的虚拟地址空间，即4G的虚拟地址空间。这4G的虚拟地址空间分为两个大部分：

- 每个进程独立的3G用户空间
- 所有进程共享的1G内核空间。

具体分布如下图：
![虚拟内存分布](./从C++源文件到ELF文件到可执行文件fig/虚拟内存分布.png)
一个小问题：为什么前128M不可访问，而不是256M？

以上4个点是一些基础概念，第1、2两点比较常见，第3点与ELF文件有关，第4点与操作系统：用户进程空间有关。

其实第2点就已经解答了我的疑惑了，但是也继续看下去。

### 二、 C语言的翻译层次

>参考
>
>- [Linux下C语言生成可执行文件的过程](https://www.cnblogs.com/jack-zou/p/9068247.html)

#### 1. 翻译层次

首先需要知道一个**源程序**从**编写完成**到**加载到内存中**要经过哪些**阶段**，以及每个阶段所使用的**程序**：

![C语言的翻译层次](./从C++源文件到ELF文件到可执行文件fig/C语言的翻译层次.png)

![C语言的翻译层次](./从C++源文件到ELF文件到可执行文件fig/源程序翻译过程1.png)

#### 2. 各层次分析

在 `Unix` 系统中，从源文件到可执行目标文件（=可执行文件）是由编译驱动程序完成的，如大名鼎鼎的 `gcc`，翻译过程包括图中的四个阶段，接下来分析这四个阶段:

- **预处理阶段**
  预处理器（ `cpp` ）根据以字符 `#` 开头的命令修给原始的C程序，结果得到另一个C程序，通常以 `.i` 作为文件扩展名。主要是进行文本替换、宏展开、删除注释这类简单工作。对应的命令：

  ```shell
  # 预处理
  # 在当前目录下会多出一个预处理结果文件main.i，打开main.i可以看到，
  # 在main.c的基础上吧stdio.h的内容插进去了。-o为指定文件名
  gcc -E main.c -o main.i 
  ```

- **编译阶段**：
  编译器将文本文件 `main.i` 翻译成 `main.s`，包含相应的汇编语言程序。对应的命令：

  ```shell
  # 编译为汇编代码
  # 其中-S参数是在编译完成后退出，-o为指定文件名
  gcc -S main.c -o main.s 
  ```

接下来的汇编阶段和链接阶段都会生成**目标文件**，但是生成的**目标文件具有不同的类型**。所以我们可以把焦点放在**目标文件的区别**上，以此来完成对这两个阶段的分析。首先先看下目标文件。

- ***目标文件***：
  - **目标文件类型**
    一共有三类目标文件：
    **1. 可重定位目标文件**：包含**二进制**代码和数据，其形式可以和其他目标文件进行合并，创建一个**可执行目标文件**
    **2. 可执行目标文件**：包含**二进制**代码和数据，可直接被加载器加载执行
    **3. 共享目标文件**：可被动态的加载和链接（这里暂不讨论）
  - **两个阶段生成的目标文件类型**
    由**汇编器生成的就是可重定位目标文件**，经过**链接器作用后才生成可执行目标文件**，链接器的作用就是以一组可重定位目标文件作为输入，生成可加载和运行的可执行目标文件。
  - **目标文件构成**
    在UNIX系统中的目标文件（可重定位目标文件、可执行目标文件）通常包含以下6个不同的部分（按段来分的，不是按节来分，其实知道一个段包含多个节区这点就行了）：
    - **目标文件头**，描述目标文件其它部分的大小和位置
    - **代码段**，包含机器语言代码
    - **静态数据段**，包含在程序生命周期内分配的数据
    - **重定位信息**，标记了一些在程序加载进内存时依赖于绝对地址的指令和数据。如果程序中这些部分在内存中被移动，这些引用必须改变。
    - **符号表**，包含未定义的剩余标记，如外部引用。
    - **调试信息**，包含一份说明目标模块如何编译的简明描述，这样，调试器能够将机器指令关联到C源文件，并使数据结构变得可读。

- **汇编阶段**
  系统程序汇编器将 `.s` 文件翻译成机器语言指令，即二进制机器代码，并把这些指令打包成 **可重定位目标文件** 的格式，将结果保存在目标文件 `.o` 中。**注意这个可重定位目标文件中包含有未解决的引用**。对应的命令：

  ```shell
  # 汇编为可重定位目标文件
  # .o就是可重定位目标文件，可重定位目标文件与可执行文件类似，
  # 都是机器能够识别的可执行代码，但是由于还没有链接，结构会稍有不同。
  gcc -c main.c -o main.o
  ```

- **链接阶段**
  系统程序链接器将所有独立汇编的机器语言程序（可重定位目标文件）拼接在一起，它使用每个模块中的重定位信息和符号表，来解析所有未定义标签（这种标签引用会发生在分支指令、跳转指令和数据寻址处）。如果所有外部引用都解析完了，链接器决定每个模块将要占用的内存位置，此时重定位所有的绝对引用以反映它们真实的地址，最后生成可执行目标文件。
  举例如：此时 `main` 程序**如果**调用了其它函数库中的 `printf` 函数。`printf` 函数存在于一个名为 `printf.o` 的单独的预编译目标文件中。 **链接器**（`ld`）就负责处理，把这个文件并入到 `main.o` 程序中，结果得到 `main` 文件，一个**可执行目标文件**。
  注：函数库一般分为**静态库**和**动态库**两种。**静态库**是指**编译链接时**，把库文件的代码全部加入到可执行目标文件中，因此生成的文件比较大，但在运行时也就不再需要库文件了。其后缀名一般为 `.a`。**动态库**与之相反，在**编译链接时**并没有把库文件的代码加入到可执行目标文件中，而是在程序执行时由运行时链接文件加载库，这样可以节省系统的开销。动态库一般后缀名为 `.so`，`gcc` 在编译时默认使用动态库。对应的指令：

  ```shell
  # 链接并生成可执行文件
  gcc main.o -o main
  ```

- **加载阶段**
  操作系统需要将位于磁盘中的可执行目标文件加载到内存中并启动执行它，在UNIX系统中，加载器按照如下步骤工作：
  - 读取可执行目标文件头来确定代码段和数据段的大小。
  - 为正文和数据创建一个足够大的地址空间。
  - 将可执行目标文件中的指令和数据复制到内存中。
  - 把主程序的参数（如果存在）复制到栈顶。
  - 初始化机器寄存器，将栈指针指向第一个空位置。
  - 跳转到启动例程，它将参数复制到参数寄存器并且调用程序的main函数。当 `main` 函数返回时，启动例程通过系统调用 `exit` 终止程序。

在Linux下上面四步可以直接用下面的命令完成：

```shell
gcc main.c -o main
```

第二小节给出了一个大致的C语言翻译流程，接下来结合之前的实际代码来看看各阶段生成文件中的具体内容。

### 三、 编译过程

这里我们的编译过程包括：预编译阶段、编译阶段、汇编阶段。所以编译结束后的文件是一个**可重定位目标文件**，linux下是 `*.o` 文件，windows中是 `*.obj` 文件。经过后续的链接器作用才会得到一个**可执行目标文件**。

#### 1. 编译命令

如上所述，整个编译将分为四个步骤：

- 首先编写源文件 `main.c/main.cpp`；
- 编写好代码以后进行预编译成 `main.i` 文件，预编译过程中去掉注释、进行宏替换、增加行号信息等；
- 然后将 `main.i` 文件经过语法分析、代码优化和汇总符号等步骤后，编译形成 `main.S` 的汇编文件，里面存放的都是汇编代码；
- 最后一个编译步骤是进行汇编，从 `main.S` 变成二进制可重定位目标文件 `main.o`。

以上四个步骤对应的在linux下的命令为：

```shell
gcc -E main.c -o main.i  #预编译，生成main.i文件
gcc -S main.i            #编译，生成main.S文件
gcc -c main.S            #汇编，生成main.o文件
```

#### 2. 二进制目标文件的结构和布局

>参考：
>
>- [ELF文件详解—初步认识](https://blog.csdn.net/daide2012/article/details/73065204)
>- [Linux[ELF]: ELF文件结构简单梳理](https://www.jianshu.com/p/dd5aec5826da)
>- [使用readelf和objdump解析目标文件。](https://www.jianshu.com/p/863b279c941e)
>- [objdump命令](https://man.linuxde.net/objdump)
>- [readelf命令](https://man.linuxde.net/readelf)
>- [readelf 和 objdump 例子详解及区别](https://blog.csdn.net/lqy971966/article/details/106905237?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-2&spm=1001.2101.3001.4242)

首先给出一个二进制目标文件的总体布局，如果是汇编器输出的可重定位目标文件，则可能没有 `program header`（里面有多个元素，每个元素是一个结构体，这个结构体描述了一个具体节的相关属性），编译器输出的可执行目标文件`program header` 和 `section header`（里面有多个元素，每个元素是一个结构体，这个结构体描述了一个具体节的相关属性） 均有：

![二进制可重定位目标文件](./从C++源文件到ELF文件到可执行文件fig/ELF文件简介.png)

上图中的节类型和段类型网上资料有，这里就不详细列出来了。

前文中提到从链接器的角度来看ELF文件（下图中的链接视图），另外一个角度就是从加载器的角度看ELF文件（下图中的执行视图）。**注意是先有这些东西，链接器和加载器看的时候才有可以看的东西**。汇编器生成了可重定位文件，这个文件里面已经有节区的存在，链接器看的时候就是链接视图，然后链接器处理完成生成 `program header`，加载器看的时候就是执行视图了。

![二进制可重定位目标文件](./从C++源文件到ELF文件到可执行文件fig/ELF文件简介3.png)

>注：那篇博客中称为段segment是不正确的，ELF文件只有一个，它会被链接器和加载器顺序处理：
>
>- 链接器在处理它的时候通过 `section header table` 中的每一个节描述结构体来认识这个ELF文件。`section header table` 由汇编器生成。链接器处理操作之一就是将读写等属性相同的 `section` 合并为 `segment` ，这种合并的体现是生成对应的 `program header table`，而不是抹掉 `section` 并建立 `segment`，所以此时 `section header table` 和 `program header table` 并存。
>- 加载器在处理它的时候通过 `program header table` 中的每一个段描述结构体来认识这个ELF文件。`program header table` 由链接器生成，此时 `section header table` 虽然存在但是不会用上。

#### 3. 使用Linux命令查看可重定位文件内容

这里主要使用到了两个Linux命令，具体的命令选项这里就不给出，边运行边解释：

```shell
readelf 显示elf文件的信息，并不提供反汇编功能，可以显示调试信息
objdump 显示目标文件的信息，提供反汇编功能，不能显示调试信息
```

##### 1) **查看ELF header**

首先查看可重定位目标文件 `main.o` 的 `ELF header`，在Linux下输入如下命令：

```shell
# readelf用来显示一个或多个elf格式的目标文件的信息
# -h 
# --file-header 显示elf文件开始的文件头信息.
readelf -h main.o
```

得到输出：

```shell
moocos-> readelf -h main.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          288 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         11
  Section header string table index: 8
```

可以看到ELF header占64个字节，里面存放着文件类型、支持的平台、程序入口点地址等信息，如果你对每个字段的具体含义感兴趣，可以看《程序员自我修养》

##### 2) **查看ELF的section header**
  
使用如下命令：

```shell
# -S 
# --section-headers
# --sections 显示节头信息(如果有的话)。
readelf -S main.o

# .text是代码
# .data、.bss是数据
# .comment是注释
# .symtab是符号表。
```

得到输出：

```shell
moocos-> readelf -S main.o
There are 11 section headers, starting at offset 0x120:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000019  0000000000000000  AX       0     0     1
  [ 2] .data             PROGBITS         0000000000000000  0000005c
       000000000000000c  0000000000000000  WA       0     0     4
  [ 3] .bss              NOBITS           0000000000000000  00000068
       0000000000000014  0000000000000000  WA       0     0     4
  [ 4] .comment          PROGBITS         0000000000000000  00000068
       0000000000000025  0000000000000001  MS       0     0     1
  [ 5] .note.GNU-stack   PROGBITS         0000000000000000  0000008d
       0000000000000000  0000000000000000           0     0     1
  [ 6] .eh_frame         PROGBITS         0000000000000000  00000090
       0000000000000038  0000000000000000   A       0     0     8
  [ 7] .rela.eh_frame    RELA             0000000000000000  000005e0
       0000000000000018  0000000000000018           9     6     8
  [ 8] .shstrtab         STRTAB           0000000000000000  000000c8
       0000000000000054  0000000000000000           0     0     1
  [ 9] .symtab           SYMTAB           0000000000000000  000003e0
       00000000000001b0  0000000000000018          10    14     8
  [10] .strtab           STRTAB           0000000000000000  00000590
       000000000000004c  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
```

可以看到源代码 `.c` 中的数据和指令在目标文件中是按节的形式组织起来的，而且 `.text` 节的起始位置从 `offset` 字段可以看到是 `0x40` 位置，即64字节处，也说明 `.text` 节是接在ELF header后面。
  
`.text` 代码节的大小为0x19，起始偏移为0x40，所以 `.data` 节的起始偏移应该为0x19+0x40=0x59，但是为了字节对齐，所以。`.data` 节的起始地址为0x5c，也即图中 `offset` 字段所示，后面的节以此类推。
  
之后的 `.bss` 节会出现两个问题，一个是 `.bss` 节的大小应该为4*6=24字节（对于未初始化或者初始化值为0的数据存放在 `.bss` 节，而且不占目标文件的空间，即 `gdata2、gdata3、gdata5、gdata6、e和f`），但是实际上却是20字节；另一个问题就是可以看到 `.comment` 节的偏移(offset)也为0x68，这说明 `.bss` 节在目标文件中是不占大小的，即 `.comment` 和 `.bss` 的偏移相同。对于这两个问题，简单说一下。第一个问题，涉及到C语言中的强符号和弱符号概念；第二个问题我们可以这样理解，因为 `.bss` 节中存的是初始化为0或者未初始化的数据，而实际未初始化的数据其默认值也为0，这样我们就没必要存它们的初始值，相当于有一个默认值0。

当然也可以使用如下的命令：

```shell
# -h 
# --section-headers 
# --headers 
# 显示目标文件各个section的头部摘要信息，使用readelf可以查看完整信息
# 注意objdump无法查看 ELF 文件的header
moocos-> objdump -h main.o 。

main.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000019  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         0000000c  0000000000000000  0000000000000000  0000005c  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000014  0000000000000000  0000000000000000  00000068  2**2
                  ALLOC
  3 .comment      00000025  0000000000000000  0000000000000000  00000068  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  0000008d  2**0
                  CONTENTS, READONLY
  5 .eh_frame     00000038  0000000000000000  0000000000000000  00000090  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
```

如果我们想输出节的详细内容，我们可以使用如下命令：

```shell
# -s 
# --full-contents 
# 显示指定section的完整内容，默认所有的非空section都会被显示。
moocos-> objdump -s main.o

main.o:     file format elf64-x86-64

Contents of section .text:
 0000 554889e5 c745f80c 000000c7 45fc0000  UH...E......E...
 0010 0000b800 0000005d c3                 .......].       
Contents of section .data:
 0000 0a000000 0b000000 0d000000           ............    
Contents of section .comment:
 0000 00474343 3a202855 62756e74 7520342e  .GCC: (Ubuntu 4.
 0010 382e322d 31397562 756e7475 31292034  8.2-19ubuntu1) 4
 0020 2e382e32 00                          .8.2.           
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 19000000 00410e10 8602430d  .........A....C.
 0030 06540c07 08000000                    .T......        
```

指定输出 `.text` 中的内容并查看反汇编结果：

```shell
moocos-> objdump --section=.text -s main.o

main.o:     file format elf64-x86-64

Contents of section .text:
 0000 554889e5 c745f80c 000000c7 45fc0000  UH...E......E...
 0010 0000b800 0000005d c3                 .......].       

# 使用-d命令选项反汇编.text中的二进制代码
moocos-> objdump -d main.o

main.o:     file format elf64-x86-64

Disassembly of section .text:

0000000000000000 <main>:
   0: 55                    push   %rbp
   1: 48 89 e5              mov    %rsp,%rbp
   4: c7 45 f8 0c 00 00 00  movl   $0xc,-0x8(%rbp)
   b: c7 45 fc 00 00 00 00  movl   $0x0,-0x4(%rbp)
  12: b8 00 00 00 00        mov    $0x0,%eax
  17: 5d                    pop    %rbp
  18: c3                    retq 
```

##### 3) **查看ELF的segment header**

使用如下指令：

```shell
# -l
# --program-headers
# --segments 显示程序头（段头）信息(如果有的话)。
readelf -l main.o
```

输出结果如下：

```shell
moocos-> readelf -l main.o

There are no program headers in this file.
```

显示没有 `program header`，因为由汇编器生成得到的可重定位目标文件中只有节区，而没有段，段是在链接器处理之后才会生成的。

`objdump` 没有查看 `segment header` 的命令选项，但是你可以使用 `objdump -x` 来查看所有的头部信息

##### 4) **符号表信息**

以上就是可重定位目标文件的组成，下面再介绍一下上面提到的**符号表**如下图，第一列是符号的地址，由于编译的时候不分配地址，所以放的是零地址或者偏移量；第二列是符号的作用域( `g` 代表 `global`，`l` 代表 `local`)，前面讨论了用 `static` 修饰过的符号均是 `local` 的（不明白的搜一下 `static` 关键字的作用），如下图中 `gdata4/gdata5/gdata6` 等；第三列表示符号位于哪个节，在这里也能看到 `gdata1、gdata4` 和 `d` 都存放在 `.data` 节中，初始化为0或未初始化的 `gdata2/gdata5/gdata6` 等都存放在 `.bss` 节：

```shell
# -t 
#  --syms 显示文件的符号表入口。类似于nm -s提供的信息
moocos-> objdump -t main.o

main.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 main.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l    d  .data  0000000000000000 .data
0000000000000000 l    d  .bss   0000000000000000 .bss
0000000000000004 l     O .data  0000000000000004 gdata4
0000000000000004 l     O .bss   0000000000000004 gdata5
0000000000000008 l     O .bss   0000000000000004 gdata6
000000000000000c l     O .bss   0000000000000004 f.2190
0000000000000010 l     O .bss   0000000000000004 e.2189
0000000000000008 l     O .data  0000000000000004 d.2188
0000000000000000 l    d  .note.GNU-stack  0000000000000000 .note.GNU-stack
0000000000000000 l    d  .eh_frame  0000000000000000 .eh_frame
0000000000000000 l    d  .comment 0000000000000000 .comment
0000000000000000 g     O .data  0000000000000004 gdata1
0000000000000000 g     O .bss 0000000000000004 gdata2
0000000000000004       O *COM*  0000000000000004 gdata3
0000000000000000 g     F .text  0000000000000019 main
```

```shell
# -s
#  --syms        
#  --symbols 显示符号表段中的项（如果有的话）。
moocos-> readelf -s main.o

Symbol table '.symtab' contains 18 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    2 
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 
     5: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    2 gdata4
     6: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    3 gdata5
     7: 0000000000000008     4 OBJECT  LOCAL  DEFAULT    3 gdata6
     8: 000000000000000c     4 OBJECT  LOCAL  DEFAULT    3 f.2190
     9: 0000000000000010     4 OBJECT  LOCAL  DEFAULT    3 e.2189
    10: 0000000000000008     4 OBJECT  LOCAL  DEFAULT    2 d.2188
    11: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 
    12: 0000000000000000     0 SECTION LOCAL  DEFAULT    6 
    13: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 
    14: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    2 gdata1
    15: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 gdata2
    16: 0000000000000004     4 OBJECT  GLOBAL DEFAULT  COM gdata3
    17: 0000000000000000    25 FUNC    GLOBAL DEFAULT    1 main
```

这里特别说一下 `gdata3`，按上面的分析来说它应该是存放在 `.bss` 节，但是我们可以看到它是*COM*，原因在于它是一个弱符号，在编译时无法确定有没有强符号会覆盖它。

##### 5) 以上三个header可以使用以下命令一并输出

```shell
# -a
# --all 显示全部信息
readelf -a main.o

# -e
# --headers 显示全部头信息
readelf -e main.o

# --all-headers 
# -x 
# 显示所可用的头信息，包括符号表、重定位入口。-x 等价于-a -f -h -r -t 同时指定。 
objdump -x main.o
```

### 四、链接过程

汇编器生成**可重定位目标文件**，链接器生成**可执行目标文件**。我们依旧可以使用第三部分的命令来查看可执行目标文件的相关部分，这里就不再展示，主要考虑地址信息。

链接过程分为两步：

- 第一步是合并所有目标文件的节生成段，并调整段偏移和段长度，合并符号表，分配内存地址；
- 第二步是链接的核心，进行符号的重定位。

##### 1) **合并节**

所有相同属性的节进行合并，组织在一个页面上，这样更节省空间。如 `.text` 节的权限是可读可执行，`.rodata` 节也是可读可执行，所以将两者合并组织在一个页面上；同理合并 `.data` 节和 `.bss` 节。

##### 2) **合并符号表**

链接阶段只处理所有 `obj` 文件的 `global` 符号，`local` 符号不作任何处理。

##### 3) **符号解析**

符号解析指的是所有引用符号的地方都要找到符号定义的地方。

##### 4) **分配内存地址**

在编译过程中不分配地址（给的是零地址和偏移），直到符号解析完成以后才分配地址，这里我们可以使用 `objdump` 中的 `-d` 选项分别查看 `main.o` 和 `main` 两个目标文件中的 `.text` 部分，观察其中的地址信息部分，如下：

```shell
# -d
# --disassemble 
# 从objfile中反汇编那些特定指令机器码的section。

# -D 
# --disassemble-all 
# 与 -d 类似，但反汇编所有section.

moocos-> objdump -d main.o

main.o:     file format elf64-x86-64

Disassembly of section .text:

0000000000000000 <main>: # 地址为0，未分配地址
   0: 55                    push   %rbp
   1: 48 89 e5              mov    %rsp,%rbp
   4: c7 45 f8 0c 00 00 00  movl   $0xc,-0x8(%rbp)
   b: c7 45 fc 00 00 00 00  movl   $0x0,-0x4(%rbp)
  12: b8 00 00 00 00        mov    $0x0,%eax
  17: 5d                    pop    %rbp
  18: c3                    retq 
-----------------------------------------------------------
moocos-> objdump -d main

main.o:     file format elf64-x86-64

Disassembly of section .text:

00000000004004ed <main>: # 给出实际地址
  4004ed: 55                    push   %rbp
  4004ee: 48 89 e5              mov    %rsp,%rbp
  4004f1: c7 45 f8 0c 00 00 00  movl   $0xc,-0x8(%rbp)
  4004f8: c7 45 fc 00 00 00 00  movl   $0x0,-0x4(%rbp)
  4004ff: b8 00 00 00 00        mov    $0x0,%eax
  400504: 5d                    pop    %rbp
  400505: c3                    retq   
  400506: 66 2e 0f 1f 84 00 00  nopw   %cs:0x0(%rax,%rax,1)
  40050d: 00 00 00 
```

##### 5) 符号重定位

因为在编译过程中不分配地址，所以在目标文件所有数据出现的地方都给的是零地址，所有函数调用的地方给的是相对于下一条指令的地址的偏移量。在符号重定位时，要把分配的地址回填到数据和函数调用出现的地方，而且对于数据而言填的是绝对地址，而对函数调用而言填的是偏移量。

```shell
# -t 
# --syms 
# 显示文件的符号表入口。类似于nm -s提供的信息
# 这里也可以使用readelf -s 来查看
moocos-> objdump -t main.o # 汇编器得到的可重定位目标文件

main.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 main.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l    d  .data  0000000000000000 .data
0000000000000000 l    d  .bss 0000000000000000 .bss
0000000000000004 l     O .data  0000000000000004 gdata4
0000000000000004 l     O .bss 0000000000000004 gdata5
0000000000000008 l     O .bss 0000000000000004 gdata6
000000000000000c l     O .bss 0000000000000004 f.2190
0000000000000010 l     O .bss 0000000000000004 e.2189
0000000000000008 l     O .data  0000000000000004 d.2188
0000000000000000 l    d  .note.GNU-stack  0000000000000000 .note.GNU-stack
0000000000000000 l    d  .eh_frame  0000000000000000 .eh_frame
0000000000000000 l    d  .comment 0000000000000000 .comment
0000000000000000 g     O .data  0000000000000004 gdata1
0000000000000000 g     O .bss 0000000000000004 gdata2
0000000000000004       O *COM*  0000000000000004 gdata3
0000000000000000 g     F .text  0000000000000019 main

--------------------------------------------------------------
moocos-> objdump -t main # 链接器得到的可执行目标文件

main:     file format elf64-x86-64

SYMBOL TABLE:
0000000000400238 l    d  .interp  0000000000000000              .interp
0000000000400254 l    d  .note.ABI-tag  0000000000000000              .note.ABI-tag
0000000000400274 l    d  .note.gnu.build-id 0000000000000000              .note.gnu.build-id
0000000000400298 l    d  .gnu.hash  0000000000000000              .gnu.hash
00000000004002b8 l    d  .dynsym  0000000000000000              .dynsym
0000000000400300 l    d  .dynstr  0000000000000000              .dynstr
0000000000400338 l    d  .gnu.version 0000000000000000              .gnu.version
0000000000400340 l    d  .gnu.version_r   0000000000000000              .gnu.version_r
0000000000400360 l    d  .rela.dyn  0000000000000000              .rela.dyn
0000000000400378 l    d  .rela.plt  0000000000000000              .rela.plt
00000000004003a8 l    d  .init  0000000000000000              .init
00000000004003d0 l    d  .plt 0000000000000000              .plt
0000000000400400 l    d  .text  0000000000000000              .text
0000000000400584 l    d  .fini  0000000000000000              .fini
0000000000400590 l    d  .rodata  0000000000000000              .rodata
0000000000400594 l    d  .eh_frame_hdr  0000000000000000              .eh_frame_hdr
00000000004005c8 l    d  .eh_frame  0000000000000000              .eh_frame
0000000000600e10 l    d  .init_array  0000000000000000              .init_array
0000000000600e18 l    d  .fini_array  0000000000000000              .fini_array
0000000000600e20 l    d  .jcr 0000000000000000              .jcr
0000000000600e28 l    d  .dynamic 0000000000000000              .dynamic
0000000000600ff8 l    d  .got 0000000000000000              .got
0000000000601000 l    d  .got.plt 0000000000000000              .got.plt
0000000000601028 l    d  .data  0000000000000000              .data
0000000000601044 l    d  .bss 0000000000000000              .bss
0000000000000000 l    d  .comment 0000000000000000              .comment
0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
0000000000600e20 l     O .jcr 0000000000000000              __JCR_LIST__
0000000000400430 l     F .text  0000000000000000              deregister_tm_clones
0000000000400460 l     F .text  0000000000000000              register_tm_clones
00000000004004a0 l     F .text  0000000000000000              __do_global_dtors_aux
0000000000601044 l     O .bss 0000000000000001              completed.6972
0000000000600e18 l     O .fini_array  0000000000000000              __do_global_dtors_aux_fini_array_entry
00000000004004c0 l     F .text  0000000000000000              frame_dummy
0000000000600e10 l     O .init_array  0000000000000000              __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*  0000000000000000              main.c
000000000060103c l     O .data  0000000000000004              gdata4
000000000060104c l     O .bss 0000000000000004              gdata5
0000000000601050 l     O .bss 0000000000000004              gdata6
0000000000601054 l     O .bss 0000000000000004              f.2190
0000000000601058 l     O .bss 0000000000000004              e.2189
0000000000601040 l     O .data  0000000000000004              d.2188
0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
00000000004006b8 l     O .eh_frame  0000000000000000              __FRAME_END__
0000000000600e20 l     O .jcr 0000000000000000              __JCR_END__
0000000000000000 l    df *ABS*  0000000000000000              
0000000000600e18 l       .init_array  0000000000000000              __init_array_end
0000000000600e28 l     O .dynamic 0000000000000000              _DYNAMIC
0000000000600e10 l       .init_array  0000000000000000              __init_array_start
0000000000601000 l     O .got.plt 0000000000000000              _GLOBAL_OFFSET_TABLE_
0000000000400580 g     F .text  0000000000000002              __libc_csu_fini
0000000000000000  w      *UND*  0000000000000000              _ITM_deregisterTMCloneTable
0000000000601028  w      .data  0000000000000000              data_start
0000000000601044 g       .data  0000000000000000              _edata
0000000000400584 g     F .fini  0000000000000000              _fini
0000000000000000       F *UND*  0000000000000000              __libc_start_main@@GLIBC_2.2.5
0000000000601038 g     O .data  0000000000000004              gdata1
0000000000601028 g       .data  0000000000000000              __data_start
000000000060105c g     O .bss 0000000000000004              gdata3
0000000000000000  w      *UND*  0000000000000000              __gmon_start__
0000000000601030 g     O .data  0000000000000000              .hidden __dso_handle
0000000000400590 g     O .rodata  0000000000000004              _IO_stdin_used
0000000000400510 g     F .text  0000000000000065              __libc_csu_init
0000000000601060 g       .bss 0000000000000000              _end
0000000000400400 g     F .text  0000000000000000              _start
0000000000601044 g       .bss 0000000000000000              __bss_start
00000000004004ed g     F .text  0000000000000019              main
0000000000000000  w      *UND*  0000000000000000              _Jv_RegisterClasses
0000000000601048 g     O .data  0000000000000000              .hidden __TMC_END__
0000000000000000  w      *UND*  0000000000000000              _ITM_registerTMCloneTable
0000000000601048 g     O .bss 0000000000000004              gdata2
00000000004003a8 g     F .init  0000000000000000              _init

```

从上图中我们可以看到 `gdata1` 等变量的地址不再是0，而是0x0601038，正确回填了绝对地址。

### 五、 可执行程序

链接完成以后形成了可执行文件，下面来解析可执行文件是如何执行起来的。同样，首先给出可执行文件的总体布局，然后再来深入解析。
![可执行文件布局](./从C++源文件到ELF文件到可执行文件fig/可执行文件布局.png)

#### 1. 可执行文件程序入口点

首先看一下可执行文件的头部，如下图，里面记录的程序入口地址为 `0x400400`。看下它怎么得到的：

- 第一步：找到 `.text` 在整个ELF文件中的偏移量
  `.text` 是程序的一个节，所以我们可以在 `section header table` 找到关于 `.text` 的描述，我们使用 `readelf -S main` 可以看见 `.text` 的偏移是400，
- 第二步：我们找到ELF文件的第一个字节加载到虚拟内存的何处
  由于加载器在加载ELF文件的时候，是以执行视图来看待ELF文件的，此时它看到的是段，我们可以通过 `program header table` 来找到ELF文件的第一个字节被加载到虚拟内存的何处。使用 `readelf -l main`，观察第一个 `LOAD` 段，它的起始虚拟地址是0x400000，所以ELF的第一个字节被加载到了0x400000处。
  
结合上面两点，`.text` 的虚拟地址就是0x400400，`.text` 是我们程序开始执行的起始点，所以头文件中的入口点地址是0x400400.

再观察 `.text` 中的内容，我们使用 `objdump -d main` 可以看到0x400400处是 `_start` 函数，也就是该可执行文件的启动函数，而我们实际编写的程序 `main` 的起始地址是 `0x4004ed`。

至于博客里面讲的：程序的入口地址 = ELF第一字节的虚拟地址+ `program header table` 的大小。这种说法是错误的。可以通过 `readelf -S main` 观察节区的分布情况，在 `program header table` 和 `.text` 之间还有许多其它的节区。

```shell
moocos-> readelf -h main
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x400400
  Start of program headers:          64 (bytes into file)
  Start of section headers:          4464 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         30
  Section header string table index: 27

```

#### 2. LOAD段分析

再来看看 `program headers` 中的两个LOAD段描述符，第一个load项的属性是可读可执行，其实存放的就是代码段；第二个load项的属性是可读可写，其实存放的就是数据段。这两个load项的意义在于它指示了哪些段会被加载到同一个页面中，可以看到这两个load项的对齐方式是页面对齐（32位linux操作系统页面大小为4K）。

```shell
moocos-> readelf -l main

Elf file type is EXEC (Executable file)
Entry point 0x400400
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000400040 0x0000000000400040
                 0x00000000000001f8 0x00000000000001f8  R E    8
  INTERP         0x0000000000000238 0x0000000000400238 0x0000000000400238
                 0x000000000000001c 0x000000000000001c  R      1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000400000 0x0000000000400000
                 0x00000000000006bc 0x00000000000006bc  R E    200000
  LOAD           0x0000000000000e10 0x0000000000600e10 0x0000000000600e10
                 0x0000000000000234 0x0000000000000250  RW     200000
  DYNAMIC        0x0000000000000e28 0x0000000000600e28 0x0000000000600e28
                 0x00000000000001d0 0x00000000000001d0  RW     8
  NOTE           0x0000000000000254 0x0000000000400254 0x0000000000400254
                 0x0000000000000044 0x0000000000000044  R      4
  GNU_EH_FRAME   0x0000000000000594 0x0000000000400594 0x0000000000400594
                 0x0000000000000034 0x0000000000000034  R      4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     10
  GNU_RELRO      0x0000000000000e10 0x0000000000600e10 0x0000000000600e10
                 0x00000000000001f0 0x00000000000001f0  R      1

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .interp 
   02     .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .text .fini .rodata .eh_frame_hdr .eh_frame 
   03     .init_array .fini_array .jcr .dynamic .got .got.plt .data .bss 
   04     .dynamic 
   05     .note.ABI-tag .note.gnu.build-id 
   06     .eh_frame_hdr 
   07     
   08     .init_array .fini_array .jcr .dynamic .got 
```

综上，当双击一个可执行程序时，首先解析其文件头部 `ELF header` 获取 `entry point address` 程序入口点地址，然后按照两个 `load` 项的指示将相应的段通过 `mmap()` 函数映射到虚拟页面中（虚拟页面存在于虚拟地址空间中），最后再通过多级页表映射将虚拟页面（VP）映射到物理页面（PP）中。该映射分为三步，1.首先是创建虚拟地址到物理内存的映射（创建内核地址映射结构体），创建页目录和页表；2. 再就是加载代码段和数据段；3.把可执行文件的入口地址写到CPU的PC寄存器中。

具体的加载方式以及虚拟页物理页映射参考操作系统。
