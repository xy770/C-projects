#include <stdio.h>

int main(void) {
    int n = 0;
    int nums[256] = { 0 };
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &nums[i]);
    }
    int target = 0;
    scanf("%d", &target);

    int left = 0;
    int right = 0;
    int best = 999999;
    int sum = 0;
    while (right < n) {
        sum += nums[right++];
        while(sum >= target){
            int len = right - left;
            best = best <= len ? best : len;
            sum -= nums[left++];
        }
    }
    best = best == 999999 ? 0 : best;
    printf("best = %d", best);
    return 0;
}
