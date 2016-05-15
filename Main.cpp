//THE GAME:扫雷.cpp 
#include<iostream>
#include<string>
#include<set>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<windows.h>

#define KEY_UP    0xE048
#define KEY_DOWN  0xE050
#define KEY_LEFT  0xE04B
#define KEY_RIGHT 0xE04D
#define KEY_ESC   0x001B
#define KEY_0     '0'
#define KEY_1     '1'
#define KEY_2     '2'
#define KEY_3     '3'
#define KEY_4     '4'
#define MAX_N     100 
using namespace std;
//Fuctions  declaration
//--------------------------------------------------------------------
/*Produce minefield and mark the number*/
void MineGenerator(char*&, const int, const int, const int);
/*Enter the position return the result of the click*/
bool MineFinder(char*&, const int, const int, const int, const int);
/*take control of the game*/
void MineControl(char*, int&, int&, int);
/*Initialize the map with ch*/
void mem_set(char*&, char, const int);
/*Print the current map*/
void print(char*, const int, const int);
/*move the handle to (x,y)*/
int move(int, int);
/*whether meet the winning conditions*/
bool surplus(int, int, int);
/*get the key imput*/
int Getkey();
/*show game rules message*/
void message1();
void message2();
/*global variable*/
char vis[MAX_N][MAX_N];
char check[MAX_N][MAX_N];
int count_n = 0;
//Fuctions  Codes
//--------------------------------------------------------------------
void MineGenerator(char*&a, const int m, const int n, const int k)
{
	/*Initialize the map with no mine*/
	mem_set(a, '0', m*n);
	/*Produce minefield stored in STL set*/
	srand(int(time(0)));
	set<int>st;
	while (st.size() != k)
		st.insert(rand() % (m*n));
	/*Produce minefield graph*/
	while (!st.empty())
	{
		int x = *st.begin() / n, y = *st.begin() % n;
		*(a + n*x + y) = '*';
		st.erase(st.begin());
		/*Tag minefield digits*/
		for (int i = x - 1; i <= x + 1; i++)
			for (int j = y - 1; j <= y + 1; j++)
				if (i >= 0 && i<m&&j >= 0 && j<n&&*(a + n*i + j) != '*')
					(*(a + n*i + j))++;
	}
	for (int i = 0; i<m; i++)
		for (int j = 0; j<n; j++)
			if (*(a + n*i + j) == '0')
				*(a + n*i + j) = '.';
	return;
}

bool MineFinder(char*&a, const int m, const int n, const int x, const int y)
{
	if (check[x][y] == '1') {
		check[x][y] = '0';
		count_n--;
	}
	move(x, y);
	/*if x,y is not allowed*/
	if (x<0 || x >= m || y<0 || y >= n || *(a + n*x + y) == ' ')
		return 1;
	/*if is boom*/
	if (*(a + n*x + y) == '*') {
		cout << '\a';
		return 0;
	}
	/*if is num*/
	if (*(a + n*x + y)>'0'&&*(a + n*x + y)<'9') {
		printf("%c", *(a + n*x + y));
		vis[x][y] = '1';
		return 1;
	}
	/*if is a blank*/
	vis[x][y] = '1';
	*(a + n*x + y) = ' ';
	printf(" ");
	/*dfs the others*/
	MineFinder(a, m, n, x - 1, y);
	MineFinder(a, m, n, x + 1, y);
	MineFinder(a, m, n, x, y + 1);
	MineFinder(a, m, n, x, y - 1);
	return 1;
}

void MineControl(char*a, int &m, int &n, int k)
{
	/*x,y is the position and sum is the booms*/
	int x = 0, y = 0, sum = 0;
	count_n = 0;
	int in;
	bool end = 0;
	memset(vis, '0', sizeof(vis));
	memset(check, '0', sizeof(check));
	/*return the time*/
	clock_t start, finish;
	double   duration;
	bool first = 1, wrong = 0;
	/*control the imput keys*/
	while (!((sum == k&&!wrong) || end))
	{
		move(m, 0);
		printf("\n\t\t\t\t\t\t\t\t\t\r");
		printf("总地雷数：%d ，已标记：%d", k, count_n);
		wrong = 0;
		move(x, y);
		fflush(stdin);
		in = Getkey();
		if (first)
			start = clock(), first = 0;
		switch (in)
		{
		case KEY_UP: {
			if (x>0)
				x--;
			break;
		}
		case KEY_DOWN: {
			if (x<m - 1)
				x++;
			break;
		}
		case KEY_LEFT: {
			if (y>0)
				y--;
			break;
		}
		case KEY_RIGHT: {
			if (y<n - 1)
				y++;
			break;
		}
		case KEY_ESC: {
			m = EOF;
			n = EOF;
			return;
		}
		case KEY_1: {
			if (vis[x][y] == '1')
				break;
			if (MineFinder(a, m, n, x, y)) {
				if (vis[x][y] == '2')
					vis[x][y] = '1';
				break;
			}
			else {
				move(0, 0);
				print(a, m, n);
				end = 1;
				break;
			}
			break;
		}
		case KEY_2: {
			if (vis[x][y] == '1')
				break;
			if (vis[x][y] == '0') {
				printf("#");
				if (*(a + n*x + y) == '*'&&vis[x][y] != '3') {
					vis[x][y] = '3';
					sum++;
				}
				if (*(a + n*x + y) != '*')
					vis[x][y] = '2';
				check[x][y] = '1';
				count_n++;
				break;
			}
			if (vis[x][y] == '2') {
				printf(".");
				vis[x][y] = '0';
				check[x][y] = '0';
				count_n--;
				break;
			}
			if (vis[x][y] == '3') {
				printf(".");
				vis[x][y] = '0';
				sum--;
				count_n--;
				check[x][y] = '0';
				break;
			}
		}
		}
		/*whether meet the winning conditions,if so,print the map*/
		if (surplus(m, n, k)) {
			move(0, 0);
			for (int i = 0; i<m; i++) {
				for (int j = 0; j<n; j++) {
					if (*(a + n*i + j) == '*')
						*(a + n*i + j) = '#';
					printf("%c", *(a + n*i + j));
				}
				printf("\n");
			}
			break;
		}
		for (int i = 0; i<m&&!wrong; i++)
			for (int j = 0; j<n&&!wrong; j++)
				if (vis[i][j] == '2')
					wrong = 1;
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	move(m, 0);
	for (int i = 0; i <= 5; i++)
		printf("\n\t\t\t\t\t\t\t\t\t");
	move(m + 1, 0);
	if (!end)
		printf("你获胜啦\n*********************\n用时 %.2f seconds\n*********************\n\n", duration);
	else
		printf("GAME OVER\n*********************\n用时 %.2f seconds\n*********************\n\n", duration);
	fflush(stdin);
	return;
}

void mem_set(char*&a, char c, const int size)
{
	for (int i = 0; i<size; i++)
		*(a + i) = c;
	return;
}

void print(char*a, const int m, const int n)
{
	for (int i = 0; i<m; i++) {
		for (int j = 0; j<n; j++)
			printf("%c", *(a + n*i + j));
		printf("\n");
	}
	return;
}

int move(int x, int y)
{
	COORD cd;
	cd.X = y; cd.Y = x;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

bool surplus(int m, int n, int k)
{
	int res = 0;
	for (int i = 0; i<m; i++)
		for (int j = 0; j<n; j++)
			if (vis[i][j] != '1')
				res++;
	/*if the mines not visited are all the boom*/
	if (res == k)
		return 1;
	else
		return 0;
}

int Getkey()
{
	/*return hexadecimal key id*/
	int nkey = _getch(), nk = 0;
	if (nkey >= 128 || nkey == 0)nk = _getch();
	return nk>0 ? nkey * 256 + nk : nkey;
}

void message1()
{
	printf("游戏名称:   扫雷\n");
	printf("设计者:     华南理工大学 计科院 马一宁\n");
	printf("----------------------------------------------------------------\n");
	printf("胜利条件：  标记出所有地雷或者剩余的方块全是地雷即可获胜\n\n");
	printf("游戏玩法：  使用键盘方向键控制光标移动\n");
	printf("            按下数字'1'表示打开方块\n");
	printf("            按下数字'2'表示标记为地雷，再次按下2取消标记\n");
	printf("----------------------------------------------------------------\n");
}

void message2()
{
	printf("游戏玩法：  使用键盘方向键控制光标移动\n");
	printf("            按下数字'1'表示打开方块\n");
	printf("            按下数字'2'表示标记为地雷，再次按下2取消标记\n");
	printf("            按下'ESC'退出游戏\n\n");
}
//MAIN Fuctions
//--------------------------------------------------------------------
int main()
{
	/*the rules of this game*/
	message1();
	/* the size is m * n and number of mines is k */
	int m, n, k;
	char con = '1';
	bool tw0 = 0;
	while (con - 48)
	{
		/*Judge the legitimacy of the input*/
		printf("请按下相应数字选择难度：\n1.简单:5颗雷\n2.普通:10颗雷\n3.困难:50颗雷\n4.自定义\n");
		con = '?';
		while (con != KEY_1&&con != KEY_2&&con != KEY_3&&con != KEY_4)
			con = _getch();
		switch (con - 48)
		{
		case 1: {
			m = 9; n = 9; k = 5;
			break;
		}
		case 2: {
			m = 12; n = 12; k = 10;
			break;
		}
		case 3: {
			m = 12; n = 24; k = 50;
			break;
		}
		case 4: {
			printf("请输入扫雷高度，宽度，地雷个数，以回车结束:\n");
			if (tw0)
				printf("(按方向键可以调出历史输入)\n");
			cin >> m >> n >> k;
			if (m <= 0 || n <= 0 || k >= m*n || m >= MAX_N || n >= MAX_N) {
				cout << "错误，请重新输入\n";
				continue;
			}
			fflush(stdin);
			break;
		}
		}
		con = '?';
		/*Declare a M*N map*/
		char *a = new char[m*n];
		mem_set(a, '.', m*n);
		system("cls");
		print(a, m, n);
		move(m + 3, 0);
		message2();
		MineGenerator(a, m, n, k);
		/*take control*/
		MineControl(a, m, n, k);
		if (m == EOF || n == EOF)
			break;
		/*Recovery the map*/
		delete[]a;
		a = NULL;
		fflush(stdin);
		printf("要继续吗？(1 or 0)\n");
		con = '?';
		while (con != KEY_1&&con != KEY_0)
			con = _getch();
		tw0 = 1;
		system("cls");
	}
	/*end of the game*/
	system("cls");
	printf("下次再见!\n");
	system("pause");
	return 0;
}00
