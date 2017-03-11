CC = gcc
CFLAGS = -ggdb -g3 -Wall -std=gnu11 -g -O2 -pg -fno-omit-frame-pointer
LFLAGS = -lm -g -pg -lprofiler -Wl,-no_pie

.PHONY: all clean

all: render julia

render: main.o tga.o model.o
	$(CC) -o $@ $^ $(LFLAGS)

julia: julia.o tga.o model.o
	$(CC) -o $@ $^ $(LFLAGS)

julia.o: julia.c tga.h model.h
	$(CC) -c $(CFLAGS) -o $@ $<

main.o: main.c tga.h model.h
	$(CC) -c $(CFLAGS) -o $@ $<

tga.o:tga.c tga.h
	$(CC) -c $(CFLAGS) -o $@ $<

model.o:model.c model.h tga.h
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf render
	rm -rf julia
	rm -rf *.o
