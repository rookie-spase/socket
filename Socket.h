#pragma once
#ifndef _SOCKET_H_
#define _SOCKET_H_

#include<iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include<netdb.h>


// 一直 (发送/接收) 数据，直到(发送/接收)了len个字节
//bool Write(const int& fd, char* buffer, const int& len);
//bool Read(const int& fd, char* buffer, const int& len);
bool Read(const int sockfd, char* buffer, const size_t n);
bool Write(const int sockfd, const char* buffer, const size_t n);

// 用来告诉Write和read应该发送多少和接收多少
//bool TcpRead(const int& fd, char* buffer, int* success_obtained);
//bool TcpWrite(const int& sockfd, char* buffer, const int ibuflen);
bool TcpRead(const int sockfd, char* buffer, int* ibuflen);
bool TcpWrite(const int sockfd, const char* buffer, const int ibuflen);


class TcpServer {
private:
	int listend;
	int client_fd;
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	char buffer[1024];

public:
	TcpServer(const int& port);
	~TcpServer() {
		close(listend);
		close(client_fd);

	};

	bool Bind();
	bool Listen(const int& len = 5);
	void Accept();

	int Send(const char* str);
	int Recv();

	const char* get_buffer() const { return buffer; }
	const char* get_client_ip()const {return inet_ntoa(client_addr.sin_addr);}
};




class TcpClient {
private:
	int fd;
	struct sockaddr_in  server;
	char buffer[1024];
public:
	TcpClient(const int& port, const char* ip);
	~TcpClient() { close(fd); };


	bool Connect();
	
	int Send(const char* str);
	int Recv();

	const char* get_buffer() const {return buffer;}
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*	这里是将 send 和 recv 函数单独提取出来了
*	提取出来也为了解决  "粘包"  和   "分包"    的问题
* 
*	Write，Read   这两个函数确保可以读取指定长度的数据
*	TcpWrite,TcpRead 这两个函数是来告诉 Write 和 Read函数应该接收多少字节
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// 一直 (发送/接收) 数据，直到(发送/接收)了len个字节
//bool Write(const int& fd, const char* buffer, const int& len);
//bool Read(const int& fd, char* buffer, const int& len);

// 用来告诉Write和read应该发送多少和接收多少
//bool TcpRead(const int& fd, char* buffer, int* success_obtained);
//bool TcpWrite(const int& sockfd, const char* buffer, const int& ibuflen);

// 现在暂时不考虑使用select模型这类东西
//-------------------------------------------------------------------------------------------------------------

// Send和recv函数可以单独提取出来
//		产生了{Write，Read},{TcpWrite,TcpRead}函数。  使得数据传输更加可靠
// buffer 可以单独定义一个类
// 基类Socket可以有一个sock_fd,
//		继承Socket:  TCP服务端(自己再定义一个client_fd)
					//客户端就不用定义,当成连接的socket就行
					//select模型自己定义个fd_set
					//poll模型自己定义数组struct pollfd fds[MAXNFDS];
					//epoll模型自己创建poll编号 int epollfd;


#endif