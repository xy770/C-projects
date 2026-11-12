#include <stdio.h>

// dp_min[i] = if(nums[i]>0&&dp_min<0) -> dp_min[i-1] * nums[i] else min
//             if(num[i]<0&&dp_min<0) -> dp_max[i-1] * nums[i] else nums[i]

int max(int x, int y){
    return x > y ? x : y;
}

int min(int x, int y){
    return x < y ? x : y;
}

int main(void){
    int nums[256] = { 0 };
    int dp_max[256] = { 0 };
    int dp_min[256] = { 0 };
    int num_counts = 0;

    scanf("%d", &num_counts);
    for (int i = 0; i < num_counts; ++i) {
        scanf("%d", &nums[i]);
    }

    dp_max[0] = nums[0];
    dp_min[0] = nums[0];
    for(int i = 1; i < num_counts; ++i){
        if(nums[i] > 0){
            if(dp_min[i-1] < 0){
                dp_min[i] = dp_min[i-1] * nums[i];
            }else {
                dp_min[i] = min(dp_min[i-1] * nums[i], nums[i]);
            }
            if(dp_max[i-1] > 0){
                dp_max[i] = dp_max[i-1] * nums[i];
            }else {
                dp_max[i] = max(dp_max[i-1] * nums[i], nums[i]);
            }
        }else if(nums[i] < 0){
            if(dp_max[i-1] > 0){
                dp_min[i] = dp_max[i-1] * nums[i];
            }else {
                dp_min[i] = min(dp_max[i-1] * nums[i], nums[i]);
            }
            if(dp_min[i-1] < 0){
                dp_max[i] = dp_min[i-1] * nums[i];
            }else {
                dp_max[i] = max(dp_min[i-1] * nums[i], nums[i]);
            }
        }else {
            dp_max[i] = 0;
            dp_min[i] = 0;
        }
    }

    int best = -999999999;
    for(int i = 0; i < num_counts; ++i){
        best = best >= dp_max[i] ? best : dp_max[i];
    }
    printf("best = %d", best);
}
