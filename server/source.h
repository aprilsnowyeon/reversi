#define _CRT_SECURE_NO_WARNINGS
#pragma once
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#define ROWS 8
#define COLS 8
#define CLIENT 1
#define SERVER 2
#define BUFSIZE 512

struct datas {
	int int_x, int_y; //��ǥ ����
	int int_myturn; //�� Ȯ��
	int gamestate; //���ӻ��� ������=0, ������=1, ����=2
	int int_blackscore, int_whitescore; //�浹 �鵹 ���� ����
	int int_enablechecking; //enablechecking�Լ����� ������ �ڸ� ���� Ȯ�ο� ����
	int int_key; //Ű���� �Է°� ����Ǵ� ���� kbhit�� ���
	int int_grid[ROWS][COLS]; //�ٵ��� ��ǥ �������� ����
};

struct tcpdata_server   // tcp ��ſ� ���Ǵ� ����ü 
{
    WSADATA wsaData; // ������
    SOCKET serverSocket, clientSocket; // ���� ����ü
    SOCKADDR_IN serverAddress, clientAddress; // ����,Ŭ���̾�Ʈ �ּ� ����ü
    int stringLength;   // ���� ������ ���� ����
    char message[BUFSIZE]; // ���� �޽��� ����
    char data[BUFSIZE]; // ���� �޽��� ����
    int clientAddressSize; // Ŭ���̾�Ʈ �ּ� ũ�� ����
};