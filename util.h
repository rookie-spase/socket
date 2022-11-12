#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_


#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <string>


namespace Util {


	// 这个命名空间目前就2个作用
	//  对socket设置
	// 通过socket读取地址这一类的信息

	int connect(const std::string& host, const int& port);
	int listen(const uint16_t port, const char* localIp, int backLog);
	int bind(const int& port, const char* ip = "0.0.0.0");



	// 不对啊，我怎么光cv代码去了..
	string get_local_ip(int fd);  // 从fd获取地址信息，这我真没见过，学到了(getsockname(fd,addr,addr_len))
	int get_local_port(int fd);
	string get_peer_ip(int fd);		// 获取远程的信息就改成getpeername就行；
	int get_peer_port(int fd);
};




#endif