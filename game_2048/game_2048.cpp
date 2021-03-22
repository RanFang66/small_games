#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <graphics.h>


#define BLOCK_WIDTH		100
#define BLOCK_HEIGHT	BLOCK_WIDTH
#define INTERVAL		15
#define BLOCK_COL_NUM	4
#define BLOCK_ROW_NUM	BLOCK_COL_NUM
#define BLOCK_COLOR		RGB(205, 193, 180)

#define BKG_WIDTH		(BLOCK_WIDTH * BLOCK_COL_NUM + INTERVAL * (BLOCK_COL_NUM+1))
#define BKG_HEIGHT		(BLOCK_HEIGHT * BLOCK_ROW_NUM + INTERVAL * (BLOCK_ROW_NUM+1))
#define BKG_COLOR		RGB(187, 173, 160)

#define ELLIPSE_WIDTH	15
#define ELLIPSE_HEIGHT	15

#define TEXT_HEIGHT		50
#define TEXT_WIDTH		10			//adjust automatically
#define TEXT_COLOR		RGB(163, 73, 164)

#define INIT_NUM		3

#define INDEX_MAX		11

#define COLOR_NUM_VOID	RGB(205, 193, 180)
#define COLOR_NUM_2		RGB(238, 193, 180)
#define COLOR_NUM_4		RGB(237, 224, 200)
#define COLOR_NUM_8		RGB(242, 177, 121)
#define COLOR_NUM_16	RGB(245, 149, 99)
#define COLOR_NUM_32	RGB(246, 124, 95)
#define COLOR_NUM_64	RGB(246, 94, 59)
#define COLOR_NUM_128	RGB(242, 177, 121)
#define COLOR_NUM_256	RGB(237, 204, 97)
#define COLOR_NUM_512	RGB(255, 0, 128)
#define COLOR_NUM_1024  RGB(145, 0, 72)
#define COLOR_NUM_2048	RGB(242, 17, 158)

#define KEY_UP			72
#define KEY_DOWN		80
#define KEY_LEFT		75
#define KEY_RIGHT		77
#define KEY_ESC			27

typedef enum MOVE_ACTION {
	MOVE_INVALID = 0,
	MOVE_UP		 = 1,
	MOVE_DOWN	 = 2,
	MOVE_LEFT	 = 3,
	MOVE_RIGHT   = 4,
} move_t;

const int num_tbl[INDEX_MAX+1] = { 0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };
const COLORREF color_tbl[INDEX_MAX+1] = {
	COLOR_NUM_VOID, 
	COLOR_NUM_2, 
	COLOR_NUM_4,
	COLOR_NUM_8,
	COLOR_NUM_16,
	COLOR_NUM_32,
	COLOR_NUM_64,
	COLOR_NUM_128,
	COLOR_NUM_256,
	COLOR_NUM_512,
	COLOR_NUM_1024,
	COLOR_NUM_2048,
};
int map[BLOCK_ROW_NUM][BLOCK_COL_NUM] = { 0 };

void init_map(int col, int row)
{
	int r;
	int c;
	int num_index;
	int i = 0;


	map[0][0] = 3;
	map[0][1] = 4;
	map[0][2] = 5;
	while (i < INIT_NUM) {
		r = rand() % row;
		c = rand() % col;
		
		if (map[r][c] == 0) {
			map[r][c] = rand() % 2 + 1;
			i++;
		}
	}
}

void draw_block(int row, int col)
{
	int block_left;
	int block_top;
	int text_x;
	int text_y;
	char num_str[10] = { 0 };
	int index = map[row][col];

	block_left = BLOCK_WIDTH * col + INTERVAL * (col + 1);
	block_top = BLOCK_HEIGHT * row + INTERVAL * (row + 1);

	setfillcolor(color_tbl[index]);
	solidroundrect(block_left, block_top, block_left + BLOCK_WIDTH, block_top + BLOCK_HEIGHT, ELLIPSE_WIDTH, ELLIPSE_HEIGHT);

	if (index != 0) {
		setbkmode(TRANSPARENT);
		settextstyle(TEXT_HEIGHT, TEXT_WIDTH, _T("ºÚÌå"));
		settextcolor(TEXT_COLOR);
		sprintf(num_str, "%d", num_tbl[index]);
		//printf("%s\n", num_str);
		text_x = block_left + (BLOCK_WIDTH - textwidth((LPCTSTR)num_str)) / 2;
		text_y = block_top + (BLOCK_HEIGHT - textheight((LPCTSTR)num_str)) / 2;
		outtextxy(text_x, text_y, (LPCTSTR)num_str);
	}
}

void draw_map(int row_num, int col_num)
{
	for (int i = 0; i < col_num; i++) {
		for (int j = 0; j < row_num; j++) {
			draw_block(i, j);
		}
	}
}

move_t key_parse(int key)
{
	move_t move_action = MOVE_INVALID;

	switch (key) {
	case 'W':
	case 'w':
	case KEY_UP:
		move_action = MOVE_UP;
		break;
	case 'S':
	case 's':
	case KEY_DOWN:
		move_action = MOVE_DOWN;
		break;
	case 'A':
	case 'a':
	case KEY_LEFT:
		move_action = MOVE_LEFT;
		break;
	case 'D':
	case 'd':
	case KEY_RIGHT:
		move_action = MOVE_RIGHT;
		break;
	defalut:
		break;
	}

	return move_action;
}

void move_block(move_t move_action)
{
	switch (move_action) {
	case MOVE_UP:

		break;
	case MOVE_DOWN:

		break;
	case MOVE_LEFT:

		break;
	case MOVE_RIGHT:

		break;
	default:
		break;
	}
}

int main(void)
{
	HWND grap_handle;
	int  key;

	srand((unsigned int)time(NULL));
	grap_handle = initgraph(BKG_WIDTH, BKG_HEIGHT, SHOWCONSOLE);
	setbkcolor(BKG_COLOR);
	init_map(BLOCK_ROW_NUM, BLOCK_COL_NUM);

	cleardevice();
	BeginBatchDraw();
	while ((key = _getch()) != KEY_ESC) {
		printf("---%d\n", key);
		draw_map(BLOCK_ROW_NUM, BLOCK_COL_NUM);
		FlushBatchDraw();
	}
	EndBatchDraw();
	closegraph();
	return 1;
}