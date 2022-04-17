# Mandelbrot Set

This project is C++ implementation of the Mandelbrot set renderer. The feature the project is using Intel<sup>&reg;</sup> Intrinsics for optimization of calculations.

## Dependencies

Visulization of the Mandelbrot set is implemented by [TXLib](https://github.com/ded32/TXLib) by [Ilya Dedinsky (aka ded 32)](https://github.com/ded32).

## Results

1. **No output, static picture**

|     | No optimizations | AVX2 double | AVX2 float |
|-----|------------------|-------------|------------|
| FPS |       7-8        |   300-500   | 400 - 700  |

2. **Output, static picture**

|     | No optimizations | AVX2 double | AVX2 float |
|-----|------------------|-------------|------------|
| FPS |        4         |   25 - 40   |  30 - 50   |

3. **Output, moving picture**

|     | No optimizations | AVX2 double | AVX2 float |
|-----|------------------|-------------|------------|
| FPS |       4-7        |   25 - 40   |  30 - 50   |

## Picture of the Mandelbrot set

![Picture](Picture.png)
