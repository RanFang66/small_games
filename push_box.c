#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <windows.h>

#define BACKGROUND_WIDTH        (10)
#define BACKGROUND_HEIGHT       (10)

#define BOX_NUM                 (3)
#define LEVEL_NUM               (3)


#define KEY_UP                  (72)
#define KEY_DOWN                (80)
#define KEY_LEFT                (75)
#define KEY_RIGHT               (77)
#define KEY_ESC                 (27)
#define KEY_BLANK               (32)

#define SPACE                   (0)
#define WALL                    (1)
#define DEST                    (2)
#define BOX                     (3)
#define PLAYER                  (4)

typedef enum action_type {
    MOVE_INVALID = 0,
    MOVE_DOWN = 1,
    MOVE_UP = 2,
    MOVE_LEFT = 3,
    MOVE_RIGHT = 4,
} action_t;

#define VALID_POS               (1)
#define INVALID_POS             (2)

#define MOVE_SUCCESS            (1)
#define MOVE_FAILED             (0)

#define MAP_VALUE(POS)         (map[POS.y][POS.x])

#define IS_VALID_POS(POS)      (POS.x < BACKGROUND_WIDTH \
                                && POS.y< BACKGROUND_HEIGHT)

#define MISSION_SUCCESS        (score == BOX_NUM)

const int8_t MAP_INIT[LEVEL_NUM][BACKGROUND_HEIGHT][BACKGROUND_WIDTH] = {
   {{ 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 1, 1, 1, 1 },
    { 1, 1, 1, 4, 0, 3, 0, 0, 2, 1 },
    { 1, 2, 0, 0, 3, 0, 1, 1, 1, 1 },
    { 1, 1, 1, 0, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 1, 0, 3, 0, 1, 0, 0, 0 },
    { 0, 0, 1, 0, 1, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 }},

   {{  0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
    {  0, 1, 2, 1, 0, 0, 1, 2, 1, 0 },
    {  1, 0, 0, 1, 1, 1, 0, 0, 0, 1 },
    {  1, 0, 4, 0, 1, 0, 3, 0, 0, 1 },
    {  1, 0, 0, 3, 0, 0, 0, 0, 1, 0 },
    {  0, 1, 0, 0, 0, 3, 0, 1, 0, 0 },
    {  0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
    {  0, 0, 0, 1, 2, 1, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},

   {{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
    { 1, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
    { 1, 0, 1, 4, 0, 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 3, 1, 0, 0, 1 },
    { 1, 0, 1, 3, 0, 0, 1, 0, 0, 1 },
    { 1, 2, 1, 0, 3, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 },
    { 0, 0, 1, 0, 1, 1, 1, 1, 0, 1 },
    { 0, 0, 1, 2, 1, 0, 0, 1, 2, 1 },
    { 0, 0, 1, 1, 1, 0, 0, 1, 1, 1 }},
};

typedef struct position {
    int8_t     x;
    int8_t     y;
}pos_t;

const pos_t pos_table[LEVEL_NUM] = { {3, 2}, {2, 3}, {3, 2} };

pos_t   player_pos = { .x = 3, .y = 2 };
int8_t  map[BACKGROUND_HEIGHT][BACKGROUND_WIDTH];
int8_t  score = 0;
int8_t  level = 0;

void map_init(const int8_t m[BACKGROUND_HEIGHT][BACKGROUND_WIDTH])
{
    for (int i = 0; i < BACKGROUND_HEIGHT; i++) {
        for (int k = 0; k < BACKGROUND_WIDTH; k++) {
            map[i][k] = m[i][k];
        }
    }
}

void player_pos_init(const pos_t* pos)
{
    player_pos.x = pos->x;
    player_pos.y = pos->y;
}

void map_update(void)
{
    printf("--- Level: %d, Score: %d --- \n--- Input Esc Key to Exit the Game ---.\n", level + 1, score);
    for (int8_t i = 0; i < BACKGROUND_HEIGHT; i++) {
        for (int8_t k = 0; k < BACKGROUND_WIDTH; k++) {
            switch (map[i][k]) {
            case SPACE:
                printf("  ");
                break;
            case WALL:
                printf("||");
                break;
            case DEST:
                printf("口");
                break;
            case BOX:
                printf("😄");
                break;
            case PLAYER:
                printf("人");
                break;
            case PLAYER + DEST:
                printf("囚");
                break;
            case BOX + DEST:
                printf("回");
                break;
            default:
                printf("  ");
                break;
            }
        }
        printf("\n");
    }
}

action_t parse_key(char key)
{
    int8_t move_action = MOVE_INVALID;

    switch (key) {
    case 'w':
    case 'W':
    case KEY_UP:
        move_action = MOVE_UP;
        break;
    case 's':
    case 'S':
    case KEY_DOWN:
        move_action = MOVE_DOWN;
        break;
    case 'a':
    case 'A':
    case KEY_LEFT:
        move_action = MOVE_LEFT;
        break;
    case 'd':
    case 'D':
    case KEY_RIGHT:
        move_action = MOVE_RIGHT;
        break;
    default:
        break;
    }
    return move_action;
}

pos_t update_position(pos_t pos, action_t action)
{
    pos_t pos_new;

    switch (action) {
    case MOVE_UP:
        pos_new.x = pos.x;
        pos_new.y = pos.y - 1;
        break;
    case MOVE_DOWN:
        pos_new.x = pos.x;
        pos_new.y = pos.y + 1;
        break;
    case MOVE_LEFT:
        pos_new.x = pos.x - 1;
        pos_new.y = pos.y;
        break;
    case MOVE_RIGHT:
        pos_new.x = pos.x + 1;
        pos_new.y = pos.y;
        break;
    default:
        pos_new.x = -1;
        pos_new.y = -1;
        break;
    }
    return pos_new;
}

void update_score(pos_t pos)
{
    if (MAP_VALUE(pos) == BOX + DEST) {
        score++;
    }
}


int8_t move(action_t action)
{
    pos_t   pos_new;
    pos_t   box_pos_new;
    int8_t  ret = MOVE_FAILED;

    pos_new = update_position(player_pos, action);

    if (!IS_VALID_POS(pos_new)) {
        return MOVE_FAILED;
    }
    else if (MAP_VALUE(pos_new) == SPACE || MAP_VALUE(pos_new) == DEST) {
        MAP_VALUE(player_pos) -= PLAYER;
        MAP_VALUE(pos_new) += PLAYER;
        player_pos = pos_new;
        ret = MOVE_SUCCESS;
    }
    else if (MAP_VALUE(pos_new) == BOX) {
        box_pos_new = update_position(pos_new, action);
        if (IS_VALID_POS(box_pos_new) &&
            (MAP_VALUE(box_pos_new) == SPACE || MAP_VALUE(box_pos_new) == DEST)) {
            MAP_VALUE(pos_new) -= BOX;
            MAP_VALUE(pos_new) += PLAYER;
            MAP_VALUE(box_pos_new) += BOX;
            MAP_VALUE(player_pos) -= PLAYER;
            player_pos = pos_new;
            update_score(box_pos_new);
            ret = MOVE_SUCCESS;
        }
    }

    return ret;
}

void level_update(void)
{
    printf("You finished Level 1! Now Start the Level 2...");
    Sleep(2000);
    map_init(MAP_INIT[level]);
    player_pos_init(pos_table + level);
    system("cls");
    map_update();
    score = 0;
}

int main(void)
{
    char        key_push;
    action_t    move_action = 0;

    system("mode con cols=40 lines=25");
    printf("Welcome~~~\nPlease Input Blank Key to Start Game...\n");
    while ((key_push = _getch()) != KEY_BLANK) {
        if (key_push == KEY_ESC)
            return 0;
    }

    map_init(MAP_INIT[level]);
    player_pos_init(pos_table + level);
    system("cls");
    map_update();

    while ((key_push = _getch()) != KEY_ESC) {
        move_action = parse_key(key_push);
        if (move_action != MOVE_INVALID && move(move_action) == MOVE_SUCCESS) {
            system("cls");
            map_update();
            if (MISSION_SUCCESS) {
                if (++level < LEVEL_NUM) {
                    level_update();
                }
                else {
                    printf("You Got It!\n");
                    Sleep(2000);
                    return 0;
                }
            }
        }
    }
    printf("Exiting Game...\n");
    Sleep(2000);
    return 0;
}


