#include "color.h"
#include <math.h>

ColorMap::ColorMap(int step){
    this->len = static_cast<int>(pow(step, 3));
    this->r_map = static_cast<int *>(malloc(static_cast<size_t>(this->len) * sizeof(int)));
    this->g_map = static_cast<int *>(malloc(static_cast<size_t>(this->len) * sizeof(int)));
    this->b_map = static_cast<int *>(malloc(static_cast<size_t>(this->len) * sizeof(int)));

    int index = 0;

    for (int r = 0; r <= 255; r += step) {
        for (int g = 0; g <= 255; g += step) {
            for (int b = 0; b <= 255; b += step) {
                this->r_map[index] = r;
                this->g_map[index] = g;
                this->b_map[index] = b;
                index++;
            }
        }
    }
}

ColorMap::~ColorMap(){
    free(this->r_map);
    free(this->b_map);
    free(this->g_map);
}

void ColorMap::map(int numero, int* r, int* g, int* b)
{
    if (numero == -1) {
        *r = 255;
        *g = 255;
        *b = 255;
    } else if (numero > this->len) {
        *r = 0;
        *g = 0;
        *b = 0;
    } else {
        *r = this->r_map[numero];
        *g = this->g_map[numero];
        *b = this->b_map[numero];
    }
}
