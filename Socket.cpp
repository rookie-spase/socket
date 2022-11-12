#include"Socket.h"

TcpServer::TcpServer(const int& port)
	:client_fd(-1),base_socket()
{

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		memset(&client_addr,0,sizeof(client_addr));
}

bool TcpServer::Bind()
{
	return ::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0;
}

bool TcpServer::Listen(const int& len)
{
	return listen(fd, len) == 0;
}

bool TcpServer::Accept()
{
		int socklen = sizeof(client_addr);
		client_fd = accept(fd, (struct sockaddr*)&client_addr, (socklen_t*)&socklen);
		std::cout << "client " << inet_ntoa(client_addr.sin_addr) << " Linked." << std::endl;

		
		return client_fd < 0 ? false:true ;
}


int TcpServer::Send(const char* str)
{
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, str);

		return TcpWrite(client_fd, buffer,0);
}

int TcpServer::Recv()
{
		memset(buffer, 0, sizeof(buffer));
		int len = 0;
		return TcpRead(client_fd, buffer, &len);
}

TcpClient::TcpClient(const int& port, const char* ip)
	:base_socket()
{
	struct hostent* h = gethostbyname(ip);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	memcpy(&server.sin_addr, h->h_addr, h->h_length); // 将ip地址放到服务器类
}

bool TcpClient::Connect()
{
	return ::connect(fd, (struct sockaddr*)&server, sizeof(server)) == 0;
}

int TcpClient::Send(const char* str)
{
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, str);
	return TcpWrite(fd, buffer, 0);
}

int TcpClient::Recv()
{
	memset(buffer, 0, sizeof(buffer));
	int ilen = 0;
	return TcpRead(fd, buffer, &ilen);
}

/*
bool Write(const int& fd, char* buffer, const int& len)
{

	for (int start{}, end = len, has_send{};					// 记录开始和结束的点
		end > 0;
		end -= has_send, start += has_send)
	{
		if (has_send = send(fd, buffer + start, strlen(buffer), 0) <= 0)	// send函数会返回成功发送了多少
		{
			return false;
		}
	}

	return true;
}
*/
bool Write(const int sockfd, const char* buffer, const size_t n)
{
	
	int nLeft, idx, nwritten;
	nLeft = n;
	idx = 0;
	while (nLeft > 0)
	{
		if ((nwritten = send(sockfd, buffer + idx, nLeft, 0)) <= 0) return false;

		nLeft -= nwritten;
		idx += nwritten;
	}

	return true;
	/*
	for (int start{}, end = n, has_send{};					// 记录开始和结束的点
					end > 0;
		end -= has_send, start += has_send)
	{
		if (has_send = send(sockfd, buffer + start, end, 0) <= 0)	// send函数会返回成功发送了多少
			return false;
	}

	return true;*/
}

/*
bool Read(const int& fd, char* buffer, const int& len)
{
	for (int start{}, end = len, has_recv{};
		end > 0;
		end -= has_recv, start += has_recv)
	{
		if (has_recv = recv(fd, buffer + start, end, 0) <= 0)
		{
			return false;
		}
	}

	return true;
}*/

bool Read(const int sockfd, char* buffer, const size_t n)
{
	
	int nLeft, nread, idx;

	nLeft = n;
	idx = 0;

	while (nLeft > 0)
	{
		if ((nread = recv(sockfd, buffer + idx, nLeft, 0)) <= 0) return false;

		idx += nread;
		nLeft -= nread;
	}/*
	for (int start = 0, end = n, has_recv = 0;
		end > 0;
		end -= has_recv, start += has_recv)
	{
		if (has_recv = recv(sockfd, buffer + start, end, 0) <= 0)
		{
			return false;
		}
	}*/

	return true;
}

/*
bool TcpRead(const int& fd, char* buffer, int* success_obtained)
{
	
	if (fd < 0)return false;
	memset(buffer, 0, sizeof(buffer));
	(*success_obtained) = 0;
	if (Read(fd, (char*)success_obtained, 4) == false)return false;			//  接收4个字节，也就是一个int来表示内容长度
	*success_obtained = ntohl(*success_obtained);	// 把网络字节序变成主机字节序
	if (Read(fd, buffer, *success_obtained) == false)return false;			// 接收内容

	return true;
}*/

bool TcpRead(const int fd, char* buffer, int* len)
{
	if (fd == -1) return false;

	(*len) = 0;
	if (Read(fd, (char*)len, 4) == false) return false;        // 接收长度
	(*len) = ntohl(*len);										// 把网络字节序转换为主机字节序。
	if (Read(fd, buffer, (*len)) == false) return false;     // 接受内容

	return true;
}

/*
bool TcpWrite(const int& sockfd, char* buffer, const int& ibuflen)
{
	if (sockfd < 0) return false;

	int ilen = (ibuflen == 0) ? strlen(buffer) : ibuflen;  // 0就是buffer的长度
	int ilenn = htonl(ilen);							// 转换为网络字节序。

	char strTBuffer[ilen + 4];
	memset(strTBuffer,0,sizeof(strTBuffer));
	memcpy(strTBuffer, &ilenn, 4);						// 将长度写入
	memcpy(strTBuffer + 4, buffer, ilen);             // 将内容写入

	if (Write(sockfd, strTBuffer, ilen + 4) == false) return false;

	return true;
}
*/

bool TcpWrite(const int fd, const char* buffer, const int len)
{
	if (fd == -1) return false;

	//int target_length = 0;
		// 如果长度为0，就采用字符串的长度
	//if (len == 0) target_length = strlen(buffer);
	//else target_length = len;
	int target_length = (len == 0) ? strlen(buffer) : 0;


	int target_lengthn = htonl(target_length);  // 转换为网络字节序。

	char strTBuffer[target_length + 4];
	memset(strTBuffer, 0, sizeof(strTBuffer));
	memcpy(strTBuffer, &target_lengthn, 4);                  // 将长度写入
	memcpy(strTBuffer + 4, buffer, target_length);             // 将内容写入

	if (Write(fd, strTBuffer, target_length + 4) == false) return false;

	return true;
}
