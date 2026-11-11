#include <stddef.h>
#include <string.h>
#include <stdio.h>
// expr -> ('(' + expr + ')' | '[' + expr + ']')*

#define OK -2 
#define Err -1

int parse_expr(char *str, int *index){
    if(*index == Err) return Err;
    size_t len = strlen(str);

    while (*index + 1 <= len) {
        if(str[*index] == '('){
            ++*index;
            parse_expr(str, index);
            if(str[*index] == ')'){
                ++*index;
                continue;
            }
        }else if(str[*index] == '['){
            ++*index;
            parse_expr(str, index);
            if(str[*index] == ']'){
                ++*index;
                continue;
            }
        }else {
            return Err;
        }   
    }

    return OK;
}

int main(void) {
    int result[64] = {0};
    int i = 0;
    int n = 0;
    scanf("%d", &n);
    for (i = 0; i < n; ++i) {
        char buf[256] = { 0 };
        scanf("%s", buf);
        int idx = 0; 
        result[i] = parse_expr(buf, &idx);
    }

    for (int j = 0; j < n; ++j) {
        if(result[j] == OK){
            printf("YES\n");
        }else{
            printf("NO\n");
        }
    }
    return 0;
}
