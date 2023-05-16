#include "source.h"

struct datas dt;//전역변수
struct tcpdata_server g_server;

void graphic() //그래픽구현
{
	// □기본판 번호0, ○흑돌 번호1, ●백돌 번호2, (※)가능위치 번호3
	system("cls");

	printf("검은돌 : %d", dt.int_blackscore);
	printf(" / 흰돌 : %d\n", dt.int_whitescore);

	printf("   ");
	for (int ii = 0; ii < COLS; ii++)
	{
		if (ii + 1 < 10)
			printf(" ");
		printf("%d", ii + 1);
	}

	printf("\n");

	for (int ii = 0; ii < ROWS; ii++)
	{
		if (ii + 1 < 10)
			printf(" ");
		printf("%d ", ii + 1);
		for (int jj = 0; jj < COLS; jj++)
		{
			if (dt.int_grid[ii][jj] == 0)
				printf("□");
			if (dt.int_grid[ii][jj] == CLIENT)
				printf("○");
			if (dt.int_grid[ii][jj] == SERVER)
				printf("●");
			if (dt.int_grid[ii][jj] == 3)
				printf("※");
		}
		if (ii + 1 == dt.int_x)
			printf("◀");
		printf("\n");
	}
	printf(" ");
	for (int ii = 0; ii < dt.int_y; ii++)
		printf("  ");
	printf("▲");
	printf("\n");
}

void lose() //패배그래픽
{
	//대충 슬픈 그림
	printf("YOU ARE LOSER!");
}

void win() //승리그래픽
{
	//대충 기쁜 그림
	printf("YOU ARE WINNER!");
}

void draw() //패배그래픽
{
	//대충 슬픈 그림
	printf("EVERYONE IS LOSER!");
}

void enablechecking() //둘수있는위치 체크
{
	int add;
	int addedx, addedy;
	int i, j, k, l; //I, J, K, L
	dt.int_enablechecking = 0;// int_enablechecking 변수 0으로 초기화

	for (int h = 0; h < ROWS; h++)
	{
		for (int y = 0; y < COLS; y++)
		{
			if (dt.int_grid[h][y] == 3)
				dt.int_grid[h][y] = 0;
		}
	}
	for (i = 0; i < ROWS; i++) // 2중 for문 i 0~ROWS-1, j 0~COLS-1
	{
		for (j = 0; j < COLS; j++)
		{
			if (dt.int_grid[i][j] == SERVER)
			{
				if (dt.int_grid[i][j] == 3)
					dt.int_grid[i][j] = 0;
				for (k = -1; k <= 1; k++) // 2중 for문 k -1~1, l -1~1
				{
					for (l = -1; l <= 1; l++)
					{
						if (dt.int_grid[i + k][j + l] == CLIENT) // 인접한 8방향으로 상대돌이 있는지 확인
						{
							add = 1;
							while (1)
							{
								addedx = i + k * add;
								addedy = j + l * add;
								if (addedy < 0 || addedy == COLS)
									break;
								if (dt.int_grid[addedx][addedy] == 0)
								{
									dt.int_grid[addedx][addedy] = 3;
									dt.int_enablechecking++;
									break;
								}
								else if (dt.int_grid[addedx][addedy] == CLIENT)
								{
									add++;
									continue;
								}
								else
									break;
							}
						}
					}

				}
			}
		}
	}

	if (dt.int_enablechecking == 0) //둘곳없음
	{
		//x = ROWS, y=COLS값 전송
		if (dt.int_x == ROWS && dt.int_y == COLS) //상대도 둘 곳 없음
			return; //함수탈출

		//dt.int_myturn = SERVER; //턴종료(int_myturn)
	}
	else if (dt.int_enablechecking != 0) //둘곳있음
	{
		graphic();
	}
}

void gameovercheck() //경기종료
{
	//이중for문으로 int_grid의 모든 값이 0이 아니라면
	//int_blackscore, int_whitescore값 비교
	for (int ii = 0; ii < ROWS; ii++)
	{
		for (int jj = 0; jj < COLS; jj++)
		{
			if (dt.int_grid[ii][jj] == 0)
				return;
		}
	}
	if (dt.int_blackscore > dt.int_whitescore)//이기면
		win();
	if (dt.int_blackscore < dt.int_whitescore)//지면
		lose();
	if (dt.int_blackscore == dt.int_whitescore)//지면
		draw();
}


void server_reverse() //뒤집기
{
	// int_grid[int_x][int_y] 부터 8방향으로 상대돌 확인
	// 상대돌이 있다면 해당방향으로 끝에 자신돌 / 벽 / 빈칸 확인
	// 끝에 자신돌이면 좌표 되돌아가면서 해당 좌표 정수값 자신돌값으로 변경
	// 8방향 전부 다 변경후 함수종료
	int add;
	int addedx, addedy;
	int k, l; //K, L

	for (k = -1; k <= 1; k++) // 2중 for문 k -1~1, l -1~1
	{
		for (l = -1; l <= 1; l++)
		{
			if (k == 0 && l == 0)
				continue;
			if (dt.int_grid[dt.int_x - 1 + k][dt.int_y - 1 + l] == CLIENT) // 인접한 8방향으로 상대돌이 있는지 확인
			{
				add = 1;
				while (1) //돌둔곳부터 진행
				{
					addedx = dt.int_x - 1 + k * add;
					addedy = dt.int_y - 1 + l * add;
					if (addedy < 0 || addedy >= COLS) //벽
					{	
						add = 0;
						break;
					}
					else if (dt.int_grid[addedx][addedy] == CLIENT) //상대돌이 계속 이어질때
					{
						add++;
						continue;
					}
					else if (dt.int_grid[addedx][addedy] == SERVER) //내돌이 끝에 나왔을때
					{
						break;
					}
					else if (dt.int_grid[addedx][addedy] == 0 || dt.int_grid[addedx][addedy] == 3)
					{
						add = 0;
						break;
					}
				}
				while (1)
				{
					if (add == 0)
						break;
					addedx = dt.int_x - 1 + k * add;
					addedy = dt.int_y - 1 + l * add;
					dt.int_grid[addedx][addedy] = SERVER;

					add--;
					if (add == 0)
						break;
					
					
				}
			}
		}
	}
}

void client_reverse() //뒤집기
{
	// int_grid[int_x][int_y] 부터 8방향으로 상대돌 확인
	// 상대돌이 있다면 해당방향으로 끝에 자신돌 / 벽 / 빈칸 확인
	// 끝에 자신돌이면 좌표 되돌아가면서 해당 좌표 정수값 자신돌값으로 변경
	// 8방향 전부 다 변경후 함수종료
	int add;
	int addedx, addedy;
	int k, l; //K, L

	for (k = -1; k <= 1; k++) // 2중 for문 k -1~1, l -1~1
	{
		for (l = -1; l <= 1; l++)
		{
			if (dt.int_grid[dt.int_x - 1 + k][dt.int_y - 1 + l] == SERVER) // 인접한 8방향으로 상대돌이 있는지 확인
			{
				add = 1;
				while (1) //돌둔곳부터 진행
				{
					addedx = dt.int_x - 1 + k * add;
					addedy = dt.int_y - 1 + l * add;
					if (addedy < 0 || addedy >= COLS) //벽
					{
						add = 0;
						break;
					}
					else if (dt.int_grid[addedx][addedy] == SERVER) //상대돌이 계속 이어질때
					{
						add++;
						continue;
					}
					else if (dt.int_grid[addedx][addedy] == CLIENT) //내돌이 끝에 나왔을때
					{
						break;
					}
					else if (dt.int_grid[addedx][addedy] == 0 || dt.int_grid[addedx][addedy] == 3)
					{
						add = 0;
						break;
					}
				}
				while (1)
				{
					if (add == 0)
						break;
					addedx = dt.int_x - 1 + k * add;
					addedy = dt.int_y - 1 + l * add;
					dt.int_grid[addedx][addedy] = CLIENT;

					add--;
					if (add == 0)
						break;
				}
			}
		}
	}
}

void countingscore() //점수계산
{
	// 2중 for문 i 0~ROWS, j 0~COLS
	// int_grid[0][0] 부터 grid[ROWS][COLS]까지 확인
	// int_grid[i][j]값이 1이면 blackscore +1, 2이면 whitescore +1
	dt.int_blackscore = 0;
	dt.int_whitescore = 0;
	for (int ii = 0; ii < ROWS; ii++)
	{
		for (int jj = 0; jj < COLS; jj++)
		{
			if (dt.int_grid[ii][jj] == CLIENT)
				dt.int_blackscore++;
			if (dt.int_grid[ii][jj] == SERVER)
				dt.int_whitescore++;
		}
	}
}

void typing() //키입력 확인
{
	//if(kbhit()) //키보드 방향키, 돌 두기 z (한번만 물러줭 잉잉 x)
	//상하 int_x+- 좌우 int_y+-
	while (1) // 조작파트
	{
		if (kbhit())
		{
			dt.int_key = getch();
			if (dt.int_key == 224)
			{
				dt.int_key = getch();
				switch (dt.int_key)
				{
				case 80: //상
					dt.int_x++;
					if (dt.int_x >= ROWS)
						dt.int_x = ROWS;
					break;
				case 72: //하
					dt.int_x--;
					if (dt.int_x < 1)
						dt.int_x = 1;
					break;
				case 77: //우
					dt.int_y++;
					if (dt.int_y >= ROWS)
						dt.int_y = ROWS;
					break;
				case 75: //좌
					dt.int_y--;
					if (dt.int_y < 1)
						dt.int_y = 1;
					break;
				}
			}
			else if (dt.int_key == 'z' || dt.int_key == 'Z')
				break;
			graphic();
		}
	}
	graphic();//키 입력시마다 변화 반영
}

main() //뭐넣지 그러게
{
	first_connect();//서버관련내용
	dt.int_grid[3][4] = CLIENT; //기본세팅
	dt.int_grid[4][3] = CLIENT;
	dt.int_grid[4][4] = SERVER;
	dt.int_grid[3][3] = SERVER; //기본세팅
	dt.gamestate = 1;
	dt.int_x = 1;
	dt.int_y = 1;
	dt.int_blackscore = 2;
	dt.int_whitescore = 2;
	dt.int_myturn = CLIENT; //서버는 2로 시작
	graphic();

	while (1)
	{
		if (dt.int_myturn == SERVER) //클라 흑돌 턴
		{
			enablechecking();
			graphic();
			typing();


			if (dt.int_key == 'z')
			{
				if (dt.int_grid[dt.int_x - 1][dt.int_y - 1] == 3) //둘 수 있는곳에 돌 놓기
				{
					dt.int_grid[dt.int_x - 1][dt.int_y - 1] = SERVER; //클라1, 서버2
					server_reverse(); //int_grid 정수값들 대격변
					countingscore(); //점수계산
					graphic(); //화면갱신
					out_gamedata();//int_x, int_y 값 소켓통신 전송
					dt.int_myturn = CLIENT;
				}
				else
					continue;
			}
		}
		else if (dt.int_myturn == CLIENT) //클라 백돌 턴
		{
			get_gamedata();//TCP통신 받는코드
			printf("%d %d", dt.int_x, dt.int_y);
			//받은 문자열(예 "3 5")변수 split해서 문자열"3","5"로분리
			//atoi로 각각 int_x, int_y에 저장
			if (dt.int_x == ROWS && dt.int_y == COLS)
			{
				enablechecking();
				countingscore(); //점수계산

				if (dt.int_blackscore > dt.int_whitescore)//이기면
					win();
				if (dt.int_blackscore < dt.int_whitescore)//지면
					lose();
				if (dt.int_blackscore == dt.int_whitescore)//지면
					draw();
			}
			dt.int_grid[dt.int_x-1][dt.int_y-1] = CLIENT; //클라1, 서버2
			graphic();
			client_reverse();
			countingscore(); //점수계산
			graphic();
			dt.int_myturn = SERVER;
		}
		gameovercheck();
	}
}