import numpy as np
from matplotlib import pyplot as plt
from PIL import Image

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


def color(numero):
     if numero >= 0 and numero < 40:
          return (0,255,0)
     elif numero >= 40 and numero < 81:
          return (0,0,255)
     elif numero >= 81 :
          return (255,0,0)
     else:
          print(numero)
 
# Constantes
funcion = [1, 0, -3, 0, 0]
G = 9.81


def numero_botes(x_0, y_0):
     distancia_caida = y_0 - f(x_0)


     v = np.sqrt(2*G*distancia_caida)
     inter = [x_0, f(x_0)]
     x = np.linspace(-2, 2, 100)

     plt.plot(x_0, y_0, 'ro')
     plt.plot(x_0, f(x_0), 'ro')

     y_0 = f(x_0)

     numero_botes = 0

     while(x_0 < 0 and numero_botes < 1000):
          m = -derivada(x_0)
          c = G*(m**2 + 1) / (2*v**2)

          trayectoria = [0, 0, -c, m + 2*x_0*c, -m*x_0 -c*x_0**2 + y_0]

          resta = np.subtract(trayectoria, funcion)

          r = np.roots(resta)
          r = r[np.isreal(r)].real # Seleccionar solo las raices reales
          
          # Seleccionar la raiz correcta (puede haber muchas intersecciobnes en la parábola pero solo una es correcta)
          if m > 0:
               menor = 9000
               
               for raiz in r: 
                    raiz_final = round(raiz, 7)
                    if(raiz_final != round(x_0,7)):
                         if(raiz_final < menor):
                              menor = round(raiz, 7)
               x_i = menor
          else:
               mayor = -9000
               
               for raiz in r: 
                    raiz_final = round(raiz, 7)
                    if(raiz_final != round(x_0,7)):
                         if(raiz_final > mayor):
                              mayor = round(raiz, 7)
               x_i = mayor
                    
          y_i = f(x_i)
          
          # Actualizar la velocidad
          distancia_caida = y_0 - y_i
          
          if(distancia_caida < 0):
               distancia_caida = -distancia_caida
               velocidad_ganada = -np.sqrt(2*G*distancia_caida)
          else:
               velocidad_ganada = np.sqrt(2*G*distancia_caida)
          
          # Actualizar parámetros
          v += velocidad_ganada
          
          x_0 = x_i
          y_0 = y_i
          
          numero_botes += 1
          
     return numero_botes

MIN_X = -1.5
MAX_X = -0.5

MIN_Y = 1.5
MAX_Y = 0.5

RESOLUCION = 1000

X = np.linspace(-1.5, -0.5, RESOLUCION)
Y = np.linspace(0.5, 1.5, RESOLUCION)

pixeles = np.zeros((RESOLUCION, RESOLUCION, 3), dtype=np.uint8)

mayor = numero_botes(X[0], Y[0])

for i in range(RESOLUCION):
     for j in range(RESOLUCION):
          n_botes = numero_botes(X[i], Y[j])
          pixeles[i][j] = color(n_botes)
          if n_botes > mayor:
               mayor = n_botes
          print("Progreso en porcentaje: ", round((i*RESOLUCION + j) / (RESOLUCION**2) * 100, 2), "%")

print("Mayor numero de botes: ", mayor)

new_image = Image.fromarray(pixeles)
new_image.save('new.png')