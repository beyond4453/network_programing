此程序基于pthread_create方式实现了一个并发TCP服务器：

1.通过makefile来生成可执行程序
$ make clean
$ make

2.运行服务器
$ ./server
此时会打印Accepting conections ...，服务器会等待客户链


3.可以同时开启多个客户端运行
$ ./client hello
如果成功则会收到服务器返回的"hi".

感谢您的测试，如果遇到任何问题，请联系：
QQ： 393852478
email : beyond_acm@163.com

