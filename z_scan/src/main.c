#include <assert.h>
#include <stdio.h>

int n = 0;
int mat[256] = {0};

int mat_at(int x, int y){
    return mat[x + y * n];
}

int is_mat_at(int x, int y){
    if(x < 0 || y < 0) return 0;
    if(x >= n || y >= n) return 0;

    return 1;
}

int main(void){
    scanf("%d", &n);
    assert(n > 0);

    for (int i = 0; i < n * n; ++i) {
        scanf("%d", &mat[i]);
    }

    int mode = 1;
    for(int i = 0; i < 2 * n - 1; ++i){
        int x = 0;
        int y = 0;
        if(i % 2 == 0){
            x = 0;
            y = i;
        }else {
            x = i;
            y = 0;
        }
        
        while (is_mat_at(x, y)) {
            if(i + 1 == n * n){
                printf("%d\n", mat_at(x, y));
                break;
            }
            printf("%d ", mat_at(x, y));
            x += mode;
            y += mode;
        }

        mode = -mode;
    }
}
