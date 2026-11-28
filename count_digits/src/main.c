#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    uint64_t num;
    uint32_t even_count;
    uint32_t even_sum;
}Num;

int to_digit(char v){
    return v - '0';
}

Num num_calc(uint64_t num){
    char str[32] = { 0 };
    Num ret = { 0 };
    ret.num = num;

    sprintf(str, "%zu", num);
    uint32_t len = strlen(str);
    for(int i = 0; i < len; ++i){
        int digit = to_digit(str[i]);
        if(digit % 2 == 0){
            ++ret.even_count;
            ret.even_sum += digit;
        }
    }

    return ret;
}

int main(void){
    Num nums[64] = { 0 };

    int n = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        uint64_t input = 0;
        scanf("%zu", &input);
        nums[i] = num_calc(input);
    }

    uint32_t best_count = nums->even_count;
    uint32_t best_sum = nums->even_sum;
    for (int i = 0; i < n; ++i) {
        best_count = best_count >= nums[i].even_count ? best_count : nums[i].even_count;
        best_sum = best_sum >= nums[i].even_sum ? best_sum : nums[i].even_sum;
    }

    for (int i = 0; i < n; ++i) {
        if(nums[i].even_count == best_count && nums[i].even_sum == best_sum){
            printf("Yes %zu %u %u", nums[i].num, nums[i].even_count, nums[i].even_sum);
            return 0;
        }
    }

    printf("No");
}
