#include "Mandelbrot.hpp"

int main (int argc, char *argv[])
{
    if (argc < 3)
    {
        printf ("You haven't set the size of the window\n");
        exit (EXIT_FAILURE);
    }
    else if (argc > 3)
    {
        printf ("There are too many arguments for main ()\n");
        exit (EXIT_FAILURE);
    }
    
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

    txDisableAutoPause(); 
    // txIDontWantToHaveAPauseAfterMyProgramBeforeTheWindowWillClose_AndIWillNotBeAskingWhereIsMyPicture ()

    return 0;
}
