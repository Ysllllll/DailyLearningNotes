## 命令

## 面试题

- 写程序删除一个文件的第10行
- 补充函数：truncate / ftruncate

# 文件系统

## 一、目录和文件

1.获取文件属性
    stat：通过文件路径获取属性，面对符号链接文件时获取的是所指向的目标文件的属性
    fstat：通过文件描述符获取属性
    lstat：面对符号链接文件时获取的是符号链接文件的属性
2.文件访问权限
    st_mode是一个16位的位图，用于表示文件类型，文件访问权限

3.umask
    作用：防止产生权限过松的文件

4.文件权限的更改、管理
    chmod
    fchmod

5.粘住位
    t位

6.文件系统：FAT,UFS
    文件系统：文件或数据的存储和管理

7.硬链接，符号链接
    硬链接与目录项是同义词，且建立硬链接有限制：不能区分建立，不能给目录建立。符号链接优点：可跨分区，可以给目录建立
    link
    unlink
    remove
    rename

8.utime：可更改文件的最后读的时间和最后修改的时间

9.目录的创建和销毁
    mkdir
    rmdir

10.更改当前工作路径
    chdir
    fchdir
    getpwd

11.分析目录、读取目录内容

读取目录方式一：glob.c
glob()：解析模式 / 通配符

读取目录方式二：getdir.c
opendir();
closedir();
readdir();
rewinddir();
seekdir();
telldir();

du 可以分析出来一个目录或者是一个文件所占的磁盘空间大小，默认是以k为单位。可以使用stat来验证，Blocks / 2 即为所占磁盘空间大小，两种方式获取的结果应该是一致的。

面试题：类ls的实现，如myls -l -a -i -n

面试题：touch -a，注意-a是文件，但是命令行会报错，它把它当成了命令
touch -- -a
touch ./-b

## 二、系统数据文件和信息

1 /etc/passwd
    getpwuid();
    getpwname();

2 /etc/group
    getgrgid();
    getgrnam();

3 /etc/shadow
    getspnam();
    crypt(); 

4 时间戳
