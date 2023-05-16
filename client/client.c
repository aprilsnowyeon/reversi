#include "source.h"
extern struct datas dt;
extern struct tcpdata_client g_client;

int recvn(SOCKET s, char* buf, int length, int flags) {

    int received;
    char* ptr = buf;
    int left = length;

    while (left > 0) {

        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR) {
            return SOCKET_ERROR;
        }
        else if (received == 0) {
            break;
        }

        left -= received;
        ptr += received;
    }

    return (length - left);
}

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


void first_connect()   // 처음 tcp통신 연결 - 클라이언트
{
    // 1. 소켓 라이브러리 초기화
    if (WSAStartup(MAKEWORD(2, 2), &g_client.wsadata) != 0) {
        return -1;
    }
    // 
    // 2. 소켓 생성
    g_client.clientSocket2 = socket(AF_INET, SOCK_STREAM, 0);
    if (g_client.clientSocket2 == INVALID_SOCKET) {
        ErrorQuit("socket()");
    }

    g_client.serverAddress2.sin_family = AF_INET;
    g_client.serverAddress2.sin_addr.s_addr = inet_addr("192.168.0.29");
    g_client.serverAddress2.sin_port = htons(9000);
    // 3. 서버에 연결요청
    g_client.retval = connect(g_client.clientSocket2, (SOCKADDR*)&g_client.serverAddress2, sizeof(g_client.serverAddress2));
    if (g_client.retval == SOCKET_ERROR) {
        ErrorQuit("connect()");
    }
}

void out_gamedata()  // 게임 시작 후 돌을 두었던 좌표 데이터 보내기
{
    // 1. 서버로 데이터 출력
    g_client.stringLength2 = sizeof(g_client.message2);
    char temp[10];
    strcpy(g_client.message2, _itoa(dt.int_x, temp, 10));
    strcat(g_client.message2, _itoa(dt.int_y, temp, 10));
    send(g_client.clientSocket2, g_client.message2, g_client.stringLength2, 0); // data 보내기 
    strcpy(g_client.message2, "\0");
    strcpy(g_client.data2, "\0");
}

void get_gamedata()  // 게임 중 돌을 두고 알고리즘 수행후 서버에서 데이터 올 때까지 대기
{


    // 1. 클라이언트의 좌표값 획득
    g_client.stringLength2 = recv(g_client.clientSocket2, g_client.message2, BUFSIZE, 0);

    memset(g_client.data2, 0, BUFSIZE);
    memcpy(g_client.data2, g_client.message2, g_client.stringLength2);

    // 2. 좌표값을 받으면 x정수, y정수로 변환
    // 3. 정수로 변환한 값 갱신
    memset(g_client.data2, 0, BUFSIZE);
    memcpy(g_client.data2, g_client.message2, g_client.stringLength2);
    dt.int_x = atoi(g_client.message2) / 10;   // 문자로 받은 x값 int 형 변환
    dt.int_y = atoi(g_client.message2) % 10;   // 문자로 받은 y값 int 형 변환

}

void endsocket() // 통신 종료
{
    closesocket(g_client.clientSocket2);
    //소켓 라이브러리 해제
    WSACleanup();
}