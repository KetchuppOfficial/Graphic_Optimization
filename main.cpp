#include "Mandelbrot.hpp"

int Check_Argc (int argc)
{
    if (argc < 3)
    {
        printf ("You haven't set the size of the window\n");
        return -1;
    }
    else if (argc > 3)
    {
        printf ("There are too many arguments for main ()\n");
        return -1;
    }

    return 0;
}

int main (int argc, char *argv[])
{
    if (Check_Argc (argc) == -1)
        return 1;
    
    #ifndef DOUBLE
    #ifndef FLOAT
    Draw_Mandelbrot             (atoi (argv[1]), atoi (argv[2]));
    #endif
    #endif

    #ifdef DOUBLE
    Draw_Mandelbrot_AVX2_double (atoi (argv[1]), atoi (argv[2]));
    #endif

    #ifdef FLOAT
    Draw_Mandelbrot_AVX2_float  (atoi (argv[1]), atoi (argv[2]));
    #endif

    return 0;
}
