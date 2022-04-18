![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

# Mandelbrot Set

This project is C++ implementation of the Mandelbrot set renderer. The feature the project is using Intel<sup>&reg;</sup> Intrinsics for optimization of calculations.

## Dependencies

Visulization of the Mandelbrot set is implemented by [TXLib](https://github.com/ded32/TXLib) by [Ilya Dedinsky (aka ded 32)](https://github.com/ded32).

## Results

Measurements were carried out on a 800x600 screen. Mandelbot scale was 0.004 and the picture was static.

**FPS rate:**

| Optimization key | No optimizations | AVX2 double | AVX2 float |
|------------------|------------------|-------------|------------|
|       None       |        5         |     8       |     14     |
|       -O2        |       12         |   31 - 32   |  52 - 53   |
|       -O3        |     12 - 13      |   31 - 32   |  52 - 53   |

## Picture of the Mandelbrot set

![Picture](Picture.png)
