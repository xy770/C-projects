#include <stdio.h>
#include <stdlib.h>

int main(void){
    int num_count = 0;
    int *nums = NULL;
    int *dp = NULL;
    scanf("%d", &num_count);
    nums = malloc(sizeof(*nums) * num_count);
    dp = malloc(sizeof(*nums) * num_count);

    for(int i = 0; i < num_count; ++i){
        scanf("%d", &nums[i]);
    }

    dp[0] = nums[0];
    for(int i = 1; i < num_count; ++i){
        dp[i] = nums[i] > dp[i-1] + nums[i] \
                ? nums[i] \
                : dp[i-1] + nums[i];
    }
    
    int best = -99999999;
    for(int i = 0; i < num_count; ++i){
        best = best >= dp[i] \
               ? best \
               : dp[i];
    }

    printf("best = %d\n", best);
    free(nums);
    free(dp);
}
