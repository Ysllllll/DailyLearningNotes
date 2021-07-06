```bash
sudo apt-get install xxx
sudo apt-get remove xxx
sudo apt-cache search xxx
sudo apt-cache show/showpkg xxx
```

```bash
sudo apt-get install gcc-doc cpp-doc glibc-doc
```

```bash
sleep 60 & # == sleep 60;

#[ctrl + z]
bg
fg
```

```bash
sort < file5.txt
```

```bash
sudo mkdir -p /var/run/mysqld
sudo chown mysql /var/run/mysqld/
sudo service mysql restart
```

```bash
ps -a
```

```bash
sudo apt-get install build-essential
```

```bash
gcc -Wall -g hello.c -o hello
```

```bash
    * 寄存器命名原则
        AT&T: %eax                      Intel: eax
    * 源/目的操作数顺序
        AT&T: movl %eax, %ebx           Intel: mov ebx, eax
    * 常数/立即数的格式　
        AT&T: movl $_value, %ebx        Intel: mov eax, _value
      把value的地址放入eax寄存器
        AT&T: movl $0xd00d, %ebx        Intel: mov ebx, 0xd00d
    * 操作数长度标识
        AT&T: movw %ax, %bx             Intel: mov bx, ax
    * 寻址方式
        AT&T:   immed32(basepointer, indexpointer, indexscale)
        Intel:  [basepointer + indexpointer × indexscale + imm32)

    * 寻找example
    * 直接寻址
            AT&T:  foo                         Intel: [foo]
            boo是一个全局变量。注意加上$是表示地址引用，不加是表示值引用。对于局部变量，可以通过堆栈指针引用。

    * 寄存器间接寻址
            AT&T: (%eax)                        Intel: [eax]

    * 变址寻址
            AT&T: _variable(%eax)               Intel: [eax + _variable]
            AT&T: _array( ,%eax, 4)             Intel: [eax × 4 + _array]
            AT&T: _array(%ebx, %eax,8)          Intel: [ebx + eax × 8 + _array]
```

```bash
gdb调试功能包含如下：
    监视变量的值
    查看、修改变量的值
    查看、修改寄存器的值
    查看程序的堆栈情况
    
    设置断点
    单步调试

    调试线程


```
