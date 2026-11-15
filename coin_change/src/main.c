#include <stdio.h>

int min(int x, int y){
    return x <= y ? x : y;
}

int main(void) {
    freopen("in.txt", "r", stdin);
    int coin_count = 0;
    int coins[256] = { 0 };
    int sum = 0;
    int dp[256*2] = { 0 };

    scanf("%d", &coin_count);


    for(int i = 0; i < coin_count; ++i){
        scanf("%d", &coins[i]);
    }

    scanf("%d", &sum);

    for(int i = 0; i <= sum; ++i){
        dp[i] = 999999999;
    }
    for(int i = 0; i < coin_count; ++i){
        dp[coins[i]] = 1;
    }
    dp[0] = 0;

    for (int i = 1; i <= sum; ++i) {
        for (int j = 0; j < coin_count; ++j) {
            int amount = coins[j];
            
            if(i - amount >= 0){
                dp[i] = min(dp[i], dp[i-amount] + 1);
            }
        }
    }
    printf("solution: %d", dp[sum]);
    return 0;
}
