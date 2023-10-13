import pygame
import os

nombre_archivo_imagen = "imagen.png"
ruta = os.path.join(nombre_archivo_imagen)

nombre_exe = "balls.exe"

LARGO_PANTALLA = 720
ALTO_PANTALLA = 720

pygame.init()
pygame.display.set_caption("Zoom")
pantalla = pygame.display.set_mode((LARGO_PANTALLA, ALTO_PANTALLA))
reloj = pygame.time.Clock()

imagen = pygame.image.load(ruta).convert_alpha()
imagen = pygame.transform.scale(imagen, (LARGO_PANTALLA, ALTO_PANTALLA))

OS = os.name

args_file = open("args.txt", "r")
args = args_file.readlines()
MIN_X = float(args[0])
MAX_X = float(args[1])
MIN_Y = float(args[2])
MAX_Y = float(args[3])
OFFET_X = float(args[4])
OFFET_Y = float(args[5])
args_file.close()

puntos = []

click_activo = False

def pixel_to_coords(x, y):
    x_new = MAX_X*(x - LARGO_PANTALLA/2) / (LARGO_PANTALLA/2) + OFFET_X
    y_new = MAX_Y*(-y + ALTO_PANTALLA/2) / (ALTO_PANTALLA/2) + OFFET_Y
    return (x_new, y_new)
    

while True:
    
    #Limpiar la pantalla
    pantalla.fill((0, 0, 0)) 
    
    pantalla.blit(imagen, (0, 0))
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()
        # Si pulsa el espacio
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE and len(puntos) == 2:
                nombre_archivo_resultado = "run.bat"
                ruta_resultado = os.path.join(nombre_archivo_resultado)
                resultado = open(ruta_resultado, "w")
                
                LARGO = abs(puntos[1][0] - puntos[0][0])
                
                punto_1 = pixel_to_coords(puntos[0][0], puntos[0][1])
                punto_2 = pixel_to_coords(puntos[1][0], puntos[1][1])
                
                NEW_MAX_X = punto_2[0]
                NEW_MIN_X = punto_1[0]
                NEW_MAX_Y = punto_1[1]
                NEW_MIN_Y = punto_2[1]
    
                x_1 = puntos[0][0]
                y_1 = puntos[0][1]
                
                x_2 = puntos[1][0]
                y_2 = puntos[1][1]
                
                punto_medio = ((x_1 + x_2)/2, (y_1 + y_2)/2)
                punto_medio = pixel_to_coords(punto_medio[0], punto_medio[1])
                
                NEW_OFFSET_X = punto_medio[0]
                NEW_OFFSET_Y = punto_medio[1]
                
                resultado.write(f"{nombre_exe} {LARGO_PANTALLA} {ALTO_PANTALLA} {NEW_MIN_X} {NEW_MAX_X} {NEW_MIN_Y} {NEW_MAX_Y} 0 0")
                resultado.close()
                args_file = open("args.txt", "w")
                args_file.write(f"{NEW_MIN_X}\n{NEW_MAX_X}\n{NEW_MIN_Y}\n{NEW_MAX_Y}\n{NEW_OFFSET_X}\n{NEW_OFFSET_Y}")
                args_file.close()
                
                print("INFO:", punto_1, punto_2, punto_medio)
                
                pygame.quit()
                quit()

    # Posicion del raton
    pos = pygame.mouse.get_pos()
    
    print(pixel_to_coords(pos[0], pos[1]))
    
    # Boton del raton
    boton = pygame.mouse.get_pressed()

    if(boton[0] == 1):
        if (click_activo == False):
            puntos.append(pos)
            click_activo = True
    elif(click_activo == True):
        click_activo = False
        
    if(len(puntos) >= 3):
        puntos = []
        
    if(len(puntos) == 2):
        # Dibujar un rectangulo
        pygame.draw.rect(pantalla, (255, 0, 0), (puntos[0][0], puntos[0][1], puntos[1][0] - puntos[0][0], puntos[1][1] - puntos[0][1]), 2)
    
    
        
    pygame.display.update()
    reloj.tick(60)