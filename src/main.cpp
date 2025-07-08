#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "../lib/BS_thread_pool_light.hpp"
#include "../include/bounce.h"
#include "../include/utils.h"
#include <SFML/Graphics.hpp>

int main(int argc, char const* argv[])
{
    const int ANCHO_PANTALLA = 1080;
    const int ALTO_PANTALLA = 720;
    const int DOWNSCALE = 5;
    const int ANCHO = ANCHO_PANTALLA / DOWNSCALE;
    const int ALTO = ALTO_PANTALLA / DOWNSCALE;
    const double C_X = 0;
    const double C_Y = 2;
    const double ZOOM = 2;
    const int MAX_X_ORIGINAL = 5;
    const int MIN_X_ORIGINAL = -5;
    const int MAX_Y_ORIGINAL = 5;
    const int MIN_Y_ORIGINAL = -5;
    const double MAX_X = MAX_X_ORIGINAL / ZOOM + C_X;
    const double MIN_X = MIN_X_ORIGINAL / ZOOM + C_X;
    const double MAX_Y = MAX_Y_ORIGINAL / ZOOM + C_Y;
    const double MIN_Y = MIN_Y_ORIGINAL / ZOOM + C_Y;
    const short unsigned int MAX_BOTES = 1000;

    std::vector<double> X = linspace_double(MIN_X, MAX_X, ANCHO);
    std::vector<double> Y = linspace_double(MIN_Y, MAX_Y, ALTO);

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

    // Utilizando multiprocessing, calculamos todos los promesas en un vector
    BS::thread_pool_light pool;
    std::vector<std::future<int>> promesas;

    int head = 5;
    int cont = 0;
    int next = 0;

    for (auto y = Y.rbegin(); y != Y.rend(); ++y) {
        for (auto x = X.begin(); x != X.end(); ++x) {
            promesas.push_back(pool.submit(calcular_botes, *x, *y, MAX_BOTES));
        }
    }

    // Una vez calculado el vector, transformamos ese vector de promesas en una imagen
    sf::Image image;
    image.create(ANCHO, ALTO);

    int r, g, b;
    int pos_x = 0;
    int pos_y = 0;
    int botes = 0;

    for (auto i = promesas.begin(); i != promesas.end(); ++i) {
        botes = i->get();
        map_to_color(botes, map_r, map_g, map_b, len, &r, &g, &b);
        image.setPixel(pos_x, pos_y, sf::Color(r, g, b));

        pos_x++;
        if(pos_x >= ANCHO) {
            pos_x = 0;
            pos_y++;
        }
    }

    printf("Imagen generada\n");

    // Create a texture from the image
    sf::Texture texture;
    texture.loadFromImage(image);

    // Create a sprite from the texture
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(DOWNSCALE, DOWNSCALE);

    sf::RenderWindow window(sf::VideoMode(ANCHO_PANTALLA, ALTO_PANTALLA), "fractal");

    // Run the program as long as the window is open
    while (window.isOpen()) {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with a black color
        window.clear();

        // Draw the sprite
        window.draw(sprite);

        // Update the window
        window.display();
    }

    return 0;
}
