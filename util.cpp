#include "util.h"

using namespace std;

int Util::connect(const std::string& host, const int& port)
{

	struct addrinfo* answer = nullptr;

	int ret = getaddrinfo(host.c_str(), NULL, NULL, &answer);
	int sock = socket(answer->ai_family, SOCK_STREAM, IPPROTO_TCP);

	int16_t n_port = htons(port);
	memcpy(answer->ai_addr->sa_data, &n_port, 2);
	::connect(sock, answer->ai_addr, sizeof(struct sockaddr));

	freeaddrinfo(answer);

	return sock;
}

int Util::listen(const uint16_t port, const char* localIp, int backLog)
{

	int fd = socket(AF_INET,SOCK_STREAM,0);
	if (fd == -1)
		cerr << "创建失败" << endl;

	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(localIp);
	server.sin_port = htons(port);
	server.sin_family = AF_INET;

	bind(fd, (struct sockaddr*)&server,sizeof(server));
	::listen(fd,backLog);

		return fd;

}

int bind(const int& port, const char* ip = "0.0.0.0") {

	int fd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in self;
	self.sin_family = AF_INET;
	self.sin_port = htons(port);
	self.sin_addr.s_addr = inet_addr(ip);
	
	bind(fd,(struct sockaddr*)&self,sizeof(self));

	return fd;
}

string Util::get_local_ip(int fd) {
	struct sockaddr addr;
	struct sockaddr_in* addr_v4;
	socklen_t addr_len = sizeof(addr);

	memset(&addr, 0, sizeof(addr));
	if (0 == getsockname(fd, &addr, &addr_len)) {
		if (addr.sa_family == AF_INET) {
			addr_v4 = (sockaddr_in*)&addr;
			return string(inet_ntoa(addr_v4->sin_addr));		// const char* 向string转换
		}
	}
	return "0.0.0.0";
}

int Util::get_local_port(int fd) {
	struct sockaddr addr;
	struct sockaddr_in* addr_v4;
	socklen_t addr_len = sizeof(addr);

	memset(&addr, 0, sizeof(addr));
	if (0 == getsockname(fd, &addr, &addr_len)) {
		if (addr.sa_family == AF_INET) {
			addr_v4 = (sockaddr_in*)&addr;
			return ntohs(addr_v4->sin_port);		// const char* 向string转换
		}
	}
	return 0;
}


string Util::get_peer_ip(int fd) {
	struct sockaddr addr;
	struct sockaddr_in* addr_v4;
	socklen_t addr_len = sizeof(addr);

	memset(&addr, 0, sizeof(addr));		
	if (0 == getpeername(fd, &addr, &addr_len)) {				// 只是这边改成了getpeername，代码的重复还是挺高的
		if (addr.sa_family == AF_INET) {
			addr_v4 = (sockaddr_in*)&addr;
			return string(inet_ntoa(addr_v4->sin_addr));		// const char* 向string转换
		}
	}
	return 0;
}


int get_peer_port(int fd) {
	struct sockaddr addr;
	struct sockaddr_in* addr_v4;
	socklen_t addr_len = sizeof(addr);
	if (0 == getpeername(fd, &addr, &addr_len)) {
		if (addr.sa_family == AF_INET) {
			addr_v4 = (sockaddr_in*)&addr;
			return ntohs(addr_v4->sin_port);
		}
	}
	return 0;
}