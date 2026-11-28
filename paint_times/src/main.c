#include <stdio.h>

void paint(int, int (*)[4], int *);

int main()
{
    freopen("in.txt", "rb", stdin);
    int n, l[100][4];
    int result[300];
    int i;
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        scanf("%d%d%d%d", &l[i][0], &l[i][1], &l[i][2], &l[i][3]);
    }
    paint(n, l, result);
    for (i = 0; i <= result[0]; i++)
    {
        printf("%d %d\n", result[i * 2], result[i * 2 + 1]);
    }
    return 0;
}

int canvas[100][100] = { 0 };
void paint_canvas(int x1, int y1, int x2, int y2, int v){
    for(int y = y1; y <= y2; ++y){
        for(int x = x1; x <= x2; ++x){
            canvas[y][x] += v;
        }
    }
}

void paint(int n, int (*rects)[4], int *result){
    for(int i = 0; i < n; ++i){
        paint_canvas(rects[i][0], rects[i][1], rects[i][2], rects[i][3], 1);
    }
    for (int y = 0; y < 100; ++y) {
        for(int x = 0; x < 100; ++x){
            printf("%d", canvas[y][x]);
        }
        printf("\n");
    }
    printf("----------------\n");
    
}
