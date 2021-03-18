#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include <graphics.h>
#include <Windows.h>

#define BALL_TIMER_ID		1
#define BOARD_TIMER_ID		2
#define TIMER_NUM			3

#define BALL_TIMER_PERIOD	10
#define BOARD_TIEMR_PERIOD	2

#define MAP_WIDTH			6
#define MAP_HEIGHT			6

#define BRICK_WIDTH			80
#define BRICK_HEIGHT		25

#define	GRAPH_WIDTH			(BRICK_WIDTH * MAP_WIDTH)
#define GRAPH_HEIGHT		(BRICK_HEIGHT * MAP_HEIGHT + 400)

#define BOARD_WIDTH			100
#define BOARD_HEIGHT		20
#define BOARD_TOP			(GRAPH_HEIGHT - 100)//BOARD_HEIGHT*2)
#define BOARD_LEFT_INIT		((GRAPH_WIDTH - BOARD_WIDTH) / 2)
#define BOARD_SPEED_INIT	2
#define BOARD_COLOR			WHITE

#define BALL_POS_X			(GRAPH_WIDTH / 2)
#define BALL_POS_Y			(GRAPH_HEIGHT - 400)
#define	BALL_RADIUS			(20)
#define	BALL_SPEED_X_INIT	(2)
#define BALL_SPEED_Y_INIT	(2)
#define BALL_COLOR			YELLOW

#define MOVE_BOARD_LEFT		(GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
#define MOVE_BOARD_RIGHT	(GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))

enum class brick_t : int{
	NO_BRICK = 0,
	YELLO_BRICK,
	BLUE_BRICK,
	GREEN_BRICK,
};
#define BRICK_TYPE_NUM		3

struct Board {
	int			left;
	int			top;
	int			width;
	int			height;
	int			speed;
	COLORREF	color;
};

struct Ball {
	int			pos_x;
	int			pos_y;
	int			radius;
	int			speed_x;
	int			speed_y;
	COLORREF	color;
};

brick_t brick_map[MAP_HEIGHT][MAP_WIDTH];
const COLORREF brick_color_map[BRICK_TYPE_NUM+1] = { BLACK, LIGHTMAGENTA, LIGHTBLUE, LIGHTGREEN };

int  timer(time_t period, int id)
{
	static time_t last_time[TIMER_NUM] = {0};
	time_t now;

	now = clock();
	if (now - last_time[id] > period) {
		last_time[id] = now;
		return 1;
	}
	return 0;
}

void init_map(int width, int height) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			brick_map[i][j] =(brick_t)(rand() % BRICK_TYPE_NUM + 1);
		}
	}
}

void draw_brick(int index_x, int index_y, brick_t type)
{
	int top, left;
	int bottom, right;

	top = index_x * BRICK_HEIGHT;
	left = index_y * BRICK_WIDTH;
	bottom = top - BRICK_HEIGHT;
	right = left + BRICK_WIDTH;
	
	setlinecolor(BLACK);
	setfillcolor(brick_color_map[(int)type]);
	fillrectangle(left, top, right, bottom);
}

void draw_map(int width, int height)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			draw_brick(i, j, brick_map[i][j]);
		}
	}
}

struct Board* create_board(int left, int top, int width, int height, int speed, COLORREF color)
{
	struct Board* pB;

	if ((pB = (struct Board*)malloc(sizeof(struct Board))) == NULL) {
		return NULL;
	}

	pB->left = left;
	pB->top = top;
	pB->width = width;
	pB->height = height;
	pB->speed = speed;
	pB->color = color;

	return pB;
}

void draw_board(struct Board* board)
{
	setfillcolor(board->color);
	fillrectangle(board->left, board->top, board->left + board->width, board->top + board->height);
}

void move_board(struct Board* board)
{
	if (MOVE_BOARD_LEFT) {
		board->left = (board->left > board->speed) ? (board->left - board->speed) : 0;
		//board->left -= board->speed;
	}
	if (MOVE_BOARD_RIGHT) {
		board->left = (board->left < GRAPH_WIDTH - BOARD_WIDTH - board->speed) ? (board->left + board->speed) : (GRAPH_WIDTH - board->width);
		//board->left += board->speed;
	}
}

struct Ball* create_ball(int x, int y, int radius, int speed_x, int speed_y, COLORREF color)
{
	struct Ball* pb;

	if ((pb = (struct Ball*)malloc(sizeof(struct Ball))) == NULL) {
		return NULL;
	}

	pb->pos_x = x;
	pb->pos_y = y;
	pb->radius = radius;
	pb->speed_x = speed_x;
	pb->speed_y = speed_y;
	pb->color = color;

	return pb;
}

void draw_ball(struct Ball* ball)
{
	setfillcolor(ball->color);
	fillcircle(ball->pos_x, ball->pos_y, ball->radius);
}

int is_ball_crash(struct Ball* ball, struct Board* board)
{
	int ret = 0;
	if (ball->pos_x - ball->radius == 0 || ball->pos_x + ball->radius == GRAPH_WIDTH) {
		ball->speed_x *= -1;
		ret = 1;
	}
	if (ball->pos_y - ball->radius == 0) {
		ball->speed_y *= -1;
		ret = 1;
	}
	
	if (ball->pos_y + ball->radius - board->top >= 0
		&& ball->pos_y + ball->radius - board->top <= ball->speed_y
		&& ball->pos_x >= board->left
		&& ball->pos_x <= board->left + board->width) {
		ball->speed_y *= -1;
		ret = 1;
	}
	return ret;
}

int hit_brick(struct Ball* ball)
{
	int ball_i;
	int ball_j;
	int brick_type;
	int ret = 0;

	ball_j = ball->pos_x / BRICK_WIDTH;
	ball_i = (ball->pos_y - ball->radius) / BRICK_HEIGHT;
	brick_type = (int)brick_map[ball_i][ball_j];

	if (ball_i < MAP_WIDTH && ball_j < MAP_HEIGHT && brick_map[ball_i][ball_j] != brick_t::NO_BRICK) {
		brick_map[ball_i][ball_j] = (brick_t)(brick_type - 1);
		ball->speed_y *= -1;
		ret = 1;
	}

	return ret;
}

void move_ball(struct Ball* ball)
{
	ball->pos_x += ball->speed_x;
	ball->pos_y += ball->speed_y;
}

#define GAME_CONTINUE	0
#define GAME_OVER		1
#define	GAME_SUCCESS	2

int is_game_over(struct Ball* ball)
{
	int ret = 0;
	if (ball->pos_y > GRAPH_HEIGHT) {
		return GAME_OVER;
	}

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (brick_map[i][j] != brick_t::NO_BRICK) {
				return GAME_CONTINUE;
			}
		}
	}

	return GAME_SUCCESS;
}

int main(void)
{
	struct Board* board;
	struct Ball* ball;
	int    game_state;
	HWND   grap_handle;

	srand((unsigned int)time(NULL));
	init_map(MAP_WIDTH, MAP_HEIGHT);
	board = create_board(BOARD_LEFT_INIT, BOARD_TOP, BOARD_WIDTH, BOARD_HEIGHT, BOARD_SPEED_INIT, BOARD_COLOR);
	ball = create_ball(BALL_POS_X, BALL_POS_Y, BALL_RADIUS, BALL_SPEED_X_INIT, BALL_SPEED_Y_INIT, BALL_COLOR);
	
	if (board == NULL || ball == NULL)
		return 1;
	
	grap_handle = initgraph(GRAPH_WIDTH, GRAPH_WIDTH);
	BeginBatchDraw();
	while (1) {
		cleardevice();
		draw_map(MAP_WIDTH, MAP_HEIGHT);
		draw_board(board);
		draw_ball(ball);
		if (timer(BALL_TIMER_PERIOD, BALL_TIMER_ID)) {
			move_ball(ball);
			hit_brick(ball);
			if ((game_state = is_game_over(ball)) != GAME_CONTINUE) {
				break;
			}
		}
		if (timer(BOARD_TIEMR_PERIOD, BOARD_TIMER_ID)) {
			move_board(board);
			is_ball_crash(ball, board);
		}
		Sleep(1);
		FlushBatchDraw();
	}
	cleardevice();
	if (game_state == GAME_OVER) {
		//printf("Game Over~~\n");
		MessageBox(grap_handle, (LPCSTR)"Try Next Time!", (LPCSTR)"Game Over", MB_OK);
	}
	else if (game_state == GAME_SUCCESS) {
		//printf("You got it~~Congratuations!\n");
		MessageBox(grap_handle, (LPCSTR)"You Got It!", (LPCSTR)"Congratuations", MB_OK);
	}
	else {
		
	}
	Sleep(1000);
	EndBatchDraw();
	free(board);
	free(ball);
	closegraph();
	return 0;
}



