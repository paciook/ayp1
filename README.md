# ayp1

Every coding resource made in "Algoritmos y Programacion I". -Universidad de Buenos Aires.

# How to use

Open any of the folders and trust the corresponding Makefile (I wouldn't trust the folders starting in 'P', they're not supposed to work...).

Then check the steps depending on the TP number.

# TP1
Draw a simple B&W scene in which you can identify a sphere based on its centre and radius (Coordenates, radius, and resolution defined in main.c)

Before you execute the main bin, redirect the output into a file (ie: `./main > image.ppm`). This will generate a PPM image

# TP2
Still a B&W scene, but know we care about a third dimention and lighting. Coordinates take the form of arrays so it's easier to have more spheres and lights. Shadow also take place.

Running this should be the same as TP1 (`./main > image.ppm`).

# TP3
Spheres and lights become structs so we can add a little bit of color into their lifes. Also reflections (later added) take place.

We keep running this as `./main > image.ppm`

# TP4
Forget the spheres and anything with more than 2 dimensions. Now we care about Binary files. Now you can create a BMP image or a PPM depending on the file extension you indicate by running `./main WIDTH HEIGHT <filename>`.
