![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

# Mandelbrot Set

This project is C++ implementation of the Mandelbrot set renderer. The feature of the project is using Intel<sup>&reg;</sup> Intrinsics for optimization of calculations. This program is the first part of one task, the second one is [Alpha Blending](https://github.com/KetchuppOfficial/Alpha_Blending).

## Dependencies

Visulization of the Mandelbrot set is implemented by [TXLib](https://github.com/ded32/TXLib) by [Ilya Dedinsky (aka ded32)](https://github.com/ded32).

## Build and run

This projects is developed for Windows only.

First of all, download this repository:
```bash
git@github.com:KetchuppOfficial/Mandelbort_Set.git
cd Mandelbrot_Set
```

Compile the program using tool **make**. You can choose one of 3 modes:
```bash
make                    # for basic mode
make MODE=-DDOUBLE      # for AVX2 double mode
make MODE=-DFLOAT       # for AVX2 float mode
```

You can also choose on of compiler optimization flags as in the example:
```bash
make OPT=-O2
```

To run the program, use **make** again:
```bash
make run W=1920 H=1080
```
**W** - width, **H** - height.

## Results

Measurements were carried out on a 800x600 screen. Mandelbot scale was 0.004 and the picture was static.

**FPS rate:**

| Optimization key | No optimizations | AVX2 double | AVX2 float |
|------------------|------------------|-------------|------------|
|       None       |        2         |      3      |     5      |
|       -O1        |        5         |      13     |     19     |
|       -O2        |        5         |      13     |     19     |
|       -O3        |        5         |      13     |     19     |

## Picture of the Mandelbrot set

![Picture](Mandelbrot.png)
