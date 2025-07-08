import numpy as np
from matplotlib import pyplot as plt
import sympy as sp
from sympy.utilities.lambdify import lambdify

# Definición simbólica de la curva
x_sym = sp.symbols("x")
funcion_sym = x_sym**4 - 3 * x_sym**2  # Función original
derivada_sym = sp.diff(funcion_sym, x_sym)  # Derivada calculada automáticamente

# Convertir a funciones numéricas para evaluación
f = lambdify(x_sym, funcion_sym, "numpy")
derivada = lambdify(x_sym, derivada_sym, "numpy")

# Convertir a polinomio y extraer coeficientes
poly = sp.Poly(funcion_sym, x_sym)
coeficientes = poly.all_coeffs()

G = 9.81
X = np.linspace(-2, 2, 1000)


def poly_plot(polinomio, x):
    y = [np.polyval(polinomio, i) for i in x]
    plt.plot(x, y)


def mostrar_grafica(x_0, y_0):
    x = x_0
    y = y_0

    # Calculamos la primera intersección con la curva (intersección con una recta vertical)
    y_i = f(x)

    # Calculamos la distancia que ha caido la pelota
    distancia_caida = y_0 - y_i

    # Si no está por encima de la curva no hay botes
    if y_0 < y_i:
        return 0

    # Calculamos la velocidad inicial de la pelota
    v = np.sqrt(2 * G * distancia_caida)

    y = y_i

    numero_botes = 0

    derecha = True

    if x_0 < 0:
        derecha = False
    else:
        derecha = True

    while ((x < 0 and not derecha) or (x > 0 and derecha)) and numero_botes < 1000:
        m = -derivada(x)
        c = G * (m**2 + 1) / (2 * v**2)

        trayectoria = [0, 0, -c, m + 2 * x * c, -m * x - c * x**2 + y]

        resta = np.subtract(trayectoria, coeficientes)

        r = np.roots(resta)
        r = r[np.isreal(r)].real  # Seleccionar solo las raices reales

        # Seleccionar la raiz correcta (puede haber muchas intersecciones en la parábola pero solo una es correcta)
        mayor = 0
        menor = 0

        if m > 0:
            menor = 9000

            for raiz in r:
                raiz_final = round(raiz, 7)
                if raiz_final != round(x, 7):
                    if raiz_final < menor:
                        menor = round(raiz, 7)
            x_i = menor
        else:
            mayor = -9000

            for raiz in r:
                raiz_final = round(raiz, 7)
                if raiz_final != round(x, 7):
                    if raiz_final > mayor:
                        mayor = round(raiz, 7)
            x_i = mayor

        y_i = f(x_i)

        # Actualizar la velocidad
        distancia_caida = y - y_i

        if distancia_caida < 0:
            distancia_caida = -distancia_caida
            velocidad_ganada = -np.sqrt(2 * G * distancia_caida)
        else:
            velocidad_ganada = np.sqrt(2 * G * distancia_caida)

        # Actualizar parámetros
        v += velocidad_ganada

        x = x_i
        y = y_i

        numero_botes += 1

        # Dibujar la trayectoria y el punto de intersección
        poly_plot(trayectoria, X)
        plt.plot(x_i, y_i, "ro")

    # Configuracion de la grafica
    plt.xlim(xmax=2, xmin=-2)
    plt.ylim(ymin=-3, ymax=5)

    # Dibujar los elementos iniciales de la grafica
    plt.plot(x_0, y_0, "ro")
    plt.plot(x_0, f(x_0), "ro")
    poly_plot(coeficientes, X)
    plt.axvline(x=x_0)

    plt.show()


if __name__ == "__main__":
    mostrar_grafica(1, 1)
