#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <string.h>

#define BOARD_COL 5
#define BOARD_ROW 5
#define BOARD_SIZE BOARD_ROW * BOARD_COL
#define WIN_NUMS 5

typedef enum{
    WHITE = 0x01,
    BLACK = 0x02,
    UNKOWN = 0x04,
    EMPTY = 0x08,
}ChessType;

typedef struct{
    int x;
    int y;
}Coord;

int *board = NULL;
int depth = 3;
int current_round = BLACK;
int totol_moves = 0;

void gomoku_init(){
    board = malloc(BOARD_SIZE * sizeof(board));
    for(int i = 0; i < BOARD_SIZE; ++i){
        board[i] = EMPTY;
    }

    assert(board != NULL);
}

ChessType get_chess_type(const int *board, int x, int y){
    if(x < 0 || y < 0) return UNKOWN;
    if(x >= BOARD_COL || y >= BOARD_ROW) return UNKOWN;

    return board[x + (5 * y)];
}

void set_chess_type(int *board, unsigned int x, unsigned int y, ChessType type){
    if(x >= BOARD_COL || y >= BOARD_ROW) return;

    board[x + (5 * y)] = type;
}

int same_count(const int *board, int x, int y, ChessType type){
    int count = 0;

    for (int x_offset = 0; x_offset < WIN_NUMS; ++x_offset) {
        if((get_chess_type(board, x + x_offset, y) & type) > 0){
            if(x_offset + 1 == WIN_NUMS){
                ++count;
            }
            continue;
        }
        break;
    }

    for (int y_offset = 0; y_offset < WIN_NUMS; ++y_offset) {
        if((get_chess_type(board, x, y + y_offset) & type) > 0){
            if(y_offset + 1 == WIN_NUMS){
                ++count;
            }
            continue;
        }
        break;
    }

    for(int offset = 0; offset < WIN_NUMS; ++offset){
        if((get_chess_type(board, x + offset, y + offset) & type) > 0){
            if(offset + 1 == WIN_NUMS){
                ++count;
            }
            continue;
        }
        break;
    }

    for(int offset = 0; offset < WIN_NUMS; ++offset){
        if((get_chess_type(board, x - offset, y + offset) & type) > 0){
            if(offset + 1 == WIN_NUMS){
                ++count;
            }
            continue;
        }
        break;
    }

    return count;
}

int calc_score(int *board, ChessType type){
    int score = 0;

    for(int y = 0; y < BOARD_ROW; ++y){
        for(int x = 0; x < BOARD_COL; ++x){
            int calc_type = type | EMPTY;

            score += same_count(board, x, y, calc_type);
        }
    }

    return score;
}



int main(void) {
    gomoku_init();
    set_chess_type(board, 2, 2, BLACK);
    int s = calc_score(board, WHITE);
    printf("\n%d", s);
    return 0;
}
