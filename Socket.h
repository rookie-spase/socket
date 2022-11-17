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
#include<string>
#include <poll.h>
#include<sys/epoll.h>
#include <fcntl.h>


// 一直 (发送/接收) 数据，直到(发送/接收)了len个字节
//bool Write(const int& fd, char* buffer, const int& len);
//bool Read(const int& fd, char* buffer, const int& len);
bool Read(const int sockfd, char* buffer, const size_t n);
bool Write(const int sockfd, const char* buffer, const size_t n);

// 用来告诉Write和read应该发送多少和接收多少
//bool TcpRead(const int& fd, char* buffer, int* success_obtained);
//bool TcpWrite(const int& sockfd, char* buffer, const int ibuflen);
bool TcpRead(const int sockfd, char* buffer, int* ibuflen);
bool TcpWrite(const int sockfd, const char* buffer, const int ibuflen = 0);



class base_socket {
protected:
	int fd;				
				/*
				* 客户端:通信的socket
				* 服务端: 监听的socket
				* 基于select实现的服务端: 监听的socket
				* 基于poll的服务端: 监听socket
				*		也想作为服务器通信socket但是，服务器需要先listen之后从accept中得到socket才行
				*/
	char buffer[1024];

public:
	base_socket() :fd(socket(AF_INET, SOCK_STREAM, 0)) { memset(buffer, 0, sizeof(buffer)); }
	virtual ~base_socket() { if (fd > 0)close(fd); };

	virtual bool Recv(const int& fd);
	virtual bool Send(const int& fd, const char* str);
};

class TcpServer :public base_socket{
private:
	int client_fd;
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;

public:
	TcpServer(const int& port);
	~TcpServer() {
		// 这里会先调用tcp析构然后再调用base_socket的析构
		close(client_fd);

	};

	bool Bind();
	bool Listen(const int& len = 5);
	bool Accept();

	int Send(const char* str);
	int Recv();

	void close_listen() { close(fd); }
	void close_connect() { close(client_fd); }

	const char* get_buffer() const { return buffer; }
	const char* get_client_ip()const {return inet_ntoa(client_addr.sin_addr);}
};
class TcpClient :public base_socket {
private:
	struct sockaddr_in  server;
public:
	TcpClient(const int& port, const char* ip);
	~TcpClient() {};


	bool Connect();
	
	int Send(const char* str);
	int Recv();

	const char* get_buffer() const {return buffer; }
};


struct SelectServer :public base_socket {
private:
	fd_set rally_of_socket;
	int max_fd;

	struct sockaddr_in server;

public:
	SelectServer(const int& port, const int& listen_count = 5);
	~SelectServer();

	void polling();			// 轮询，从最开始询问，代替for;
							// 也可以理解为void run()


};
// poll模型和select 模型差别不大,把poll整完了之后epoll也是类似
const int MAXSOCK = 1024;
class PollServer :public base_socket {
private:
	struct pollfd fds[1024];				// 管理socket的集合
	struct sockaddr_in server;				// 本身的ip地址
	int maxfd;



public:
	PollServer(const int& port, const int& listen_count = 5);
	~PollServer();

	void polling();
};

/*
struct epoll_event {
	__uint32_t events;    
	epoll_data_t data;     
};
*/

int setnonblocking(int sockfd);
class EpollServer : public base_socket {
private:
	struct epoll_event ev;
	int epollfd;
	//struct sockaddr_in server;
public:
	EpollServer(const int& port,const int& listen_count = 5 );
	~EpollServer() { close(epollfd); };	// 不用在析构添加   释放循环   ， 因为在polling的时候已经解决了。 而listen  socket  则是在基类base_socket中被释放了


	void polling();




};

/*
class Buffer {
public:
	std::string data;

	Buffer(const std::string& str) :data(str) {};

	operator char* () {			// 隐式转换
		return const_cast<char*>(data.c_str());
	};			// 这是一个技术点，想要class隐式转换得写   operator 目标类型(){}
				// 那这样的话就不需要再考虑任何安全性和兼容性了
				// 因为可以向const char* 转换嘛
	operator const char* () {			// 隐式转换
		return data.c_str();
	};
	char* operator = (const char* str) {
		data = str;
		return this->operator char* ();
	};

};
*/


		// 这是真简单啊
	// 这段在g++ 编译的时候要加上选项 -std=c++11
class sockException :public std::exception {
private:
	std::string errMsg;
public:
	virtual const char* what() const noexcept {
		return errMsg.c_str();
	}
	sockException(const std::string& str) :errMsg(str) {};
};


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