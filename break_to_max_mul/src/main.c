#include <stdio.h>

int max_result(int n, int k){
    int o = n % k;
    int v = (n - o) / k;
    int result = 1;

    for (int i = 1; i <= k; ++i) {
        if(i <= o){
            result *= (v + 1);
        }else {
            result *= v;
        }
    }

    return result;
}

int main(void) {
    int n = 0;
    scanf("%d", &n);
    int best = -1;
    for(int k = 2; k < n; ++k){
        int cur = max_result(n, k);
        best = best >= cur ? best : cur;
    }
    printf("%d", best);
    return 0;
}
