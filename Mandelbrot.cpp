#include "../../TX/TXLib.h"
#include "Mandelbrot.hpp"

static inline RGBQUAD *Create_Window (const int hor_size, const int vert_size)
{
    txCreateWindow (hor_size, vert_size);       // creates window of (hor_size x vert_size); window is centered

    Win32::_fpreset();                          // reinitialization of math coprocessor ()
                                                // TXLib aborts the program if any error accures
                                                // this function returns the state to the initial one 

    RGBQUAD *screen_buff = txVideoMemory ();    // return pointer on the screen buffer as if it was one dimention array

    return screen_buff;
}

//====================================== NOT OPTIMIZED VERSION ======================================//

static inline void Check_Buttons (float *X_C, float *Y_C, float *scale)
{
    const float move_step  = 10.f;
    const float scale_up   = 1.25f;
    const float scale_down = 0.8f;

    if (txGetAsyncKeyState ('A')) 
        *X_C -= move_step;

    if (txGetAsyncKeyState ('W'))  
        *Y_C += move_step;
    
    if (txGetAsyncKeyState ('S'))    
        *Y_C -= move_step;

    if (txGetAsyncKeyState ('D'))  
        *X_C += move_step;
    
    if (txGetAsyncKeyState (VK_UP))
        *scale *= scale_down;
        
    if (txGetAsyncKeyState (VK_DOWN))
        *scale *= scale_up;
}

void Draw_Mandelbrot (const int hor_size, const int vert_size)
{
    assert (hor_size  > 0);
    assert (vert_size > 0);
    
    RGBQUAD *screen_buff = Create_Window (hor_size, vert_size);
    
    const int   iters_max = 256;
    const float r_sq_max  = 100.f;

    float X_C = (float)hor_size  / 2.f + (float)vert_size * (5.f / 18.f);
    float Y_C = (float)vert_size / 2.f;

    float scale = 0.004f;

    for (;;)
    {        
        txUpdateWindow (false); // forbids to update window

        if (GetAsyncKeyState (VK_ESCAPE))
            break;

        Check_Buttons (&X_C, &Y_C, &scale);

        for (int y_i = 0; y_i < vert_size; y_i++) 
        {
            float y_0 = ((float)y_i - Y_C) * scale;

            for (int x_i = 0; x_i < hor_size; x_i++)
            {                                                                                        
                float x_0 = ((float)x_i - X_C) * scale;

                int iter_i = 0;
                for (float x = x_0, y = y_0; iter_i < iters_max; iter_i++)
                {
                    float x_sq = x * x, y_sq = y * y, xy = x * y;

                    float r_sq = x_sq + y_sq;
                    if (r_sq >= r_sq_max)
                        break;
                        
                    x = x_sq - y_sq + x_0;
                    y =   xy +   xy + y_0;
                }
                
                #if VERY_BEATUTIFUL_COLOURS == 1
                float nearly_colour = sin((float)iter_i) * log(1 + (float)iter_i / (float)iters_max) * 255.f;
                #elif VERY_BEATUTIFUL_COLOURS == 0
                float nearly_colour = sqrtf (sqrtf((float)iter_i / (float)iters_max)) * 255.f;
                #endif
                

                RGBQUAD color = (iter_i < iters_max) ? RGBQUAD { (BYTE) (nearly_colour),
                                                                 (BYTE) (nearly_colour * 37), 
                                                                 (BYTE) (255 - nearly_colour) 
                                                               } : RGBQUAD {0, 0, 0};

                *(screen_buff + y_i * hor_size + x_i) = color;
            }
        }
            
        printf ("%.0f FPS\r", txGetFPS());

        txUpdateWindow (true);  // allows to update window
    }
}
//===================================================================================================//

//===================================== FLOAT AVX2 OPTIMIZATION =====================================//

static inline void Check_Buttons_float (float *X_C, __m256 *X_C_AVX, float *Y_C, float *scale, __m256 *scale_256)
{
    const float move_step  = 10.f;
    const float scale_up   = 1.25f;
    const float scale_down = 0.8f;

    if (txGetAsyncKeyState ('A'))
    {
        *X_C -= move_step;
        *X_C_AVX = _mm256_set1_ps (*X_C);
    }

    if (txGetAsyncKeyState ('W'))  
        *Y_C += move_step;
    
    if (txGetAsyncKeyState ('S'))    
        *Y_C -= move_step;

    if (txGetAsyncKeyState ('D'))
    {
        *X_C += move_step;
        *X_C_AVX = _mm256_set1_ps (*X_C);
    }
    
    if (txGetAsyncKeyState (VK_UP))
    {
        *scale *= scale_down;
        *scale_256 = _mm256_set1_ps (*scale);
    }
        
    if (txGetAsyncKeyState (VK_DOWN))
    {
        *scale *= scale_up;
        *scale_256 = _mm256_set1_ps (*scale);
    }    
}

void Draw_Mandelbrot_AVX2_float (const int hor_size, const int vert_size)
{
    assert (hor_size  > 0);
    assert (vert_size > 0);
    
    RGBQUAD *screen_buff = Create_Window (hor_size, vert_size);
    
    const int iters_max    = 256;
    const __m256 ITERS_MAX = _mm256_castsi256_ps (_mm256_set1_epi32 (iters_max));

    const __m256 R_SQ_MAX  = _mm256_set1_ps (100.f);

    const __m256 _76543210 = _mm256_set_ps  (7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
    const __m256 _255  = _mm256_set1_ps (255.f);

    float X_C = (float)hor_size * 0.5f + (float)vert_size * (5.f / 18.f);
    __m256 X_C_AVX = _mm256_set1_ps (X_C);

    float Y_C = (float)vert_size * 0.5f;

    float scale = 0.004f;
    __m256 SCALE = _mm256_set1_ps (scale);

    for (;;)
    {
        txUpdateWindow (false);

        if (GetAsyncKeyState (VK_ESCAPE))
            break;

        Check_Buttons_float (&X_C, &X_C_AVX, &Y_C, &scale, &SCALE);

        for (int y_i = 0; y_i < vert_size; y_i++)
        {
            __m256 Y_0 = _mm256_set1_ps ( ((float)y_i - Y_C) * scale );

            for (int x_i = 0; x_i < hor_size; x_i += 8)
            {                                                                                        
                __m256 X_0 =_mm256_mul_ps ( _mm256_sub_ps (_mm256_add_ps ( _mm256_set1_ps ( (float)x_i ), _76543210), X_C_AVX), SCALE );

                __m256 X = X_0;
                __m256 Y = Y_0;

                __m256i inside_iters = _mm256_setzero_si256 ();
                for (int iters_ctr = 0 ; iters_ctr < iters_max; iters_ctr++)
                {                                       
                    __m256 X_SQ = _mm256_mul_ps (X, X);
                    __m256 Y_SQ = _mm256_mul_ps (Y, Y);

                    __m256 R_SQ = _mm256_add_ps (X_SQ, Y_SQ);
                    
                    __m256 cmp_res = _mm256_cmp_ps (R_SQ_MAX, R_SQ, _CMP_GT_OQ); // _CMP_GT_OQ - compare if greater of equal
                                                                                 // O - ordered (can also be unordered)
                                                                                 //     returns false after comparing with NaN
                                                                                 // Q - quiet (can also be signaling)
                                                                                 //     Way of dealing with NaN
                    // if comparison between two floats is returns true, a float in the dst is set as -1, else - as 0.

                    int mask = _mm256_movemask_ps (cmp_res); // if a float is 0, appropriate bit of the mask is set as 0, else - as 1.
                    if (!mask)
                        break;
                        
                    inside_iters = _mm256_sub_epi32 (inside_iters, _mm256_castps_si256 (cmp_res));
                                    
                    __m256 XY = _mm256_mul_ps (X, Y);

                    X = _mm256_add_ps (_mm256_sub_ps (X_SQ, Y_SQ), X_0);
                    Y = _mm256_add_ps (_mm256_add_ps (XY, XY), Y_0);
                }

                __m256 INSIDE_ITERS = _mm256_castsi256_ps (inside_iters);
                __m256 I = _mm256_mul_ps (_mm256_sqrt_ps (_mm256_sqrt_ps (_mm256_div_ps (INSIDE_ITERS, ITERS_MAX))), _255);

                uint32_t *N_ptr = (uint32_t *)&inside_iters;         
                float    *I_ptr = (float *)&I;

                for (int i = 0; i < 8; i++)
                {
                    BYTE nearly_colour = (BYTE) I_ptr[i];
                    RGBQUAD color = (N_ptr[i] < iters_max) ? RGBQUAD { (BYTE) (nearly_colour), 
                                                                       (BYTE) (nearly_colour * 37), 
                                                                       (BYTE) (255 - nearly_colour) 
                                                                     } : RGBQUAD {0, 0, 0};

                    *(screen_buff + y_i * hor_size + x_i + i) = color;
                }
            }
        }
            
        printf ("%.0f FPS\r", txGetFPS());
        
        txUpdateWindow (true);
    }

    txDisableAutoPause(); 
    // txIDontWantToHaveAPauseAfterMyProgramBeforeTheWindowWillClose_AndIWillNotBeAskingWhereIsMyPicture ()
}

//===================================================================================================//

//===================================== DOUBLE AVX2 OPTIMIZATION ====================================//

static inline void Check_Buttons_double (double *X_C, __m256d *X_C_AVX, double *Y_C, double *scale, __m256d *scale_256)
{
    const double move_step  = 10.0;
    const double scale_up   = 1.25;
    const double scale_down = 0.8;

    if (txGetAsyncKeyState ('A'))
    {
        *X_C -= move_step;
        *X_C_AVX = _mm256_set1_pd (*X_C);
    }

    if (txGetAsyncKeyState ('W'))  
        *Y_C += move_step;
    
    if (txGetAsyncKeyState ('S'))    
        *Y_C -= move_step;

    if (txGetAsyncKeyState ('D'))
    {
        *X_C += move_step;
        *X_C_AVX = _mm256_set1_pd (*X_C);
    }
    
    if (txGetAsyncKeyState (VK_UP))
    {
        *scale *= scale_down;
        *scale_256 = _mm256_set1_pd (*scale);
    }
        
    if (txGetAsyncKeyState (VK_DOWN))
    {
        *scale *= scale_up;
        *scale_256 = _mm256_set1_pd (*scale);
    }
}

void Draw_Mandelbrot_AVX2_double (const int hor_size, const int vert_size)
{
    assert (hor_size  > 0);
    assert (vert_size > 0);
    
    RGBQUAD *screen_buff = Create_Window (hor_size, vert_size);

    const int iters_max     = 256;
    const __m256d ITERS_MAX = _mm256_castsi256_pd (_mm256_set1_epi64x (256));

    const __m256d R_SQ_MAX = _mm256_set1_pd (100.0);

    const __m256d _3210 = _mm256_set_pd (3.0, 2.0, 1.0, 0.0);
    const __m256d _255  = _mm256_set1_pd (255.0);

    double X_C = (double)hor_size * 0.5 + (double)vert_size * (5.0 / 18.0);
    __m256d X_C_AVX = _mm256_set1_pd (X_C);

    double Y_C = (double)vert_size * 0.5;

    double scale = 0.004;
    __m256d SCALE = _mm256_set1_pd (scale);

    for (;;)
    {
        txUpdateWindow (false);

        if (GetAsyncKeyState (VK_ESCAPE))
            break;

        Check_Buttons_double (&X_C, &X_C_AVX, &Y_C, &scale, &SCALE);

        for (int y_i = 0; y_i < vert_size; y_i++) 
        {            
            __m256d Y_0 = _mm256_set1_pd ( ((double)y_i - Y_C) * scale );

            for (int x_i = 0; x_i < hor_size; x_i += 4)
            {                                                                                        
                __m256d X_0 =_mm256_mul_pd ( _mm256_sub_pd ( _mm256_add_pd ( _mm256_set1_pd ( (double)x_i ), _3210 ), X_C_AVX ), SCALE );

                __m256d X = X_0;
                __m256d Y = Y_0;

                __m256i inside_iters = _mm256_setzero_si256 ();
                for (int iters_ctr = 0 ; iters_ctr < iters_max; iters_ctr++)
                {                                       
                    __m256d X_SQ = _mm256_mul_pd (X, X);
                    __m256d Y_SQ = _mm256_mul_pd (Y, Y);

                    __m256d R_SQ = _mm256_add_pd (X_SQ, Y_SQ);
                    
                    __m256d cmp_res = _mm256_cmp_pd (R_SQ_MAX, R_SQ, _CMP_GT_OS);

                    int mask = _mm256_movemask_pd (cmp_res);
                    if (!mask)
                        break;
                        
                    inside_iters = _mm256_sub_epi64 (inside_iters, _mm256_castpd_si256 (cmp_res));
                                    
                    __m256d XY = _mm256_mul_pd (X, Y);

                    X = _mm256_add_pd (_mm256_sub_pd (X_SQ, Y_SQ), X_0);
                    Y = _mm256_add_pd (_mm256_add_pd (XY, XY), Y_0);
                }

                __m256d INSIDE_ITERS = _mm256_castsi256_pd (inside_iters);
                __m256d I = _mm256_mul_pd (_mm256_sqrt_pd (_mm256_sqrt_pd (_mm256_div_pd (INSIDE_ITERS, ITERS_MAX))), _255);

                uint64_t *N_ptr = (uint64_t *)&inside_iters;         
                double    *I_ptr = (double *)   &I;

                for (int i = 0; i < 4; i++)
                {
                    BYTE nearly_colour = (BYTE) I_ptr[i];
                    RGBQUAD color = (N_ptr[i] < iters_max) ? RGBQUAD { (BYTE) (nearly_colour), 
                                                                       (BYTE) (nearly_colour * 37), 
                                                                       (BYTE) (255 - nearly_colour) 
                                                                     } : RGBQUAD {0, 0, 0};

                    *(screen_buff + y_i * hor_size + x_i + i) = color;
                }
            }
        }
                    
        printf ("%.0f FPS\r", txGetFPS ());

        txUpdateWindow (true);
    }

    txDisableAutoPause(); 
    // txIDontWantToHaveAPauseAfterMyProgramBeforeTheWindowWillClose_AndIWillNotBeAskingWhereIsMyPicture ()
}

//===================================================================================================//