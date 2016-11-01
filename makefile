CC = cc
project: project.c
	cc -Wall project.c -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -o project


