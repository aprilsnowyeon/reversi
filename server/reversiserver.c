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

void first_connect()   // 처음 tcp통신 연결 - 서버
{
	//strcpy(s.message, "\0");
	//strcpy(s.data, "\0");
	// 1. 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &g_server.wsaData) != 0) {
		ErrorHandling("WSAStartup() Error!");
	}
	// 2. 소켓 생성
	g_server.serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	//소켓이 유효하지 않을 경우
	if (g_server.serverSocket == INVALID_SOCKET) {
		ErrorHandling("socket() Error!");
	}
	memset(&g_server.serverAddress, 0, sizeof(g_server.serverAddress));
	g_server.serverAddress.sin_family = AF_INET;
	g_server.serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	g_server.serverAddress.sin_port = htons(9000);
	// 3. IP주소와 포트번호 할당, binding
	if (bind(g_server.serverSocket, (SOCKADDR*)&g_server.serverAddress, sizeof(g_server.serverAddress)) == SOCKET_ERROR) {
		ErrorHandling("bind() Error!");
	}
	// 4. listen 함수를 통해 서버 소켓 완성
	if (listen(g_server.serverSocket, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() Error!");
	}
	g_server.clientAddressSize = sizeof(g_server.clientAddress);
	// 5. while 문으로 연결요청 받을때까지 대기 및 연결시 수락
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

void out_gamedata()  // 게임 시작 후 돌을 두었던 좌표 데이터 보내기
{
	// 1. 클라이언트로 데이터 출력
	char temp[10];
	strcpy(g_server.message, _itoa(dt.int_x, temp, 10));
	strcat(g_server.message, _itoa(dt.int_y, temp, 10));
	send(g_server.clientSocket, g_server.message, g_server.stringLength, 0); // data 보내기
	strcpy(g_server.message, "\0");
	strcpy(g_server.data, "\0");
}

void get_gamedata()  // 게임 중 돌을 두고 알고리즘 수행후 클라이언트에서 데이터 올 때까지 대기
{


	// 1. while 문으로 클라이언트의 좌표값 받을때까지 대기
	while ((g_server.stringLength = recv(g_server.clientSocket, g_server.message, BUFSIZE, 0)) != 2)
	{
		if (strlen(g_server.message) == 2) {
			break;
		}
	}
	memset(g_server.data, 0, BUFSIZE);
	memcpy(g_server.data, g_server.message, g_server.stringLength);

	// 2. 좌표값을 받으면 x정수, y정수로 변환
	// 3. 정수로 변환한 값 갱신
	memset(g_server.data, 0, BUFSIZE);
	memcpy(g_server.data, g_server.message, g_server.stringLength);
	dt.int_x = atoi(g_server.message) / 10;   // 문자로 받은 x값 int 형 변환
	dt.int_y = atoi(g_server.message) % 10;   // 문자로 받은 y값 int 형 변환

}

void endsocket() // 통신 종료
{
	closesocket(g_server.clientSocket);
	closesocket(g_server.serverSocket);
	//소켓 라이브러리 해제
	WSACleanup();
}
