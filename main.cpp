#define _WINSOCK_DEPRECATED_NO_WARNINGS 	//比较新版的vs,会警告我们不要使用一
//下旧的函数,因为提供更新更安全的函数供我们使用,在这呢我们
//还是用旧的吧,这个宏定义就是起屏蔽警告作用,VS下面也有提示的
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<string>
#include<cstring>
#include<thread>
#include<WS2tcpip.h>
#include <WinSock2.h>						//一般情况下,这个头文件位于windows.h之前,避免发生某些错误
#include<Windows.h>
#pragma comment(lib, "ws2_32.lib") 			//显示加载 ws2_32.dll	ws2_32.dll就是最新socket版本啦
using namespace std;
void receive(SOCKET recvClientSocket)
{
	while (true)
	{

		//	6	发送/接受 数据
		char recvBuf[4024] = {};
		int reLen = recv(recvClientSocket, recvBuf, 4024, 0);
		//int sLen = send(recvClientSocket, recvBuf, reLen, 0);
		if (SOCKET_ERROR != reLen) {

			cout << "客户端:    " << recvBuf << endl << endl;
			reLen = SOCKET_ERROR;
		}
	}

}
void mysend(SOCKET recvClientSocket)
{
	while (true)
	{
		string s;
		//cout << "请输入发送数据" << endl;
		getline(cin, s);
		int seLen = send(recvClientSocket, (char*)s.c_str(), s.size(), 0);
		if (SOCKET_ERROR != seLen)
		{
			cout << "我：    " << s << endl << endl;
		}
	}
}
void service()
{
	
		cout << "-----------服务器-----------" << endl;

		//	1	初始化
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);	//make word,你把鼠标移到WSAStartup看看参数列表,是不是就是一个word啊


		//	2	创建服务器的套接字
		SOCKET serviceSocket;
		serviceSocket = socket(AF_INET, SOCK_STREAM, 0);	//socket(协议族,socket数据传输方式,某个协议)	我们默认为0,其实就是一个宏
		if (SOCKET_ERROR == serviceSocket) {
			cout << "套接字创建失败!" << endl;
		}
		else {
			cout << "套接字创建成功!" << endl;
		}


		//	3	绑定套接字	指定绑定的IP地址和端口号
		sockaddr_in socketAddr;								//一个绑定地址:有IP地址,有端口号,有协议族
		socketAddr.sin_family = AF_INET;
		socketAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");		//代码开头第一行我们定义的宏在这就其作用啦
		socketAddr.sin_port = htons(2305);
		int bRes = bind(serviceSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));	//绑定注意的一点就是记得强制类型转换
		if (SOCKET_ERROR == bRes) {
			cout << "绑定失败!" << endl;
		}
		else {
			cout << "绑定成功!" << endl;
		}

		//	4	服务器监听	
		int lLen = listen(serviceSocket, 5);	//监听的第二个参数就是:能存放多少个客户端请求,到并发编程的时候很有用哦
		if (SOCKET_ERROR == lLen) {
			cout << "监听失败!" << endl;
		}
		else {
			cout << "监听成功!" << endl;
		}


		//	5	接受请求
		sockaddr_in revClientAddr;
		SOCKET recvClientSocket = INVALID_SOCKET;	//初始化一个接受的客户端socket
		int _revSize = sizeof(sockaddr_in);
		recvClientSocket = accept(serviceSocket, (SOCKADDR*)&revClientAddr, &_revSize);
		if (INVALID_SOCKET == recvClientSocket) {
			cout << "服务端接受请求失败!" << endl;
		}
		else {
			cout << "服务端接受请求成功!" << endl;
		}
		// 循环接收数据
		thread reveMessage(receive, recvClientSocket);
		thread sendMessage(mysend, recvClientSocket);
		reveMessage.detach();
		sendMessage.join();



		//	7	关闭socket
		closesocket(recvClientSocket);
		closesocket(serviceSocket);

		//	8	终止
		WSACleanup();

		cout << "服务器停止" << endl;
		cin.get();

	

}
void client()
{
	cout << "-----------客户端-----------" << endl;

	//	1	初始化
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);


	//	2	创建套接字
	SOCKET clientSocket = {};
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == clientSocket) {
		cout << "套接字创建失败!" << endl;
	}
	else {
		cout << "套接字创建成功!" << endl;
	}


	//	3	绑定套接字	指定绑定的IP地址和端口号
	sockaddr_in socketAddr;
	socketAddr.sin_family = PF_INET;
	socketAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	socketAddr.sin_port = htons(2305);
	int cRes = connect(clientSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == cRes) {
		cout << "客户端:\t\t与服务器连接失败....." << endl;
	}
	else {
		cout << "客户端:\t\t与服务器连接成功....." << endl;
	}



	//while (true)
	//{
	//	//	4	发送请求
	//	string s;
	//	cout << "输入发送数据:   " << endl;
	//	getline(cin, s);					//可输入空格,默认以换行符结束输入,
	//	send(clientSocket, (char*)s.c_str(), s.length(), 0);
	//	//	5	发送/接受 数据
	//	/*char recvBuf[4024] = {};
	//	recv(clientSocket, recvBuf, 4024, 0);
	//	cout << "客户端接收数据	:	" << recvBuf << endl << endl;*/
	//}
	//
	thread reveMessage(receive, clientSocket);
	thread sendMessage(mysend, clientSocket);
	reveMessage.detach();
	sendMessage.join();



	//	6	关闭socket
	closesocket(clientSocket);


	//	7	终止
	WSACleanup();

	cout << "客户端退出" << endl;
	cin.get();
	
}
int main()
{
	cout << "发起连接->1" << endl;
	cout << "加入链接->2" << endl;
	int a;
	cin >> a;
	if (a == 1)
	{
		service();
	}
	else if (a == 2)
	{
		client();
	}
}