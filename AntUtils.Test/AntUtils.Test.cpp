// AntUtils.Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "../AntUtils/Socket.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	AnSocket::CreateSocket([](AnSocket sock){
		//std::cout << "build server port 6655\n";
		//sock.BuildStreamServer(6655, AF_INET).ServerListen(5, [](SOCKET sock, sockaddr_in addr){

		//});
		char recvBuff[1024] = { 0 };
		std::string s = "hello";
		sock.BuildTcpClient(AF_INET, 6655, "127.0.0.1")
			.ConnectServer()
			.SendToServer(s.c_str(),s.length())
			.RecvServerMessage(recvBuff, 1024, [](const char* buf,int len){
			std::cout << buf;
		})
			.CloseConnect();
	});
	std::cout << "�����������\n";
	system("pause");
	return 0;
}
