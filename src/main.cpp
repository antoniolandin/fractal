#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "../lib/BS_thread_pool_light.hpp"
#include "../include/bounce.h"
#include "../include/utils.h"

int main(int argc, char const* argv[])
{
    const std::string NOMBRE_ARCHIVO = "imagen.png";
    const int ANCHO_PANTALLA = 320;
    const int ALTO_PANTALLA = 180;
    const double C_X = 0;
    const double C_Y = 0;
    const double ZOOM = 2;
    const int MAX_X_ORIGINAL = 5;
    const int MIN_X_ORIGINAL = -5;
    const int MAX_Y_ORIGINAL = 5;
    const int MIN_Y_ORIGINAL = -5;
    const double MAX_X = MAX_X_ORIGINAL / ZOOM + C_X;
    const double MIN_X = MIN_X_ORIGINAL / ZOOM + C_X;
    const double MAX_Y = MAX_Y_ORIGINAL / ZOOM + C_Y;
    const double MIN_Y = MIN_Y_ORIGINAL / ZOOM + C_Y;

    std::vector<double> X = linspace_double(MIN_X, MAX_X, ANCHO_PANTALLA);
    std::vector<double> Y = linspace_double(MIN_Y, MAX_Y, ALTO_PANTALLA);

    // Rellenamos los arrays de mapeo de colores que más tarde nos ayudaran a transformar un número en un color
    int STEP = 32;
    int len = (int)pow(STEP, 3);
    int map_r[(int)pow(STEP, 3)];
    int map_g[(int)pow(STEP, 3)];
    int map_b[(int)pow(STEP, 3)];

    int index = 0;

    for (int r = 0; r <= 255; r += STEP) {
        for (int g = 0; g <= 255; g += STEP) {
            for (int b = 0; b <= 255; b += STEP) {
                map_r[index] = r;
                map_g[index] = g;
                map_b[index] = b;
                index++;
            }
        }
    }

    // Utilizando multiprocessing, calculamos todos los botes en un vector
    BS::thread_pool_light pool;
    std::vector<std::future<int>> botes;

    for (auto y = Y.end() - 1; y != Y.begin(); --y) {
        for (auto x = X.begin(); x != X.end(); ++x) {
            botes.push_back(pool.submit(calcular_botes, *x, *y));
        }
    }

    // Una vez calculado el vector, transformamos ese vector de botes en una imagen
    std::ofstream imagen;

    imagen.open(NOMBRE_ARCHIVO);

    if (imagen.is_open()) {
        // Header del bitmap
        imagen << "P3" << std::endl;
        imagen << ANCHO_PANTALLA << " " << ALTO_PANTALLA << std::endl;
        imagen << "255" << std::endl;

        int r, g, b;

        for (auto i = botes.begin(); i != botes.end(); ++i) {
            map_to_color(i->get(), map_r, map_g, map_b, len, &r, &g, &b);
            imagen << (int)r << " " << (int)g << " " << (int)b << " ";
        }

        imagen.close();
    }

    printf("Imagen generada\n");

    return 0;
}
