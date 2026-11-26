#include <stdio.h>

#define ERR -9999999

int m = 0;
int n = 0;
int mat[256] = { 0 };
int total;

static inline int at(int x, int y){
    if(x < 0 || y < 0) return ERR;
    if(x >= m || y >= n)  return ERR;

    return mat[x + y * m];
}

static inline void set(int x, int y){
    mat[x + y * m] = ERR;
}

int main(void) {
    scanf("%d%d", &m, &n);
    total = m * n;
    for (int i = 0; i < total; ++i) {
        scanf("%d", &mat[i]); 
    }

    int i = 0;
    int j = 0;
#define RIGHT 1 << 0
#define DOWN  1 << 1
#define LEFT  1 << 2
#define UP    1 << 3
#define END   1 << 4
    int dir = RIGHT;
    while (dir != END) {
        printf("%d", at(i, j));
        set(i, j);
        if(dir == RIGHT){
            if(at(i + 1, j) != ERR){
                ++i;
                continue;
            }else if(at(i, j + 1) != ERR) {
                ++j;
                dir = DOWN;
            }else {
                dir = END;
            }
        }else if(dir == DOWN){
            if(at(i, j + 1) != ERR){
                ++j;
                continue;
            }else if(at(i - 1, j) != ERR) {
                --i;
                dir = LEFT;
            }else {
                dir = END;
            }
        }else if(dir == LEFT){
            if(at(i - 1, j) != ERR){
                --i;
                continue;
            }else if(at(i, j - 1) != ERR) {
                --j;
                dir = UP;
            }else {
                dir = END;
            }
        }else if(dir == UP){
            if(at(i, j - 1) != ERR){
                --j;
                continue;
            }else if(at(i + 1, j) != ERR) {
                ++i;
                dir = RIGHT;
            }else {
                dir = END;
            }
        }
    }
    
    return 0;
}
