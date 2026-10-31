#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <string.h>

#define BOARD_COL 3
#define BOARD_ROW 3
#define BOARD_SIZE BOARD_ROW * BOARD_COL
#define WIN_NUMS 3
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

void print_coord(Coord coord){
    printf("Pos: x = %d; y = %d\n", coord.x, coord.y);
}

int same_count_right(const int *board, int x, int y, ChessType type, int same_nums, bool continuous){
    int count = 0;

    for (int x_offset = 0; x_offset < same_nums; ++x_offset) {
        if((get_chess_type(board, x + x_offset, y) & type) > 0){
            if(x_offset + 1 == same_nums){
                ++count;
            }
            continue;
        }
        if(continuous) break;
    }

    return count;
}

int same_count_down(const int *board, int x, int y, ChessType type, int same_nums, bool continuous){
    int count = 0;
    for (int y_offset = 0; y_offset < same_nums; ++y_offset) {
        if((get_chess_type(board, x, y + y_offset) & type) > 0){
            if(y_offset + 1 == same_nums){
                ++count;
            }
            continue;
        }
        break;
        if(continuous) break;
    }
    return count;
}

int same_count_slash(const int *board, int x, int y, ChessType type, int same_nums, bool continuous){
    int count = 0;
    for(int offset = 0; offset < same_nums; ++offset){
        if((get_chess_type(board, x + offset, y + offset) & type) > 0){
            if(offset + 1 == same_nums){
                ++count;
            }
            continue;
        }
        break;
        if(continuous) break;
    }

    return count;
}

int same_count_slash_r(const int *board, int x, int y, ChessType type, int same_nums, bool continuous){
    int count = 0;
    for(int offset = 0; offset < same_nums; ++offset){
        if((get_chess_type(board, x - offset, y + offset) & type) > 0){
            if(offset + 1 == same_nums){
                ++count;
            }
            continue;
        }

        if(continuous) break;
    }

    return count;
}

int same_count(const int *board, int x, int y, ChessType type, int same_nums){
    int count = 0;

    count += same_count_right(board, x, y, type, same_nums, true);
    count += same_count_down(board, x, y, type, same_nums, true);
    count += same_count_slash(board, x, y, type, same_nums, true);
    count += same_count_slash_r(board, x, y, type, same_nums, true);

    return count;
}

int calc_score(int *board, ChessType type){
    int score = 0;

    for(int y = 0; y < BOARD_ROW; ++y){
        for(int x = 0; x < BOARD_COL; ++x){
            int calc_empty = type | EMPTY;

            score += same_count(board, x, y, calc_empty, WIN_NUMS);
            if(same_count(board, x, y, calc_empty, WIN_NUMS) > 0){
                int right = same_count_right(board, x, y, type, WIN_NUMS, false);
            }
        }
    }

    return score;
}

int calc_result(int *board, ChessType type, int same_nums){
    int score = 0;

    for(int y = 0; y < BOARD_ROW; ++y){
        for(int x = 0; x < BOARD_COL; ++x){
            int calc_type = type;

            score += same_count(board, x, y, calc_type, same_nums);
        }
    }

    return score;
}

int calc_weight(int *board, int x_p, int y_p, ChessType type){
    int offset = WIN_NUMS % 2 == 0 ? WIN_NUMS : WIN_NUMS - 1;
    int start_y = y_p - (offset / 2);
    int start_x = x_p - (offset / 2);
    int end_y = y_p + (offset / 2);
    int end_x = x_p + (offset / 2);
    
    int weight = 0;

    for(int y = start_y; y <= end_y; ++y){
        for(int x = start_x; x <= end_x; ++x){
            if(x == x_p && y == y_p) continue;
            if((get_chess_type(board, x, y) & type) > 0) ++weight;
        }
    }

    return weight;
}

static inline ChessType reverse_chess_type(ChessType type){
    if(type == BLACK){
        return WHITE;
    }else {
        return BLACK;
    }
}

#undef DUBEG_ITER
//#undef ENABLE_TRIM
int score_iter(int *board, int depth, int current_depth, ChessType type, Promise parent_promise, Coord *best_pos){
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

                if(calc_result(board, current_type, WIN_NUMS) > 0){
                    set_chess_type(board, x, y, EMPTY);
                    if(type == current_type){
                        return 9999999;
                    }else {
                        return -9999999;
                    }
                }

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
                    }else{
                        if(ret.infinite == INFINITY_NOP){
                            ret.v = ret.v < scores[count] ? ret.v : scores[count];
                        }else {
                            ret.v = scores[count];
                            ret.infinite = INFINITY_NOP;
                        }
                    }

                    ++count;
                }else {
                    scores[count] = score_iter(board, depth, current_depth + 1, type, ret, NULL);
#ifdef DUBEG_ITER
                    print_chess_board(board);
                    printf("Current depth = %d score = %d\n", current_depth, scores[count]);
#endif
                    if(current_type == type){
                        if(ret.infinite == INFINITY_NOP){
                            if(scores[count] > ret.v && current_depth == 1){
                                best_pos->x = x;
                                best_pos->y = y;
                                printf("Update Score: %d ", scores[count]);
                                print_coord(*best_pos);
                            }else if(scores[count] == ret.v && current_depth == 1){
                                ChessType w_type = WHITE | BLACK;
                                int old_weight = calc_weight(board, best_pos->x, best_pos->y, w_type);
                                int new_weight = calc_weight(board, x, y, w_type);
                                if(new_weight > old_weight){
                                    best_pos->x = x;
                                    best_pos->y = y;
                                    printf("Update Score: %d old_weight = %d new_weight = %d", scores[count], old_weight, new_weight);
                                    print_coord(*best_pos);
                                }
                            }

                            ret.v = ret.v > scores[count] ? ret.v : scores[count];
                        }else {
                            ret.v = scores[count];
                            ret.infinite = INFINITY_NOP;

                            if(current_depth == 1){
                                best_pos->x = x;
                                best_pos->y = y;
                            }
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

Coord ai_brain_storm(ChessType type){
    Promise promise = (Promise){.infinite = INFINITY_POS};
    Coord pos = (Coord){ .x = 0, .y = 0 };
    int score = score_iter(board, 3, 1, type, promise, &pos);

    printf("-------------------------\n");
    printf("Think complete! final_score = %d\n", score);
    print_coord(pos);
    printf("-------------------------\n");

    return pos;
}

int main(void) {
    gomoku_init();

    while (1) {
        if(calc_result(board, WHITE, WIN_NUMS) > 0){
            printf("White Won!\n");
            break;
        }
        if(calc_result(board, BLACK, WIN_NUMS) > 0){
            printf("Black Won!\n");
            break;
        }
        
        print_chess_board(board);
        printf("Input where to put chess\n");
        if(current_round == BLACK){
            int x = 0;
            int y = 0;
            scanf("%d%d", &x, &y);
            set_chess_type(board, x, y, BLACK);
            current_round = WHITE;
        }else {
           Coord target_pos = ai_brain_storm(WHITE); 
           set_chess_type(board, target_pos.x, target_pos.y, WHITE);
           current_round = BLACK;
        }
    }

    return 0;
}
