#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a,const void *b){
    return *(int*)a - *(int*)b;
}

int longestConsecutive(int* nums, int numsSize) {
    if(nums == NULL) return 0;
    qsort(nums, numsSize,sizeof(int), cmp);
    int cur = 1;
    int best = 1;
    for(int i = 1; i < numsSize; ++i){
        if(nums[i] - nums[i - 1] == 1){
            ++cur;
        }else if(nums[i] - nums[i - 1] != 0){
            best = best > cur ? best : cur;
            cur = 1;
        }
    }

    best = best > cur ? best : cur;

    return best;
}

int main(void) {
    int nums[] = {1,0,1,2};
    int r = longestConsecutive(nums,sizeof(nums) / sizeof(int));
    printf("%d\n",r);;
    return 0;
}
