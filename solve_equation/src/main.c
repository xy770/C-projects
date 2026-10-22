#include <math.h>
#include <stdio.h>

int main(void) {
    float a = 0;
    float b = 0;
    float c = 0;
    scanf("%f%f%f", &a, &b, &c);

    float delta = (b * b) - (4 * a * c);

    if(delta == 0){
        printf("%.2f",-b / (2 * a));
    }
    else if(delta > 0){
        delta = sqrtf(delta);
        printf("%.2f %.2f", (-b + delta) / (2 * a), (-b - delta) / (2 * a));
    }
    else if(delta < 0){
        delta = sqrtf(-delta);
        printf("%.2f+%.2fi %.2f%.2fi",-b / (2 * a),delta / (2 * a),-b / (2 * a),-delta / (2 * a));
    }
    return 0;
}
