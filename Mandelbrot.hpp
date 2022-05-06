#ifndef MANDELBROT_HPP_INCLUDED
#define MANDELBROT_HPP_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "immintrin.h"

void Draw_Mandelbrot             (const int hor_size, const int vert_size);
void Draw_Mandelbrot_AVX2_double (const int hor_size, const int vert_size);
void Draw_Mandelbrot_AVX2_float  (const int hor_size, const int vert_size);

#endif