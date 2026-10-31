#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <string.h>

#define BOARD_COL 10
#define BOARD_ROW 10
#define BOARD_SIZE BOARD_ROW * BOARD_COL
#define WIN_NUMS 5
#define ENABLE_TRIM

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

typedef enum{
    INFINITY_POS,
    INFINITY_NEG,
    INFINITY_NOP,
}InfinityType;

typedef struct{
    int v;
    int infinite;
}Promise;

int *board = NULL;
int depth = 3;
int current_round = BLACK;
int totol_moves = 0;

#define DUBEG
#ifdef DUBEG
#define DUBEG_ITER
#endif

void gomoku_init(){
    board = malloc(BOARD_SIZE * sizeof(*board));
    for(int i = 0; i < BOARD_SIZE; ++i){
        board[i] = EMPTY;
    }

    assert(board != NULL);
}

ChessType get_chess_type(const int *board, int x, int y){
    if(x < 0 || y < 0) return UNKOWN;
    if(x >= BOARD_COL || y >= BOARD_ROW) return UNKOWN;

    return board[x + (BOARD_COL * y)];
}

void set_chess_type(int *board, unsigned int x, unsigned int y, ChessType type){
    if(x >= BOARD_COL || y >= BOARD_ROW) return;

    board[x + (BOARD_COL * y)] = type;
}

void print_chess_board(int *board){
    printf("-------------------\n");
    
    for(int y = 0; y < BOARD_ROW; ++y){
        for(int x = 0; x < BOARD_COL; ++x){
            if(get_chess_type(board, x, y) == WHITE) printf("w");
            if(get_chess_type(board, x, y) == BLACK) printf("b");
            if(get_chess_type(board, x, y) == EMPTY) printf("0");
        }
        printf("\n");
    }

    printf("-------------------\n");
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

static inline ChessType reverse_chess_type(ChessType type){
    if(type == BLACK){
        return WHITE;
    }else {
        return BLACK;
    }
}

#undef DUBEG_ITER
int score_iter(int *board, int depth, int current_depth, ChessType type, Promise parent_promise){
    ChessType current_type = type;
    Promise ret;
    bool allow_skip = 0;

    if(current_depth % 2 == 0){
        current_type = reverse_chess_type(type);
        ret = (Promise){ .infinite = INFINITY_POS };
    }else {
        ret = (Promise){ .infinite = INFINITY_NEG };
    }

    int scores[BOARD_SIZE] = {0};
    int count = 0;

    for(int y = 0; y < BOARD_ROW; ++y){
        for(int x = 0; x < BOARD_COL; ++x){
            if(get_chess_type(board, x, y) == EMPTY){
                set_chess_type(board, x, y, current_type);

                if(current_depth == depth){
                    int max = calc_score(board, type);
                    ChessType rev = reverse_chess_type(type);
                    int min = calc_score(board, rev);
                    scores[count] = max - min;
#ifdef DUBEG_ITER
                    print_chess_board(board);
                    printf("Reach the bottom: %d max = %d min = %d\n", scores[count], max, min);
#endif
                    if(current_type == type){
                        if(ret.infinite == INFINITY_NOP){
                            ret.v = ret.v > scores[count] ? ret.v : scores[count];
                        }else {
                            ret.v = scores[count];
                            ret.infinite = INFINITY_NOP;
                        }
#ifdef ENABLE_TRIM
                        assert(parent_promise.infinite != INFINITY_NEG);
                        if(parent_promise.infinite == INFINITY_NOP){
                            if(ret.v >= parent_promise.v) allow_skip = 1;
                        }
#endif
                    }else{
                        if(ret.infinite == INFINITY_NOP){
                            ret.v = ret.v < scores[count] ? ret.v : scores[count];
                        }else {
                            ret.v = scores[count];
                            ret.infinite = INFINITY_NOP;
                        }
#ifdef ENABLE_TRIM
                        assert(parent_promise.infinite != INFINITY_POS);
                        if(parent_promise.infinite == INFINITY_NOP){
                            if(ret.v <= parent_promise.v) allow_skip = 1;
                        }
#endif
                    }

                    ++count;
                }else {
                    scores[count] = score_iter(board, depth, current_depth + 1, type, ret);
#ifdef DUBEG_ITER
                    print_chess_board(board);
                    printf("Current depth = %d score = %d\n", current_depth, scores[count]);
#endif
                    if(current_type == type){
                        if(ret.infinite == INFINITY_NOP){
                            ret.v = ret.v > scores[count] ? ret.v : scores[count];
                        }else {
                            ret.v = scores[count];
                            ret.infinite = INFINITY_NOP;
                        }
#ifdef ENABLE_TRIM
                        assert(parent_promise.infinite != INFINITY_NEG);
                        if(parent_promise.infinite == INFINITY_NOP){
                            if(ret.v >= parent_promise.v) allow_skip = 1;
                        }
#endif
                    }else{
                        if(ret.infinite == INFINITY_NOP){
                            ret.v = ret.v < scores[count] ? ret.v : scores[count];
                        }else {
                            ret.v = scores[count];
                            ret.infinite = INFINITY_NOP;
                        }
#ifdef ENABLE_TRIM
                        assert(parent_promise.infinite != INFINITY_POS);
                        if(parent_promise.infinite == INFINITY_NOP){
                            if(ret.v <= parent_promise.v) allow_skip = 1;
                        }
#endif
                    }

                    ++count;
                }

                set_chess_type(board, x, y, EMPTY);
            }
            
            if(allow_skip) return ret.v;
        }
    }

    return ret.v;
}

int main(void) {
    gomoku_init();
    set_chess_type(board, 3,3 , BLACK);
    Promise promise = (Promise){.infinite = INFINITY_POS};
    int score = score_iter(board, 3, 1, WHITE, promise);
    print_chess_board(board);
    printf("score = %d", score);
    return 0;
}
