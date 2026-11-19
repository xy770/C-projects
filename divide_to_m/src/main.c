#include <stdio.h>

int s[256] = { 0 };
int n = 0;

int check(int m, int limit){
    int count = 0;
    int loaded = 0;
    for(int i = 0; i < n; ++i){
        if(s[i] > limit) return 0;
        if(loaded + s[i] > limit){
            ++count;
            loaded = s[i];
        }else {
            loaded += s[i];
        }
    }
    ++count;
    if(count <= m){
        return 1;
    }else {
        return 0;
    }
}

int main(void){
    int m = 1;
    int sum = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &s[i]);
        sum += s[i];
    }
    scanf("%d", &m);
    int left = 0;
    int right = sum;
    while (left < right) {
        int mid = left + (right - left) / 2;

        if(check(m, mid)){
            right = mid;
        }else {
            left = mid + 1;
        }
    }
    printf("Result: %d", left);
}
