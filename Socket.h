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

class TcpSocket {
private:
	int listend;
	int client_fd;
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	char buffer[1024];

public:
	TcpSocket(const int& port);
	~TcpSocket() {
		close(listend);
		close(client_fd);

	};

	inline bool Bind() { return bind(listend, (struct sockaddr*)&addr, sizeof(addr)) == 0; }
	inline bool Listen(const int& len = 5) { return listen(listend, len) == 0; }

	void Accept();

	int Send(const char* str);
 
	const char* get_buffer() const {
		return buffer;
	}
	int Recv() {
		memset(buffer, 0, sizeof(buffer));
		return recv(client_fd, buffer, sizeof(buffer), 0);
	}
};


class TcpClient {
private:
	int fd;
	struct sockaddr_in  server;
	char buffer[1024];
public:
	TcpClient(const int& port, const char* ip);
	~TcpClient() { close(fd); };


	bool Connect() {
		return connect(fd,(struct sockaddr*)&server,sizeof(server)) == 0;
	}
	
	int Send(const char* str) {
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, str);
		return send(fd, buffer, strlen(buffer),0);
	}
	int Recv() {
		memset(buffer, 0, sizeof(buffer));
		return recv(fd, buffer, sizeof(buffer),0);
	}
	const char* get_buffer() const {
		return buffer;
	}
};


// Send和recv函数可以单独提取出来
// buffer 可以单独定义一个类
// 基类Socket可以有一个sock_fd,
//		继承Socket:  TCP服务端(自己再定义一个client_fd)
					//客户端就不用定义,当成连接的socket就行
					//select模型自己定义个fd_set
					//poll模型自己定义数组struct pollfd fds[MAXNFDS];
					//epoll模型自己创建poll编号 int epollfd;


#endif