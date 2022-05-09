CC = g++

all: Mandelbrot clean

Mandelbrot: main.o Mandelbrot.o
	$(CC) main.o Mandelbrot.o -o Mandelbrot.exe

main.o:
	$(CC) -c $(MODE) main.cpp -o main.o

Mandelbrot.o:
	$(CC) -c Mandelbrot.cpp $(OPT) -mavx2 -o Mandelbrot.o

run:
	.\Mandelbrot.exe $(W) $(H)

clean:
	del *.o
