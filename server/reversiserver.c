#include "source.h"
extern struct datas dt;
extern struct tcpdata_server g_server;

void ErrorDisplay(char* message) {

	LPVOID lpMessageBuffer;
	FormatMessage(

		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMessageBuffer, 0, NULL);
	printf("[%s] %s", message, (LPTSTR)lpMessageBuffer);
	LocalFree(lpMessageBuffer);

}

void ErrorQuit(char* message) {

	LPVOID lpMessageBuffer;
	FormatMessage(

		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMessageBuffer, 0, NULL);

	MessageBox(NULL, (LPTSTR)lpMessageBuffer, message, MB_ICONERROR);
	LocalFree(lpMessageBuffer);

	exit(-1);
}

void ErrorHandling(char* message) {

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);

}

void first_connect()   // ó�� tcp��� ���� - ����
{
	//strcpy(s.message, "\0");
	//strcpy(s.data, "\0");
	// 1. ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &g_server.wsaData) != 0) {
		ErrorHandling("WSAStartup() Error!");
	}
	// 2. ���� ����
	g_server.serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	//������ ��ȿ���� ���� ���
	if (g_server.serverSocket == INVALID_SOCKET) {
		ErrorHandling("socket() Error!");
	}
	memset(&g_server.serverAddress, 0, sizeof(g_server.serverAddress));
	g_server.serverAddress.sin_family = AF_INET;
	g_server.serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	g_server.serverAddress.sin_port = htons(9000);
	// 3. IP�ּҿ� ��Ʈ��ȣ �Ҵ�, binding
	if (bind(g_server.serverSocket, (SOCKADDR*)&g_server.serverAddress, sizeof(g_server.serverAddress)) == SOCKET_ERROR) {
		ErrorHandling("bind() Error!");
	}
	// 4. listen �Լ��� ���� ���� ���� �ϼ�
	if (listen(g_server.serverSocket, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() Error!");
	}
	g_server.clientAddressSize = sizeof(g_server.clientAddress);
	// 5. while ������ �����û ���������� ��� �� ����� ����
	while (1)
	{
		g_server.clientSocket = accept(g_server.serverSocket, (SOCKADDR*)&g_server.clientAddress, &g_server.clientAddressSize);

		if (g_server.clientSocket == -1) {
			ErrorHandling("accept() Error!");
		}
		else {
			printf("Accept client \n");
			break;
		}
	}
	g_server.stringLength = strlen(g_server.message);
}

void out_gamedata()  // ���� ���� �� ���� �ξ��� ��ǥ ������ ������
{
	// 1. Ŭ���̾�Ʈ�� ������ ���
	char temp[10];
	strcpy(g_server.message, _itoa(dt.int_x, temp, 10));
	strcat(g_server.message, _itoa(dt.int_y, temp, 10));
	send(g_server.clientSocket, g_server.message, g_server.stringLength, 0); // data ������
	strcpy(g_server.message, "\0");
	strcpy(g_server.data, "\0");
}

void get_gamedata()  // ���� �� ���� �ΰ� �˰��� ������ Ŭ���̾�Ʈ���� ������ �� ������ ���
{


	// 1. while ������ Ŭ���̾�Ʈ�� ��ǥ�� ���������� ���
	while ((g_server.stringLength = recv(g_server.clientSocket, g_server.message, BUFSIZE, 0)) != 2)
	{
		if (strlen(g_server.message) == 2) {
			break;
		}
	}
	memset(g_server.data, 0, BUFSIZE);
	memcpy(g_server.data, g_server.message, g_server.stringLength);

	// 2. ��ǥ���� ������ x����, y������ ��ȯ
	// 3. ������ ��ȯ�� �� ����
	memset(g_server.data, 0, BUFSIZE);
	memcpy(g_server.data, g_server.message, g_server.stringLength);
	dt.int_x = atoi(g_server.message) / 10;   // ���ڷ� ���� x�� int �� ��ȯ
	dt.int_y = atoi(g_server.message) % 10;   // ���ڷ� ���� y�� int �� ��ȯ

}

void endsocket() // ��� ����
{
	closesocket(g_server.clientSocket);
	closesocket(g_server.serverSocket);
	//���� ���̺귯�� ����
	WSACleanup();
}
