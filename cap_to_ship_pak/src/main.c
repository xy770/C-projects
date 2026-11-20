#include <stdio.h>

int check(int min_cap, int n, int *weights, int day){
    int loaded = 0;
    int paks = 1;
    for (int i = 0; i < n; ++i) {
        if(min_cap < weights[i]){
            return 0;
        }
        if(loaded <= min_cap - weights[i]){
            loaded += weights[i];
        }else {
            paks += 1;
            loaded = weights[i];
        }
    }
    if(paks <= day){
        return 1;
    }
    return 0;
}

int main(void) {
    freopen("in.txt", "r", stdin);
    int weights[256] = { 0 };
    int n = 0;
    int day = 0;
    int min_cap = 999999;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &weights[i]);
    }
    scanf("%d", &day);
    int total_weight = 0;
    for (int i = 0; i < n; ++i) {
        total_weight += weights[i];
    }
    int min = total_weight / day - 1;
    int left = 0;
    int right = total_weight;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if(check(mid, n, weights, day)){
            right = mid;
        }else {
            left = mid + 1;
        }
    }

    min_cap = left;

    printf("min_cap = %d", min_cap);

    return 0;
}
