#define _WINSOCK_DEPRECATED_NO_WARNINGS 	//�Ƚ��°��vs,�ᾯ�����ǲ�Ҫʹ��һ
//�¾ɵĺ���,��Ϊ�ṩ���¸���ȫ�ĺ���������ʹ��,����������
//�����þɵİ�,����궨����������ξ�������,VS����Ҳ����ʾ��
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<string>
#include<cstring>
#include<thread>
#include<WS2tcpip.h>
#include <WinSock2.h>						//һ�������,���ͷ�ļ�λ��windows.h֮ǰ,���ⷢ��ĳЩ����
#include<Windows.h>
#pragma comment(lib, "ws2_32.lib") 			//��ʾ���� ws2_32.dll	ws2_32.dll��������socket�汾��
using namespace std;
void receive(SOCKET recvClientSocket)
{
	while (true)
	{

		//	6	����/���� ����
		char recvBuf[4024] = {};
		int reLen = recv(recvClientSocket, recvBuf, 4024, 0);
		//int sLen = send(recvClientSocket, recvBuf, reLen, 0);
		if (SOCKET_ERROR != reLen) {

			cout << "�ͻ���:    " << recvBuf << endl << endl;
			reLen = SOCKET_ERROR;
		}
	}

}
void mysend(SOCKET recvClientSocket)
{
	while (true)
	{
		string s;
		//cout << "�����뷢������" << endl;
		getline(cin, s);
		int seLen = send(recvClientSocket, (char*)s.c_str(), s.size(), 0);
		if (SOCKET_ERROR != seLen)
		{
			cout << "�ң�    " << s << endl << endl;
		}
	}
}
void service()
{
	
		cout << "-----------������-----------" << endl;

		//	1	��ʼ��
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);	//make word,�������Ƶ�WSAStartup���������б�,�ǲ��Ǿ���һ��word��


		//	2	�������������׽���
		SOCKET serviceSocket;
		serviceSocket = socket(AF_INET, SOCK_STREAM, 0);	//socket(Э����,socket���ݴ��䷽ʽ,ĳ��Э��)	����Ĭ��Ϊ0,��ʵ����һ����
		if (SOCKET_ERROR == serviceSocket) {
			cout << "�׽��ִ���ʧ��!" << endl;
		}
		else {
			cout << "�׽��ִ����ɹ�!" << endl;
		}


		//	3	���׽���	ָ���󶨵�IP��ַ�Ͷ˿ں�
		sockaddr_in socketAddr;								//һ���󶨵�ַ:��IP��ַ,�ж˿ں�,��Э����
		socketAddr.sin_family = AF_INET;
		socketAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");		//���뿪ͷ��һ�����Ƕ���ĺ��������������
		socketAddr.sin_port = htons(2305);
		int bRes = bind(serviceSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));	//��ע���һ����Ǽǵ�ǿ������ת��
		if (SOCKET_ERROR == bRes) {
			cout << "��ʧ��!" << endl;
		}
		else {
			cout << "�󶨳ɹ�!" << endl;
		}

		//	4	����������	
		int lLen = listen(serviceSocket, 5);	//�����ĵڶ�����������:�ܴ�Ŷ��ٸ��ͻ�������,��������̵�ʱ�������Ŷ
		if (SOCKET_ERROR == lLen) {
			cout << "����ʧ��!" << endl;
		}
		else {
			cout << "�����ɹ�!" << endl;
		}


		//	5	��������
		sockaddr_in revClientAddr;
		SOCKET recvClientSocket = INVALID_SOCKET;	//��ʼ��һ�����ܵĿͻ���socket
		int _revSize = sizeof(sockaddr_in);
		recvClientSocket = accept(serviceSocket, (SOCKADDR*)&revClientAddr, &_revSize);
		if (INVALID_SOCKET == recvClientSocket) {
			cout << "����˽�������ʧ��!" << endl;
		}
		else {
			cout << "����˽�������ɹ�!" << endl;
		}
		// ѭ����������
		thread reveMessage(receive, recvClientSocket);
		thread sendMessage(mysend, recvClientSocket);
		reveMessage.detach();
		sendMessage.join();



		//	7	�ر�socket
		closesocket(recvClientSocket);
		closesocket(serviceSocket);

		//	8	��ֹ
		WSACleanup();

		cout << "������ֹͣ" << endl;
		cin.get();

	

}
void client()
{
	cout << "-----------�ͻ���-----------" << endl;

	//	1	��ʼ��
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);


	//	2	�����׽���
	SOCKET clientSocket = {};
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == clientSocket) {
		cout << "�׽��ִ���ʧ��!" << endl;
	}
	else {
		cout << "�׽��ִ����ɹ�!" << endl;
	}


	//	3	���׽���	ָ���󶨵�IP��ַ�Ͷ˿ں�
	sockaddr_in socketAddr;
	socketAddr.sin_family = PF_INET;
	socketAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	socketAddr.sin_port = htons(2305);
	int cRes = connect(clientSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == cRes) {
		cout << "�ͻ���:\t\t�����������ʧ��....." << endl;
	}
	else {
		cout << "�ͻ���:\t\t����������ӳɹ�....." << endl;
	}



	//while (true)
	//{
	//	//	4	��������
	//	string s;
	//	cout << "���뷢������:   " << endl;
	//	getline(cin, s);					//������ո�,Ĭ���Ի��з���������,
	//	send(clientSocket, (char*)s.c_str(), s.length(), 0);
	//	//	5	����/���� ����
	//	/*char recvBuf[4024] = {};
	//	recv(clientSocket, recvBuf, 4024, 0);
	//	cout << "�ͻ��˽�������	:	" << recvBuf << endl << endl;*/
	//}
	//
	thread reveMessage(receive, clientSocket);
	thread sendMessage(mysend, clientSocket);
	reveMessage.detach();
	sendMessage.join();



	//	6	�ر�socket
	closesocket(clientSocket);


	//	7	��ֹ
	WSACleanup();

	cout << "�ͻ����˳�" << endl;
	cin.get();
	
}
int main()
{
	cout << "��������->1" << endl;
	cout << "��������->2" << endl;
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