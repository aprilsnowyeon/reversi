#include "source.h"

struct datas dt;//��������
struct tcpdata_server g_server;

void graphic() //�׷��ȱ���
{
	// ��⺻�� ��ȣ0, ���浹 ��ȣ1, �ܹ鵹 ��ȣ2, (��)������ġ ��ȣ3
	system("cls");

	printf("������ : %d", dt.int_blackscore);
	printf(" / �� : %d\n", dt.int_whitescore);

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
				printf("��");
			if (dt.int_grid[ii][jj] == CLIENT)
				printf("��");
			if (dt.int_grid[ii][jj] == SERVER)
				printf("��");
			if (dt.int_grid[ii][jj] == 3)
				printf("��");
		}
		if (ii + 1 == dt.int_x)
			printf("��");
		printf("\n");
	}
	printf(" ");
	for (int ii = 0; ii < dt.int_y; ii++)
		printf("  ");
	printf("��");
	printf("\n");
}

void lose() //�й�׷���
{
	//���� ���� �׸�
	printf("YOU ARE LOSER!");
}

void win() //�¸��׷���
{
	//���� ��� �׸�
	printf("YOU ARE WINNER!");
}

void draw() //�й�׷���
{
	//���� ���� �׸�
	printf("EVERYONE IS LOSER!");
}

void enablechecking() //�Ѽ��ִ���ġ üũ
{
	int add;
	int addedx, addedy;
	int i, j, k, l; //I, J, K, L
	dt.int_enablechecking = 0;// int_enablechecking ���� 0���� �ʱ�ȭ

	for (int h = 0; h < ROWS; h++)
	{
		for (int y = 0; y < COLS; y++)
		{
			if (dt.int_grid[h][y] == 3)
				dt.int_grid[h][y] = 0;
		}
	}
	for (i = 0; i < ROWS; i++) // 2�� for�� i 0~ROWS-1, j 0~COLS-1
	{
		for (j = 0; j < COLS; j++)
		{
			if (dt.int_grid[i][j] == SERVER)
			{
				if (dt.int_grid[i][j] == 3)
					dt.int_grid[i][j] = 0;
				for (k = -1; k <= 1; k++) // 2�� for�� k -1~1, l -1~1
				{
					for (l = -1; l <= 1; l++)
					{
						if (dt.int_grid[i + k][j + l] == CLIENT) // ������ 8�������� ��뵹�� �ִ��� Ȯ��
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

	if (dt.int_enablechecking == 0) //�Ѱ�����
	{
		//x = ROWS, y=COLS�� ����
		if (dt.int_x == ROWS && dt.int_y == COLS) //��뵵 �� �� ����
			return; //�Լ�Ż��

		//dt.int_myturn = SERVER; //������(int_myturn)
	}
	else if (dt.int_enablechecking != 0) //�Ѱ�����
	{
		graphic();
	}
}

void gameovercheck() //�������
{
	//����for������ int_grid�� ��� ���� 0�� �ƴ϶��
	//int_blackscore, int_whitescore�� ��
	for (int ii = 0; ii < ROWS; ii++)
	{
		for (int jj = 0; jj < COLS; jj++)
		{
			if (dt.int_grid[ii][jj] == 0)
				return;
		}
	}
	if (dt.int_blackscore > dt.int_whitescore)//�̱��
		win();
	if (dt.int_blackscore < dt.int_whitescore)//����
		lose();
	if (dt.int_blackscore == dt.int_whitescore)//����
		draw();
}


void server_reverse() //������
{
	// int_grid[int_x][int_y] ���� 8�������� ��뵹 Ȯ��
	// ��뵹�� �ִٸ� �ش�������� ���� �ڽŵ� / �� / ��ĭ Ȯ��
	// ���� �ڽŵ��̸� ��ǥ �ǵ��ư��鼭 �ش� ��ǥ ������ �ڽŵ������� ����
	// 8���� ���� �� ������ �Լ�����
	int add;
	int addedx, addedy;
	int k, l; //K, L

	for (k = -1; k <= 1; k++) // 2�� for�� k -1~1, l -1~1
	{
		for (l = -1; l <= 1; l++)
		{
			if (k == 0 && l == 0)
				continue;
			if (dt.int_grid[dt.int_x - 1 + k][dt.int_y - 1 + l] == CLIENT) // ������ 8�������� ��뵹�� �ִ��� Ȯ��
			{
				add = 1;
				while (1) //���а����� ����
				{
					addedx = dt.int_x - 1 + k * add;
					addedy = dt.int_y - 1 + l * add;
					if (addedy < 0 || addedy >= COLS) //��
					{	
						add = 0;
						break;
					}
					else if (dt.int_grid[addedx][addedy] == CLIENT) //��뵹�� ��� �̾�����
					{
						add++;
						continue;
					}
					else if (dt.int_grid[addedx][addedy] == SERVER) //������ ���� ��������
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

void client_reverse() //������
{
	// int_grid[int_x][int_y] ���� 8�������� ��뵹 Ȯ��
	// ��뵹�� �ִٸ� �ش�������� ���� �ڽŵ� / �� / ��ĭ Ȯ��
	// ���� �ڽŵ��̸� ��ǥ �ǵ��ư��鼭 �ش� ��ǥ ������ �ڽŵ������� ����
	// 8���� ���� �� ������ �Լ�����
	int add;
	int addedx, addedy;
	int k, l; //K, L

	for (k = -1; k <= 1; k++) // 2�� for�� k -1~1, l -1~1
	{
		for (l = -1; l <= 1; l++)
		{
			if (dt.int_grid[dt.int_x - 1 + k][dt.int_y - 1 + l] == SERVER) // ������ 8�������� ��뵹�� �ִ��� Ȯ��
			{
				add = 1;
				while (1) //���а����� ����
				{
					addedx = dt.int_x - 1 + k * add;
					addedy = dt.int_y - 1 + l * add;
					if (addedy < 0 || addedy >= COLS) //��
					{
						add = 0;
						break;
					}
					else if (dt.int_grid[addedx][addedy] == SERVER) //��뵹�� ��� �̾�����
					{
						add++;
						continue;
					}
					else if (dt.int_grid[addedx][addedy] == CLIENT) //������ ���� ��������
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

void countingscore() //�������
{
	// 2�� for�� i 0~ROWS, j 0~COLS
	// int_grid[0][0] ���� grid[ROWS][COLS]���� Ȯ��
	// int_grid[i][j]���� 1�̸� blackscore +1, 2�̸� whitescore +1
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

void typing() //Ű�Է� Ȯ��
{
	//if(kbhit()) //Ű���� ����Ű, �� �α� z (�ѹ��� �����a ���� x)
	//���� int_x+- �¿� int_y+-
	while (1) // ������Ʈ
	{
		if (kbhit())
		{
			dt.int_key = getch();
			if (dt.int_key == 224)
			{
				dt.int_key = getch();
				switch (dt.int_key)
				{
				case 80: //��
					dt.int_x++;
					if (dt.int_x >= ROWS)
						dt.int_x = ROWS;
					break;
				case 72: //��
					dt.int_x--;
					if (dt.int_x < 1)
						dt.int_x = 1;
					break;
				case 77: //��
					dt.int_y++;
					if (dt.int_y >= ROWS)
						dt.int_y = ROWS;
					break;
				case 75: //��
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
	graphic();//Ű �Է½ø��� ��ȭ �ݿ�
}

main() //������ �׷���
{
	first_connect();//�������ó���
	dt.int_grid[3][4] = CLIENT; //�⺻����
	dt.int_grid[4][3] = CLIENT;
	dt.int_grid[4][4] = SERVER;
	dt.int_grid[3][3] = SERVER; //�⺻����
	dt.gamestate = 1;
	dt.int_x = 1;
	dt.int_y = 1;
	dt.int_blackscore = 2;
	dt.int_whitescore = 2;
	dt.int_myturn = CLIENT; //������ 2�� ����
	graphic();

	while (1)
	{
		if (dt.int_myturn == SERVER) //Ŭ�� �浹 ��
		{
			enablechecking();
			graphic();
			typing();


			if (dt.int_key == 'z')
			{
				if (dt.int_grid[dt.int_x - 1][dt.int_y - 1] == 3) //�� �� �ִ°��� �� ����
				{
					dt.int_grid[dt.int_x - 1][dt.int_y - 1] = SERVER; //Ŭ��1, ����2
					server_reverse(); //int_grid �������� ��ݺ�
					countingscore(); //�������
					graphic(); //ȭ�鰻��
					out_gamedata();//int_x, int_y �� ������� ����
					dt.int_myturn = CLIENT;
				}
				else
					continue;
			}
		}
		else if (dt.int_myturn == CLIENT) //Ŭ�� �鵹 ��
		{
			get_gamedata();//TCP��� �޴��ڵ�
			printf("%d %d", dt.int_x, dt.int_y);
			//���� ���ڿ�(�� "3 5")���� split�ؼ� ���ڿ�"3","5"�κи�
			//atoi�� ���� int_x, int_y�� ����
			if (dt.int_x == ROWS && dt.int_y == COLS)
			{
				enablechecking();
				countingscore(); //�������

				if (dt.int_blackscore > dt.int_whitescore)//�̱��
					win();
				if (dt.int_blackscore < dt.int_whitescore)//����
					lose();
				if (dt.int_blackscore == dt.int_whitescore)//����
					draw();
			}
			dt.int_grid[dt.int_x-1][dt.int_y-1] = CLIENT; //Ŭ��1, ����2
			graphic();
			client_reverse();
			countingscore(); //�������
			graphic();
			dt.int_myturn = SERVER;
		}
		gameovercheck();
	}
}