# A C++ High Performance Web Server

[![Build Status](https://travis-ci.org/linyacool/WebServer.svg?branch=master)](https://travis-ci.org/linyacool/WebServer)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)

## Introduction

本项目为 C++11 编写的 Web 服务器，解析了 get、head 请求，可处理静态资源，支持 HTTP 长连接，支持管线化请求，并实现了异步日志，记录服务器运行状态。

测试页：http://www.linya.pub/

|                                   Part Ⅰ                                   |                                     Part Ⅱ                                     |                                                 Part Ⅲ                                                 |                                     Part Ⅳ                                     |                                                 Part Ⅴ                                                 |                                                 Part Ⅵ                                                 |
| :------------------------------------------------------------------------: | :----------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------: |
| [并发模型](https://github.com/linyacool/WebServer/blob/master/并发模型.md) | [连接的维护](https://github.com/linyacool/WebServer/blob/master/连接的维护.md) | [版本历史](https://github.com/linyacool/WebServer/blob/master/%E7%89%88%E6%9C%AC%E5%8E%86%E5%8F%B2.md) | [测试及改进](https://github.com/linyacool/WebServer/blob/master/测试及改进.md) | [项目目的](https://github.com/linyacool/WebServer/blob/master/%E9%A1%B9%E7%9B%AE%E7%9B%AE%E7%9A%84.md) | [面试问题](https://github.com/linyacool/WebServer/blob/master/%E9%9D%A2%E8%AF%95%E9%97%AE%E9%A2%98.md) |

## Envoirment

- OS: Ubuntu 14.04
- Complier: g++ 4.8

## Build

    ./build.sh

## Usage

    ./WebServer [-t thread_numbers] [-p port] [-l log_file_path(should begin with '/')]

## Technical points

- 使用 Epoll 边沿触发的 IO 多路复用技术，非阻塞 IO，使用 Reactor 模式
- 使用多线程充分利用多核 CPU，并使用线程池避免线程频繁创建销毁的开销
- 使用基于小根堆的定时器关闭超时请求
- 主线程只负责 accept 请求，并以 Round Robin 的方式分发给其它 IO 线程(兼计算线程)，锁的争用只会出现在主线程和某一特定线程中
- 使用 eventfd 实现了线程的异步唤醒
- 使用双缓冲区技术实现了简单的异步日志系统
- 为减少内存泄漏的可能，使用智能指针等 RAII 机制
- 使用状态机解析了 HTTP 请求,支持管线化
- 支持优雅关闭连接



## Model

并发模型为 Reactor+非阻塞 IO+线程池，新连接 Round Robin 分配，详细介绍请参考[并发模型](https://github.com/linyacool/WebServer/blob/master/并发模型.md)
![并发模型](https://github.com/linyacool/WebServer/blob/master/datum/model.png)

## 代码统计

![cloc](https://github.com/linyacool/WebServer/blob/master/datum/cloc.png)

## Others

除了项目基本的代码，进服务器进行压测时，对开源测试工具 Webbench 增加了 Keep-Alive 选项和测试功能: 改写后的[Webbench](https://github.com/linyacool/WebBench)
