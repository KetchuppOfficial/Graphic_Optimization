#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# NO OPTIMIZATIONS

Mandelbrot:
	g++ .\Mandelbrot.cpp -o Mandelbrot.exe

run:
	.\Mandelbrot.exe

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# AVX2 OPTIMIZATION WITH DOUBLE NUMBERS

AVX_double:
	g++ .\Mandelbrot_AVX2_double.cpp -mavx2 -O3 -o Mandelbrot_AVX2_double.exe

run_double:
	.\Mandelbrot_AVX2_double.exe

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# AVX2 OPTIMIZATION WITH FLOAT NUMBERS

AVX_float:
	g++ .\Mandelbrot_AVX2_float.cpp -mavx2 -O3 -o Mandelbrot_AVX2_float.exe

run_float:
	.\Mandelbrot_AVX2_float.exe

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

clean:
	del *.exe
