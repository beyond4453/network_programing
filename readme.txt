此程序fork自孟宁老师的github上的stream项目
此处只列出了修改的两个文件 即 client.c & server.c
其他文件见https://github.com/mengning/stream

setup:
1.source build.env 如果出错则使用dos2unix build.env处理后重新执行
2.make clean
3.make
4./server
5./client 另开一个shell窗口

此程序利用stream简单实现了服务器与客户端之间的通讯
即基于stream实现发送和回传hello和hi


感谢您的测试，如果遇到任何问题，请联系：
QQ： 393852478
email : beyond_acm@163.com

