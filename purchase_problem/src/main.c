#include <stdint.h>
#include <stdio.h>

#define CURRENT_SUM ((count[0] * data[0][1]) + (count[1] * data[1][1]) + (count[2] * data[2][1]))

int main(void) {
    int has_solution = 0;
    int32_t sum = 0;
    int32_t data[][2] = {
        {0,0},
        {0,0},
        {0,0},
    };

    char buffer[3][256] = {0};

    scanf("%d",&sum);
    fgets((void*)buffer,255,stdin);
    for(int i = 0; i < 3; ++i){
        fgets(buffer[i], 255, stdin);
        sscanf(buffer[i],"%d %d",&data[i][0], &data[i][1]); 
    }

    int32_t count[3] = {0};
    for(count[0] = 0;CURRENT_SUM <= sum;++count[0]){    
        for(count[1] = 0;CURRENT_SUM <= sum;++count[1]){
            int32_t total = sum - (count[0] * data[0][0]) - (count[1] * data[1][0]);
            if(total % data[2][0] != 0) continue;
            count[2] = total / data[2][0];
            if(CURRENT_SUM == sum){
                printf("%d %d %d\n",data[0][0] * count[0],data[1][0] * count[1],data[2][0] * count[2]);
                has_solution = 1;
            }
            count[2] = 0;
        }
        count[1] = 0;
        count[2] = 0;
    }
    if(!has_solution) printf("no solution");
    return 0;
}
