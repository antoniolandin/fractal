#include <Eigen/Dense>
#include <root_finder.hpp>
#include <math.h>
#include "bounce.h"
#include "utils.h"

int calcular_botes(double x_0, double y_0, const short unsigned int MAX_BOTES)
{
    const float g = 9.8;
    const double tol = 1e-8;

    Eigen::VectorXd coeffs(5); // coeficientes del polinomio que resolvemos para tener la intersección con la curva
    std::set<double> roots; // Raíces del polinomio

    // x,y son las coordenadas de la pelota
    double x = x_0;
    double y = y_0;

    double x_i, y_i; // Coordenadas de la intersección con la curva
    double m = 0; // Pendiente de la recta tangente a la curva en el punto de intersección
    double c = 0; // Initialize c to 0
    double mayor, menor;

    int numero_botes = 0;
    bool dir = false; // false -> derecha, true -> izquierda

    // Calculamos la primera intersección con la curva (intersección con una recta vertical)
    y_i = pow(x, 4) - 3 * pow(x, 2);

    double distancia_caida = y_0 - y_i; // Distancia que cae la pelota
    double velocidad_ganada = 0; // Velocidad ganada en la caída

    // Si no está por encima de la curva, no hay botes
    if (y_0 < y_i) {
        return -1;
    }

    // Calculamos la velocidad inicial
    double v = sqrt(2 * g * distancia_caida);

    y = y_i;

    // Ver si la pelota está en la izquierda o en la derecha
    if (x_0 < 0) {
        dir = 1;
    } else {
        dir = 0;
    }

    while (((x < 0 && dir == true) || (x > 0 && dir == false)) && (numero_botes < MAX_BOTES)) {
        m = -4 * pow(x, 3) + 6 * x; // m = -f'(x)
        c = g * (pow(m, 2) + 1) / (2 * pow(v, 2)); // Parámetro c de la parábola

        coeffs(0) = -1;
        coeffs(1) = 0;
        coeffs(2) = -c + 3;
        coeffs(3) = m + 2 * x * c;
        coeffs(4) = -m * x - c * pow(x, 2) + y;

        roots = RootFinder::solvePolynomial(coeffs, -INFINITY, INFINITY, tol); // Calcular las intersecciones de la parabola de la bola con la curva de la funcion

        // Si no hay raíces, no hay botes
        if (roots.size() == 0) {
            printf("No hay raices\n");
            return -1;
        }

        // De todas las intersecciones con la curva, nos quedamos con la más cercana a la pelota
        if (m > 0) {
            menor = 9999;

            for (auto i : roots) {
                if (double_equals(x, i) == false && i < menor) {
                    menor = i;
                }
            }

            x_i = menor;
        } else {
            mayor = -9999;

            for (auto i : roots) {
                if (double_equals(x, i) == false && i > mayor) {
                    mayor = i;
                }
            }

            x_i = mayor;
        }

        y_i = pow(x_i, 4) - 3 * pow(x_i, 2); // Calculamos la coordenada y de la intersección

        distancia_caida = y - y_i; // Distancia que cae la pelota

        // Actualizar la velocidad
        if (distancia_caida < 0) {
            distancia_caida = -distancia_caida;
            velocidad_ganada = -sqrt(2 * g * distancia_caida);
        } else {
            velocidad_ganada = sqrt(2 * g * distancia_caida);
        }

        v += velocidad_ganada;

        x = x_i;
        y = y_i;

        numero_botes++;
    }

    return numero_botes;
}
