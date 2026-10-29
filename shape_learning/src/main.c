#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHAPE_HEIGHT 256
#define SHAPE_WIDTH  256

float canvas[SHAPE_HEIGHT][SHAPE_WIDTH] = { 0 };
float weights[SHAPE_HEIGHT][SHAPE_WIDTH] = { 0 };

float rand_float(float min, float max){
    assert(min < max);
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

static inline float clampf(float v, float max){
    return v > max ? max : v;
}

void canvas_clear(){
    for(uint32_t y = 0; y < SHAPE_HEIGHT; ++y){
        for(uint32_t x = 0; x < SHAPE_WIDTH; ++x){
            canvas[y][x] = 0;
        }
    }
}

void canvas_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, float v){
    --x;
    --y;
    w = clampf(w, SHAPE_WIDTH - x);
    h = clampf(h, SHAPE_HEIGHT - x);
    for(uint32_t y_i = y; y_i <= y + h - 1; ++y_i){
        for(uint32_t x_i = x; x_i <= x + w - 1; ++x_i) {
             canvas[y_i][x_i] = v;
        }
    }
}

void canvas_fill_circle(uint32_t x, uint32_t y, uint32_t r, float v){
    --x;
    --y;
    uint32_t start_x = clampf(x - r, 0);
    uint32_t start_y = clampf(y - r, 0);
    uint32_t end_y = clampf(y + r, SHAPE_HEIGHT - 1);
    uint32_t end_x = clampf(x + r, SHAPE_WIDTH - 1);
    for(uint32_t y_i = start_y; y_i <= end_y; ++y_i){
        for(uint32_t x_i = start_x; x_i <= end_x; ++x_i) {
            uint32_t x_dis = x - x_i;
            uint32_t y_dis = y - y_i;
            if((x_dis * x_dis) + (y_dis * y_dis) <= r * r){
                canvas[y_i][x_i] = v;
            }
        }
    }
}

void canvas_fill_circle_rand(){
    uint32_t r = (uint32_t)rand_float((float)(SHAPE_WIDTH + SHAPE_HEIGHT) / 20, (float)(SHAPE_WIDTH + SHAPE_HEIGHT) / 4);
    uint32_t pos_x = rand_float(1 + r,  SHAPE_WIDTH - r);
    uint32_t pos_y = rand_float(1 + r,  SHAPE_HEIGHT - r);

    canvas_fill_circle(pos_x, pos_y, r, 1);
}

void canvas_fill_rect_rand(){
    uint32_t width = (uint32_t)rand_float((float)(SHAPE_WIDTH + SHAPE_HEIGHT) / 20, (float)(SHAPE_WIDTH + SHAPE_HEIGHT) / 3);
    uint32_t height = (uint32_t)rand_float((float)(SHAPE_WIDTH + SHAPE_HEIGHT) / 20, (float)(SHAPE_WIDTH + SHAPE_HEIGHT) / 3);

    uint32_t pos_x = rand_float(1, SHAPE_WIDTH - width);
    uint32_t pos_y = rand_float(1, SHAPE_WIDTH - height);

    canvas_fill_rect(pos_x, pos_y, width, height, 1);
}

void save_ppm_to_file(const char* path){
    if(path == NULL) return;
    FILE *fp = fopen(path, "wb");
    if(fp == NULL) return;

    char head_buf[32] = { 0 };
    uint32_t ppm_w = SHAPE_WIDTH;
    uint32_t ppm_h = SHAPE_HEIGHT;
    snprintf(head_buf, 32, "P3\n%d %d\n255\n", ppm_w, ppm_h);
    fwrite(head_buf, strlen(head_buf), 1, fp);

    for(uint32_t y = 0; y < SHAPE_HEIGHT; ++y){
        for (uint32_t x = 0; x < SHAPE_WIDTH; ++x) {
            char content_buf[32] = {0};
            snprintf(content_buf, 32, "%d 0 0\n", (int)canvas[y][x] * 255);
            fwrite(content_buf, strlen(content_buf), 1, fp);
        }
    }

    fclose(fp);
}

void canvas_print(){
    printf("--------------------------\n");
    for(uint32_t y_i = 0; y_i < SHAPE_HEIGHT; ++y_i){
        for(uint32_t x_i = 0; x_i < SHAPE_WIDTH; ++x_i) {
             printf("%.0f ", canvas[y_i][x_i]);
        }
        printf("\n");
    }
    printf("--------------------------\n");
}


int main(void) {
    for(uint32_t i = 0; i < 100; ++i){
        canvas_clear();
        canvas_fill_circle_rand();
        canvas_fill_rect_rand();
        char file_path[256];

        snprintf(file_path, 256, "./output/%03d.ppm", i);
        save_ppm_to_file(file_path);
    }

    return 0;
}
