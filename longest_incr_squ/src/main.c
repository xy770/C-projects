#include <stdio.h>

int main(void) {
    int nums[256] = { 0 };
    int dp[256] = { 0 };
    int n = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &nums[i]); 
    }
    dp[0] = 1;
    for (int i = 1; i < n; ++i) {
        int best = 0;
        for (int j = 0; j < i; ++j) {
            if(dp[j] > best && nums[i] > nums[j]){
                best = dp[j];
            }
        }
        dp[i] = best + 1;
    }

    int best = -9999999;
    for(int i = 0; i < n; ++i){
        best = best >= dp[i] ? best : dp[i];
    }
    printf("best = %d", best);
    return 0;
}
