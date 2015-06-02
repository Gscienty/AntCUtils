#pragma once
#include "Object.hpp"
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <functional>
#include <thread>
#pragma comment(lib,"ws2_32.lib")


class AnSocket : public Object{
public://静态方法
	static void CreateSocket(std::function<void(AnSocket)> func){
		std::thread([=]{
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(1, 1), &wsaData)) return;
			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1){
				WSACleanup();
				return;
			}
			func(*(new AnSocket()));
			WSACleanup();
		}).detach();
	}

public://继承方法
	std::string ToString() override { return this->_selfString; }

public://本地方法
	//建立TCP服务器
	AnSocket BuildStreamServer(u_short port, int type){
		//初始化套接字
		this->_sock = socket(type, SOCK_STREAM, 0);
		//初始化地址
		memset(&this->_addrSvr, 0, sizeof(this->_addrSvr));
		this->_addrSvr.sin_family = type;
		this->_addrSvr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		this->_addrSvr.sin_port = htons(port);
		//绑定
		bind(this->_sock, (SOCKADDR*)&this->_addrSvr, sizeof(SOCKADDR));
		return *this;
	}

	//建立TCP客户端
	AnSocket BuildTcpClient(int type, int svrPort, std::string ip){
		this->_sock = socket(type, SOCK_STREAM, IPPROTO_TCP);
		memset(&this->_addrSvr, 0, sizeof(this->_addrSvr));
		this->_addrSvr.sin_family = type;
		this->_addrSvr.sin_port = htons(svrPort);
		inet_pton(type, ip.c_str(), (void *)&this->_addrSvr.sin_addr);
		return *this;
	}

	//服务器监听
	AnSocket ServerListen(int waiting, std::function<void(SOCKET, sockaddr_in)> func){
		if (listen(this->_sock, waiting) < 0) return *this;
		int len = sizeof(SOCKADDR);
		sockaddr_in addrClet;
		while (this->_svrSwitch){
			SOCKET sockConn = accept(this->_sock, (SOCKADDR*)&addrClet, &len);
			int error = GetLastError();
			func(sockConn, addrClet);
			closesocket(sockConn);
		}
		return *this;
	}

	AnSocket ConnectServer(){
		if (connect(this->_sock, (sockaddr*)&this->_addrSvr, sizeof(this->_addrSvr)) == SOCKET_ERROR){
			int ret = GetLastError();
			return this->CloseConnect();
		}
		return *this;
	}

	AnSocket Send(std::string msg){
		if (send(this->_sock, msg.c_str(), msg.length(), 0) == SOCKET_ERROR){
			return this->CloseConnect();
		}
		return *this;
	}

	AnSocket CloseConnect(){
		closesocket(this->_sock);
		return *this;
	}

private://私有方法
	AnSocket(){
		this->_selfString = "AnSocket Object";
		this->_svrSwitch = true;
		this->_status = 0;
	}

private://私有字段
	u_short _status;
	bool _svrSwitch;
	std::string _selfString;
	sockaddr_in _addrSvr;
	SOCKET _sock;
};