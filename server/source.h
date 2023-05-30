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
	int int_x, int_y; //좌표 변수
	int int_myturn; //턴 확인
	int gamestate; //게임상태 시작전=0, 진행중=1, 종료=2
	int int_blackscore, int_whitescore; //흑돌 백돌 점수 변수
	int int_enablechecking; //enablechecking함수에서 가능한 자리 갯수 확인용 변수
	int int_key; //키보드 입력값 저장되는 변수 kbhit때 사용
	int int_grid[ROWS][COLS]; //바둑판 좌표 상태저장 변수
};

struct tcpdata_server   // tcp 통신에 사용되는 구조체 
{
    WSADATA wsaData; // 윈소켓
    SOCKET serverSocket, clientSocket; // 소켓 구조체
    SOCKADDR_IN serverAddress, clientAddress; // 서버,클라이언트 주소 구조체
    int stringLength;   // 받은 데이터 길이 저장
    char message[BUFSIZE]; // 보낼 메시지 저장
    char data[BUFSIZE]; // 받은 메시지 저장
    int clientAddressSize; // 클라이언트 주소 크기 저장
};