
import numpy as np
from matplotlib import pyplot as plt

def f(x):
     return x**4 - 3*x**2 
 
def derivada(x):
     return 4*x**3 - 6*x
 
def PolyCoefficients(x, coeffs):
    """ Returns a polynomial for ``x`` values for the ``coeffs`` provided.

    The coefficients must be in ascending order (``x**0`` to ``x**o``).
    """
    o = len(coeffs)
    print(f'# This is a polynomial of order {o}.')
    y = 0
    for i in range(o):
        y += coeffs[i]*x**i
    return y

 
def poly_plot(polinomio, x):
     y = [np.polyval(polinomio, i) for i in x]
     plt.plot(x,y)

# Constantes
funcion = [1, 0, -3, 0, 0]
G = 9.81
X = np.linspace(-2, 2, 1000)

def mostrar_grafica(x_0, y_0):
    
    x = x_0
    y = y_0
    
    # Calculamos la primera intersección con la curva (intersección con una recta vertical)
    y_i = f(x)
     
    # Calculamos la distancia que ha caido la pelota
    distancia_caida = y_0 - y_i
     
    # Si no está por encima de la curva no hay botes
    if(y_0 < y_i):
        return 0
     
    # Calculamos la velocidad inicial de la pelota
    v = np.sqrt(2*G*distancia_caida) 
     
    y = y_i

    numero_botes = 0
     
    derecha = True
     
    if(x_0 < 0):
        derecha = False
    else:
        derecha = True

    while( ((x < 0 and derecha == False) or (x > 0 and derecha == True)) and numero_botes < 1000):
        m = -derivada(x)
        c = G*(m**2 + 1) / (2*v**2)

        trayectoria = [0, 0, -c, m + 2*x*c, -m*x -c*x**2 + y]

        resta = np.subtract(trayectoria, funcion)

        r = np.roots(resta)
        r = r[np.isreal(r)].real # Seleccionar solo las raices reales
          
        # Seleccionar la raiz correcta (puede haber muchas intersecciones en la parábola pero solo una es correcta)
        mayor = 0
        menor = 0
          
        if m > 0:
            menor = 9000
               
            for raiz in r: 
                raiz_final = round(raiz, 7)
                if(raiz_final != round(x,7)):
                    if(raiz_final < menor):
                        menor = round(raiz, 7)
            x_i = menor
        else:
            mayor = -9000
               
            for raiz in r: 
                raiz_final = round(raiz, 7)
                if(raiz_final != round(x,7)):
                    if(raiz_final > mayor):
                        mayor = round(raiz, 7)
            x_i = mayor
                    
        y_i = f(x_i)
          
        # Actualizar la velocidad
        distancia_caida = y - y_i
          
        if(distancia_caida < 0):
            distancia_caida = -distancia_caida
            velocidad_ganada = -np.sqrt(2*G*distancia_caida)
        else:
            velocidad_ganada = np.sqrt(2*G*distancia_caida)
          
        # Actualizar parámetros
        v += velocidad_ganada
          
        x = x_i
        y = y_i
          
        numero_botes += 1
          
        # Dibujar la trayectoria y el punto de intersección
        poly_plot(trayectoria, X)
        plt.plot(x_i, y_i, 'ro')
         
    # Configuracion de la grafica
    plt.xlim(xmax=2, xmin=-2)
    plt.ylim(ymin=-3, ymax=5)
    
    # Dibujar los elementos iniciales de la grafica
    plt.plot(x_0, y_0, 'ro')
    plt.plot(x_0, f(x_0), 'ro')
    poly_plot(funcion, X)
    plt.axvline(x = x_0)
    
    # Guardar la imagen de la gráfica
    plt.savefig(f"botes en ({x_0},{y_0}).png")
          
    # limpiar memoria
    try:
        del trayectoria, resta, r, x_i, y_i, x_0, y_0, x, y, v, distancia_caida, velocidad_ganada, m, c, mayor, menor, raiz_final
    except:
        pass     
     
    return numero_botes

mostrar_grafica(1, 1)