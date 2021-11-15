default: build run open-image

raytracer: src/*
	g++ -Wall -O3 -g3 src/main.cpp -o raytracer

build: raytracer

run:
	./raytracer

open-image:
	/mnt/c/Program\ Files/ImageGlass/ImageGlass.exe image.ppm
