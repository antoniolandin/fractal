#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "../lib/BS_thread_pool_light.hpp"
#include "../include/bounce.h"
#include "../include/utils.h"
#include "../include/color.h"
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

    // Utilizando multiprocessing, calculamos todos los promesas en un vector
    BS::thread_pool_light pool;
    std::vector<std::future<int>> promesas;

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
    int num_botes = 0;

    ColorMap color_map = ColorMap(32);

    for (auto i = promesas.begin(); i != promesas.end(); ++i) {
        num_botes = i->get();
        color_map.map(num_botes, &r, &g, &b);
        image.setPixel(pos_x, pos_y, sf::Color(r, g, b));

        pos_x++;
        if(pos_x >= ANCHO) {
            pos_x = 0;
            pos_y++;
        }
    }

    printf("Imagen generada\n");

    // Crear una textura de la imagen
    sf::Texture texture;
    texture.loadFromImage(image);

    // Crear un sprite de la textura
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // Rescalar el sprite al tamaño de la pantalla
    sprite.setScale(DOWNSCALE, DOWNSCALE);
    
    // Crear ventana del programa
    sf::RenderWindow window(sf::VideoMode(ANCHO_PANTALLA, ALTO_PANTALLA), "fractal");
    
    // Bucle de renderizado
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
