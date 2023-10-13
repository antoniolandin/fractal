# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <vector>
# include <iostream>
# include "librerias/root_finder.hpp"
# include "librerias/Eigen/Eigen"
# include <fstream>
# include <future>
# include "librerias/BS_thread_pool_light.hpp"

# define MAX_BOTES 1000

using namespace std;using namespace Eigen;

bool double_equals(double a, double b, double epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

template<typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in)
{

  std::vector<double> linspaced;

  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);

  if (num == 0) { return linspaced; }
  if (num == 1) 
    {
      linspaced.push_back(start);
      return linspaced;
    }

  double delta = (end - start) / (num - 1);

  for(int i=0; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  linspaced.push_back(end); // I want to ensure that start and end
                            // are exactly the same as the input
  return linspaced;
}

int calcular_botes(double x_0, double y_0)
{
    const float g = 9.8;
    const double tol = 1e-8;

    Eigen::VectorXd coeffs(5);  // coeficientes del polinomio que resolvemos para tener la intersección con la curva
    set<double> roots;  // Raíces del polinomio

    // x,y son las coordenadas de la pelota
    double x = x_0;
    double y = y_0;

    double x_i, y_i;  // Coordenadas de la intersección con la curva
    double m = 0;    // Pendiente de la recta tangente a la curva en el punto de intersección
    double c = 0;    // Initialize c to 0
    double mayor, menor;

    int numero_botes = 0;
    bool dir = false; // false -> derecha, true -> izquierda

    // Calculamos la primera intersección con la curva (intersección con una recta vertical)
    y_i = pow(x, 4) - 3*pow(x, 2);

    double distancia_caida = y_0 - y_i;  // Distancia que cae la pelota
    double velocidad_ganada = 0;         // Velocidad ganada en la caída

    // Si no está por encima de la curva, no hay botes
    if(y_0 < y_i)
    {
        return -1;
    }

    // Calculamos la velocidad inicial
    double v = sqrt(2*g*distancia_caida);
    
    y = y_i;

    // Ver si la pelota está en la izquierda o en la derecha
    if (x_0 < 0)
    {
        dir = 1;
    }
    else
    {
        dir = 0;
    }

    while( ((x < 0 && dir == true) || (x > 0 && dir == false)) && (numero_botes < MAX_BOTES))
    {
        m = -4*pow(x, 3) + 6*x; // m = -f'(x)
        c = g*(pow(m, 2) + 1) / (2*pow(v, 2));   // Parámetro c de la parábola
       
        coeffs(0) = -1;
        coeffs(1) = 0;
        coeffs(2) = -c+3;
        coeffs(3) = m + 2*x*c;
        coeffs(4) = -m*x -c*pow(x, 2) + y;

        roots = RootFinder::solvePolynomial(coeffs, -INFINITY, INFINITY, tol); //Calcular las intersecciones de la parabola de la bola con la curva de la funcion

        // Si no hay raíces, no hay botes
        if(roots.size() == 0)
        {
            printf("No hay raices\n");
            return -1;
        }

        // De todas las intersecciones con la curva, nos quedamos con la más cercana a la pelota
        if(m > 0)
        {
            menor = 9999;

            for (auto i : roots) {
                if(double_equals(x, i) == false && i < menor)
                {
                    menor = i;
                }
            }

            x_i = menor;
        }
        else
        {
            mayor = -9999;

            for (auto i : roots) {
                if(double_equals(x, i) == false && i > mayor)
                {
                    mayor = i;
                }
            }

            x_i = mayor;
        }

        y_i = pow(x_i, 4) - 3*pow(x_i, 2);  // Calculamos la coordenada y de la intersección

        distancia_caida = y - y_i;  // Distancia que cae la pelota

        // Actualizar la velocidad
        if(distancia_caida < 0)
        {
            distancia_caida = -distancia_caida;
            velocidad_ganada = -sqrt(2*g*distancia_caida);
        }
        else
        {
            velocidad_ganada = sqrt(2*g*distancia_caida);
        }

        v += velocidad_ganada;

        x = x_i;
        y = y_i;

        numero_botes++;
    }

    return numero_botes;
}

void map_to_color(int numero, int *map_r, int *map_g, int *map_b, int len, int *r, int *g, int *b){
    if( numero == -1){
            *r = 255;
            *g = 255;
            *b = 255;
    }
    else if(numero > len){
            *r = 0;
            *g = 0;
            *b = 0;
     }
     else{
            *r = map_r[numero];
            *g = map_g[numero];
            *b = map_b[numero];
     }
}

int main(int argc, char const *argv[])
{
    double C_X, C_Y, ZOOM, MAX_X, MIN_X, MAX_Y, MIN_Y;
    double const MAX_X_ORIGINAL = 5;
    double const MIN_X_ORIGINAL = -5;
    double const MAX_Y_ORIGINAL = 5;
    double const MIN_Y_ORIGINAL = -5;
    int ANCHO_PANTALLA, ALTO_PANTALLA;
    string NOMBRE_ARCHIVO;

    if(argc != 7){
        printf("Uso: %s <ANCHO_PANTALLA> <ALTO_PANTALLA> <C_X> <C_Y> <ZOOM> <NOMBRE_ARCHIVO>\n", argv[0]);
        return -1;
    }
    else{
        ANCHO_PANTALLA = atoi(argv[1]);
        ALTO_PANTALLA = atoi(argv[2]);
        C_X = atof(argv[3]);
        C_Y = atof(argv[4]);
        ZOOM = atof(argv[5]);
        NOMBRE_ARCHIVO = argv[6];
    }

    MAX_X = MAX_X_ORIGINAL/ZOOM + C_X;
    MIN_X = MIN_X_ORIGINAL/ZOOM + C_X;
    MAX_Y = MAX_Y_ORIGINAL/ZOOM + C_Y;
    MIN_Y = MIN_Y_ORIGINAL/ZOOM + C_Y;

    vector<double> X = linspace(MIN_X, MAX_X, ANCHO_PANTALLA);
    vector<double> Y = linspace(MIN_Y, MAX_Y, ALTO_PANTALLA);

    // Rellenamos los arrays de mapeo de colores que más tarde nos ayudaran a transformar un número en un color
    int STEP = 32;
    int map_r[(int)pow(STEP, 3)];
    int map_g[(int)pow(STEP, 3)];
    int map_b[(int)pow(STEP, 3)];
    
    int index = 0;

    for (int r = 0; r <= 255; r += STEP){
        for (int g = 0; g <= 255; g += STEP){
            for (int b = 0; b <= 255; b += STEP){
                map_r[index] = r;
                map_g[index] = g;
                map_b[index] = b;
                index++;
            }
        }
    }

    // Utilizando multiprocessing, calculamos todos los botes en un vector
    BS::thread_pool_light pool;
    vector <future<int>> botes;

    for (auto y = Y.end()-1; y != Y.begin(); --y){
        for (auto x= X.begin(); x!= X.end(); ++x){
            botes.push_back(pool.submit(calcular_botes, *x, *y));
        }
     }

    // Una vez calculado el vector, transformamos ese vector de botes en una imagen
    ofstream imagen;

    imagen.open(NOMBRE_ARCHIVO);

    if(imagen.is_open()){
        //Header del bitmap
        imagen << "P3" << endl;
        imagen << ANCHO_PANTALLA << " " << ALTO_PANTALLA << endl;
        imagen << "255" << endl;

        int r, g, b;

        for (auto i = botes.begin(); i != botes.end(); ++i){
            map_to_color(i->get(), map_r, map_g, map_b, (int)pow(STEP, 3), &r, &g, &b);
            imagen << (int)r << " " << (int)g << " " << (int)b << " ";
        }

        imagen.close();
    }

    printf("Imagen generada\n");

    return 0; 
}
