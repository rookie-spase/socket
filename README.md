# socket
#做的一个极其简单的socket封装

不出意外的话这一周内会上传第一个版本，这是我第一个项目。
因为看不懂大佬们写的代码就先自己写吧。
用C++的class封装c的socket并加上一点C++的东西


借鉴项目：freecplus


第一版本： 就是对简单的socket进行封装，没有任何特性
第二版本： 将Send和Recv函数提取出来，提升传输数据可靠性；  
    第二版的read和write函数咋都写不对，草
第三版本： 写入sockException,不带error Code


#运行

环境：centos 7  第二版运行成功


现在是可以进行简单的发送和接收消息
