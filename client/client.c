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


void first_connect()   // ó�� tcp��� ���� - Ŭ���̾�Ʈ
{
    // 1. ���� ���̺귯�� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &g_client.wsadata) != 0) {
        return -1;
    }
    // 
    // 2. ���� ����
    g_client.clientSocket2 = socket(AF_INET, SOCK_STREAM, 0);
    if (g_client.clientSocket2 == INVALID_SOCKET) {
        ErrorQuit("socket()");
    }

    g_client.serverAddress2.sin_family = AF_INET;
    g_client.serverAddress2.sin_addr.s_addr = inet_addr("192.168.0.29");
    g_client.serverAddress2.sin_port = htons(9000);
    // 3. ������ �����û
    g_client.retval = connect(g_client.clientSocket2, (SOCKADDR*)&g_client.serverAddress2, sizeof(g_client.serverAddress2));
    if (g_client.retval == SOCKET_ERROR) {
        ErrorQuit("connect()");
    }
}

void out_gamedata()  // ���� ���� �� ���� �ξ��� ��ǥ ������ ������
{
    // 1. ������ ������ ���
    g_client.stringLength2 = sizeof(g_client.message2);
    char temp[10];
    strcpy(g_client.message2, _itoa(dt.int_x, temp, 10));
    strcat(g_client.message2, _itoa(dt.int_y, temp, 10));
    send(g_client.clientSocket2, g_client.message2, g_client.stringLength2, 0); // data ������ 
    strcpy(g_client.message2, "\0");
    strcpy(g_client.data2, "\0");
}

void get_gamedata()  // ���� �� ���� �ΰ� �˰��� ������ �������� ������ �� ������ ���
{


    // 1. Ŭ���̾�Ʈ�� ��ǥ�� ȹ��
    g_client.stringLength2 = recv(g_client.clientSocket2, g_client.message2, BUFSIZE, 0);

    memset(g_client.data2, 0, BUFSIZE);
    memcpy(g_client.data2, g_client.message2, g_client.stringLength2);

    // 2. ��ǥ���� ������ x����, y������ ��ȯ
    // 3. ������ ��ȯ�� �� ����
    memset(g_client.data2, 0, BUFSIZE);
    memcpy(g_client.data2, g_client.message2, g_client.stringLength2);
    dt.int_x = atoi(g_client.message2) / 10;   // ���ڷ� ���� x�� int �� ��ȯ
    dt.int_y = atoi(g_client.message2) % 10;   // ���ڷ� ���� y�� int �� ��ȯ

}

void endsocket() // ��� ����
{
    closesocket(g_client.clientSocket2);
    //���� ���̺귯�� ����
    WSACleanup();
}