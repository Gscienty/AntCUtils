// AntUtils.Test.cpp : 定义控制台应用程序的入口点。
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

		sock.BuildTcpClient(AF_INET, 6655, "127.0.0.1")
			.ConnectServer().Send("Hello").CloseConnect();
	});
	std::cout << "按任意键结束\n";
	system("pause");
	return 0;
}
