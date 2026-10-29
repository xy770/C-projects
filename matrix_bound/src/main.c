#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_to_num(int *res, char *str){
    int len = strlen(str);
    int res_off = 0;

    for(int i = 0; i < len; ++i){
        if(i == 0){
            sscanf(str, "%d", &res[res_off]);
            ++res_off;
            continue;
        }

        if(str[i] != ' ') continue;
        sscanf(str + i, " %d", &res[res_off]);
        ++res_off;
    }
}

int main(void) {
    int col = 0;
    int row = 0;
    int *mat = NULL;
    int target = 0;
    int count = 0;

    int first_line[4] = {0};
    char buf[256] = {0};
    fgets(buf, 256, stdin);
    string_to_num(first_line, buf);
    row = first_line[0];
    col = first_line[1];
    count = first_line[2];
    target = first_line[3];

    mat = calloc(row * col, sizeof(int) * row * col);
    int mat_off = 0;

    for(int i = 0; i < row; ++i){
        char buff[256] = {0};
        fgets(buff, 256, stdin);
        string_to_num(mat + mat_off, buff);
        mat_off += col;
    }

    int *bound = calloc(count, sizeof(int) * (count + 1));

    for(int i = 0; i < row * col ; ++i){
        if(mat[i] == target){
            if(i + 1 < row * col && (i + 1) % col != 1) bound[mat[i + 1]] = 1;
            if(i - 1 > 0 && (i - 1) % col != 0) bound[mat[i - 1]] = 1;
            if(i + col < row * col) bound[mat[i + col]] = 1;
            if(i - col > 0) bound[mat[i - col]] = 1;
        }
    }

    int result = 0;
    for(int i = 1; i <= count; ++i){
        if(bound[i] == 1 && i != target) ++result;
    }

    printf("%d", result);
    
    return 0;
}
