#include <iostream>
#include<unistd.h>
#include"Socket.h"
using namespace std;



TcpServer server(123);

void fth_ext(int sig) {
	if (sig > 0)
	{
		signal(sig, SIG_IGN); signal(SIGINT, SIG_IGN); signal(SIGTERM, SIG_IGN);
		cout << "sig: " << sig << endl;
	}

	kill(0, 15);  // 通知其它的子进程退出。

	cout << "fth proceess exit" << endl;

	// 编写善后代码（释放资源、提交或回滚事务）
	server.close_listen();

	exit(0);
}
void ch_ext(int sig) {
	if (sig > 0)
	{
		signal(sig, SIG_IGN); signal(SIGINT, SIG_IGN); signal(SIGTERM, SIG_IGN);
	}

	cout << "child procees exit " << endl;

	server.close_connect();

	exit(0);
}

int main(int argc,char* argv[])
{  
	for (int i = 0; i < 50; ++i,signal(i,SIG_IGN));
	
	
	server.Bind();
	server.Listen();


	signal(SIGINT,fth_ext);
	signal(SIGTERM,fth_ext);
	

 
	string str;

	while (1) {
		if (server.Accept() == false) {
			cout << "accept fail" << endl;
			sleep(5);
			continue;
		}
		if (fork() > 0) { server.close_connect(); continue; }
		server.close_listen();
		signal(SIGINT, ch_ext);
		signal(SIGTERM, ch_ext);

		while (1) {
			server.Recv();
			if (strcmp(server.get_buffer(), "none") == 0)break;
			cout << "recv: " << server.get_buffer() << endl;

			//cout << "send something:";
			//cin >> str;
			//server.Send(str.c_str());
			server.Send("ok");
			cout << "send: " << server.get_buffer() << endl;
		}
		ch_ext(-1);
	}

	

}
int main(int argc, char* argv[])
{

	TcpClient client(atoi(argv[2]), argv[1]);
	
	client.Connect();


	
	for (int i = 3; i < 10; ++i) {
		client.Send("what happen");
		cout << "send: " << client.get_buffer() << endl;

		client.Recv();
		cout << "recv: " << client.get_buffer() << endl;
	}
	client.Send("none");

}