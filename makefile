CC = gcc
CFLAGS = -g -Wall
LFLAGS = -lm

.PHONY: all clean

all: render sample_putPixel

render: main.o tga.o model.o
	$(CC) -o $@ $^ $(LFLAGS)

sample_putPixel: sample_putPixel.o tga.o model.o
	$(CC) $(LFLAGS) -o $@ $^

sample_putPixel.o: sample_putPixel.c tga.h model.h
	$(CC) -c $(CFLAGS) -o $@ $<

main.o: main.c tga.h model.h
	$(CC) -c $(CFLAGS) -o $@ $<

tga.o:tga.c tga.h
	$(CC) -c $(CFLAGS) -o $@ $<

model.o:model.c model.h tga.h
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf render
	rm -rf *.o
