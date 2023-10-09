import numpy as np
from intersect import intersection
import matplotlib.pyplot as plt

def f(x):
     return x**4 - 3*x**2 
 
def g(x):
    return 1
 
def derivada(x):
     return 4*x**3 - 2*x
 
G = 9.8

posicion_inicial = [1, 1]

print(1, f(1))

print("pediente: ", derivada(1))

tratectoria = (1, derivada(1))