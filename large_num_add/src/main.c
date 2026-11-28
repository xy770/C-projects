#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int to_int(char c){
    return c - '0';
}

void trim_zero(char *dest, char *str){
    uint32_t read = 0;
    uint32_t write = 0;
    int flag = 1;
    while (str[read] != '\0') {
        if(str[read] == '0' && flag){
            ++read;
            flag = 0;
        }else {
            dest[write++] = str[read++];
        }
    }
    dest[write] = '\0';
}

void pad_zero(char *dest, int cnt, char *str){
    int dp = 0;
    int sp = 0;
    while (cnt > 0) {
        dest[dp++] = '0';
        --cnt;
    }
    while (str[sp] != '\0') {
        dest[dp++] = str[sp++];
    }
    dest[dp] = '\0';
}

void reverse(char *str){
    int l = 0;
    int r = strlen(str) - 1;
    while (l < r) {
        char tmp = str[r];
        str[r] = str[l];
        str[l] = tmp;
        ++l; --r;
    }
}

void add(char *dest, char *a, char *b){
    int i = strlen(a) - 1;
    int j = strlen(b) - 1;
    int dp = 0;
    int carry = 0;
    while (i >= 0 || j >= 0) {
        int lv = 0;
        int rv = 0;
        if(i >= 0) lv = to_int(a[i]);
        if(j >= 0) rv = to_int(b[j]);

        int result = lv + rv + carry;
        carry = result / 10;
        dest[dp++] = result % 10 + '0';
        --i; --j;
    }
    dest[dp++] = carry + '0';
    dest[dp] = '\0';
    reverse(dest);
    trim_zero(dest, dest);
}

int main(void) {
    char a[256] = { 0 };
    char b[256] = { 0 };
    char r[256] = { 0 };
    scanf("%s %s", a, b);
    add(r, a, b);
    printf("%s", r);

    return 0;
}
