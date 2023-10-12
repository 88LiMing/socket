/*******************************************************************
* 文件名称：socket_server.cpp
* 功能描述：socket服务端实现
* 服务器端使用Winsock库提供的socket()函数创建一个新的socket对象，并使用bind()方法绑定IP地址和端口号。
* 然后使用listen()方法开始监听客户端连接请求，接收到客户端连接请求后使用accept()方法接受连接。
* 最后使用send()方法向客户端发送欢迎消息，关闭客户端连接。
* 创建作者：LM
* 创建日期：2023/03/29
*******************************************************************/

//------------------------------------------------------------------
// 包含宏
//------------------------------------------------------------------
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//------------------------------------------------------------------
// 包含头文件
//------------------------------------------------------------------
#include <stdio.h>
#include <winsock2.h>

//------------------------------------------------------------------
// 包含静态库
//------------------------------------------------------------------
#pragma comment(lib, "ws2_32.lib") 

int main()
{
	// 初始化Winsock库
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	// 创建socket对象
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		printf("socket failed: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	// 绑定端口号和IP地址
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("192.168.132.221");
	service.sin_port = htons(8000);

	iResult = bind(listenSocket, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		printf("bind failed: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	// 监听客户端连接请求
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("服务端已启动...\n");

	// 等待客户端连接请求
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);

	clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);
	if (clientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// 输出客户端地址信息
	printf("客户端 %s:%d 已连接...\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	// 发送欢迎消息给客户端
	const char* welcomeMsg = "欢迎来到socket通讯世界！";
	iResult = send(clientSocket, welcomeMsg, strlen(welcomeMsg), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	system("pause");
	// 关闭客户端连接
	closesocket(clientSocket);

	// 清理Winsock库
	WSACleanup();

	return 0;
}
