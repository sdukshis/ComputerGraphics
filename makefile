CC = gcc
CFLAGS = -Wall -std=gnu11 -g
LFLAGS = -lm -g

.PHONY: all clean

all: render julia

render: main.o tga.o model.o
	$(CC) -o $@ $^ $(LFLAGS)

julia: julia.o tga.o model.o complex.o
	$(CC) -o $@ $^ $(LFLAGS)

julia.o: julia.c tga.h model.h complex.h
	$(CC) -c $(CFLAGS) -o $@ $<

main.o: main.c tga.h model.h
	$(CC) -c $(CFLAGS) -o $@ $<

tga.o:tga.c tga.h
	$(CC) -c $(CFLAGS) -o $@ $<

model.o:model.c model.h tga.h
	$(CC) -c $(CFLAGS) -o $@ $<

complex.o:complex.c complex.h
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf render
	rm -rf julia
	rm -rf *.o
