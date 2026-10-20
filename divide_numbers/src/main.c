#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b){
    return *(int*)a - *(int*)b;
}

int main(void) {
    uint32_t nmemb = 5;
    int nums[100] = {0};

    int even[100] = {0};
    int even_count = 0;

    int odd[100] = {0};
    int odd_count = 0;

    for(uint32_t i = 0; i < nmemb; ++i){
        scanf("%d",&nums[i]);
        if(nums[i] % 2 == 0){
            even[even_count] = nums[i];
            ++even_count;
        }else {
            odd[odd_count] = nums[i];
            ++odd_count;
        }
    }

    qsort(even, even_count, sizeof(int), cmp);
    qsort(odd, odd_count, sizeof(int), cmp);
    
    for (int i = 0; i < odd_count; ++i) {
        printf("%d ", odd[i]);
    }
    for (int i = 0; i < even_count; ++i) {
        printf("%d ", even[i]);
    }

    return 0;
}
