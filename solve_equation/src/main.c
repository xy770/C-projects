#include <math.h>
#include <stdio.h>

int main(void) {
    float a = 0;
    float b = 0;
    float c = 0;
    scanf("%f%f%f", &a, &b, &c);

    float delta = (b * b) - (4 * a * c);

    if(a == 0 && b == 0 && c == 0){
        printf("Zero Equation\n");
        return 0;
    }
    if(a == 0 && b == 0){
        printf("Not An Equation\n");
        return 0;
    }
    if(a == 0 && b != 0){
        printf("%.2f", -c / b);
        return 0;
    }


    if(delta == 0){
        printf("%.2f",-b / (2 * a));
    }
    else if(delta > 0){
        delta = sqrtf(delta);
        if(a > 0){
            printf("%.2f\n%.2f", (-b + delta) / (2 * a), (-b - delta) / (2 * a));
        }else {
            printf("%.2f\n%.2f", (-b - delta) / (2 * a), (-b + delta) / (2 * a));
        }
    }
    else if(delta < 0){
        delta = sqrtf(-delta);
        printf("%.2f+%.2fi\n%.2f%.2fi",-b / (2 * a),delta / (2 * a),-b / (2 * a),-delta / (2 * a));
    }
    return 0;
}
