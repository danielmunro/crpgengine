main: main.c
	cc main.c `pkg-config --libs --cflags raylib` -o main
