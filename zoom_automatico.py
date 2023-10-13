import subprocess
from PIL import Image
import cv2
import os
import numpy as np

def mpp_to_png(nombre_archivo_imagen):
    img = open(nombre_archivo_imagen, "r")

    array = np.zeros([720, 720, 3], dtype=np.uint8)

    cont = 0

    img = img.readlines()[3].split(" ")

    img.pop()

    for i in range(720):
        for j in range(720):
            if(cont > len(img)-3):
                break
            array[i][j] = [int(img[cont]), int(img[cont+1]), int(img[cont+2])]
            cont += 3
            
    img = Image.fromarray(array, 'RGB')
    img.save(nombre_archivo_imagen.replace(".ppm", ".png"))
    img.close()

frame_rate = 15

iteraciones = 15*60*frame_rate
inicio = 100

centro = (-0.5796783626, 1.023391813)

# Crear las imagenes
for i in range(inicio, inicio+iteraciones+1):
    comando = f"balls.exe 720 720 -0.75 1 {i} imagen{i}.ppm"
    subprocess.run(["balls.exe", "720", "720", f"{centro[0]}", f"{centro[1]}", f"{i}", f"zoom_a_la_elipse/imagen{i}.ppm"])

# Convertir de ppm a png
for i in range(1, iteraciones+1):
    mpp_to_png(f"zoom_a_la_elipse/imagen{i}.ppm")
    os.remove(f"zoom_a_la_elipse/imagen{i}.ppm")

# Crear el video
image_folder = 'zoom_a_la_elipse'
video_name = 'video.avi'

images = [img for img in os.listdir(image_folder) if img.endswith(".png")]

images.sort(key=lambda x: int(x.split(".")[0][6:]))
frame = cv2.imread(os.path.join(image_folder, images[0]))
height, width, layers = frame.shape

video = cv2.VideoWriter(video_name, 0, frame_rate, (width,height))

for image in images:
    video.write(cv2.imread(os.path.join(image_folder, image)))

cv2.destroyAllWindows()
video.release()
