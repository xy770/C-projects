#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point Point;
struct Point{
    float x;
    float y;
    size_t id;
};

Point* points = NULL;
size_t point_count = 0;
size_t point_capacity = 0;
#define INITIAL_POINTS_COUNT 10

Point test_point = {0};

void read_points_from_file(const char* path){
    FILE* fp = fopen(path,"rb");
    if(fp == NULL){
        fprintf(stderr, "Failed to open points file");
        return;
    }

    if(points != NULL){
        free(points);
        point_capacity = 0;
    }

    points = malloc(sizeof(*points) * INITIAL_POINTS_COUNT);
    point_capacity = INITIAL_POINTS_COUNT;
    char line[256] = {0};
    int i = 0;
    while (fgets(line,sizeof(line),fp) != NULL) {
        if(point_capacity <= i){
            size_t times = 2;
            while (times * point_capacity <= i) {
                times *= 2;
            }
            Point *tmp = realloc(points, times * point_capacity * sizeof(Point));
            if(tmp == NULL){
                fprintf(stderr,"Failed to alloc mem");
                fclose(fp);
                free(points);
                return;
            }
            point_capacity *= times;
            points = tmp;
        }
        int ret = sscanf(line, "x=%f y=%f id=%zu",&points[i].x,&points[i].y,&points[i].id);
        if(ret != 3){
            fprintf(stderr,"Data file syntax error");
            fclose(fp);
            free(points);
            return;
        }
        ++i;
    }
    point_count = i;
}

static inline float distance_of(Point a,Point b){
    return sqrtf(powf(a.x - b.x,2) + powf(a.y - b.y,2));
}


int compare_dis(const void* a,const void* b){
    float d1 = distance_of(*(Point*)a, test_point);
    float d2 = distance_of(*(Point*)b, test_point);
    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}

size_t calc_point_id(uint32_t k){
    if(points == NULL) return 1;
    size_t ids[128] = {0};
    qsort(points, point_count, sizeof(Point), compare_dis);
    for(int i = 0;i < k;++i){
        ids[points[i].id]++;
    }
    size_t best_id = 0;
    int best_count = 0;
    for(int i = 0;i < 128;++i){
        if(ids[i] > best_count){
            best_id = i;
            best_count = ids[i];
        }
    }

    return best_id;
}

int main(void) {
    char buffer[256] = {0};
    printf("Please input a point(format:x=<x>y=<y>)\n");
    if(fgets(buffer, sizeof(buffer), stdin) == NULL){
        fprintf(stderr, "Invaild Input\n");
    }
    int ret = sscanf(buffer,"x=%fy=%f",&test_point.x,&test_point.y);
    if(ret != 2){
        fprintf(stderr,"Invaild Input");
        return 1;
    }

    printf("Please input K:");
    uint32_t k = 0;
    scanf("%u",&k);

    read_points_from_file("./points.txt");
    printf("Best id:%zu",calc_point_id(k));

    return 0;
 }
