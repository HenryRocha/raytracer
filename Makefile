default: build run

raytracer: src/*
	g++ src/main.cpp -o raytracer

build: raytracer

run:
	./raytracer
