此程序实现了基于C/S架构的TCP服务器：

1.通过makefile来生成可执行程序
$ make clean
$ make


2.运行服务器
$ ./server
此时会打印Accepting conections ...，服务器会等待客户链接



3.在linux下再开一个终端，编译客户端代码
$ gcc client.c -o client



4.运行客户端的代码
$ ./client hello
如果成功则会收到服务器返回的"hi".



感谢您的测试，如果遇到任何问题，请联系：
QQ： 393852478
email : beyond_acm@163.com

