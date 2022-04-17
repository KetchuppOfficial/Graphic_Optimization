#include "../../TX/TXLib.h"
#include "immintrin.h"

#define OUTPUT 1

const int VERT_SIZE = 1080;
const int HOR_SIZE  = 1920;

#if OUTPUT == 1
typedef RGBQUAD (&scr_t) [VERT_SIZE][HOR_SIZE];
#endif

static inline void Check_Buttons (double *X_C, double *Y_C, double *scale, __m256d *scale_256)
{
    if (txGetAsyncKeyState ('D'))  
        *X_C += 10.0;

    if (txGetAsyncKeyState ('A')) 
        *X_C -= 10.0;

    if (txGetAsyncKeyState ('W'))  
        *Y_C += 10.0;
    
    if (txGetAsyncKeyState ('S'))    
        *Y_C -= 10.0;
    
    if (txGetAsyncKeyState (VK_UP))
    {
        *scale *= 0.8;
        *scale_256 = _mm256_set1_pd (*scale);
    }
        
    if (txGetAsyncKeyState (VK_DOWN))
    {
        *scale *= 1.25;
        *scale_256 = _mm256_set1_pd (*scale);
    }    
}

int main (void)
{
    #if OUTPUT == 1
    txCreateWindow (HOR_SIZE, VERT_SIZE); // создание окна 1920x1080
    Win32::_fpreset();
    txBegin();                 // блокирует обновление изображения окна во избежание миганий

    scr_t screen_buff = (scr_t) *txVideoMemory(); // буфер экрана
    #endif

    const int iters_max  = 256;
    const __m256d R_SQ_MAX = _mm256_set1_pd (100.0);
    const __m256d _3210 = _mm256_set_pd (3.0, 2.0, 1.0, 0.0);

    #if OUTPUT == 1
    const __m256d ITERS_MAX = _mm256_castsi256_pd (_mm256_set1_epi64x (256));
    const __m256d _255  = _mm256_set1_pd (255.0);
    #endif

    double X_C = HOR_SIZE * 0.5 + VERT_SIZE * (5.0 / 18.0);
    double Y_C = VERT_SIZE * 0.5;

    double scale = 0.005;
    __m256d SCALE = _mm256_set1_pd (scale);

    for (;;)
    {
        if (GetAsyncKeyState (VK_ESCAPE))
            break;

        #if OUTPUT == 1
        Check_Buttons (&X_C, &Y_C, &scale, &SCALE);
        #endif

        for (int y_i = 0; y_i < VERT_SIZE; y_i++) 
        {
            __m256d Y_0 = _mm256_set1_pd ( ((double)y_i - Y_C) * scale );

            for (int x_i = 0; x_i < HOR_SIZE; x_i += 4)
            {                                                                                        
                __m256d X_0 =_mm256_mul_pd ( _mm256_sub_pd ( _mm256_add_pd ( _mm256_set1_pd ( (double)x_i ), _3210 ), _mm256_set1_pd (X_C) ), SCALE );

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

                #if OUTPUT == 1
                __m256d INSIDE_ITERS = _mm256_castsi256_pd (inside_iters);
                __m256d I = _mm256_mul_pd (_mm256_sqrt_pd (_mm256_sqrt_pd (_mm256_div_pd (INSIDE_ITERS, ITERS_MAX))), _255);

                long long *N_ptr = (long long *)&inside_iters;         
                double    *I_ptr = (double *)   &I;

                for (int i = 0; i < 4; i++)
                {
                    BYTE c = (BYTE) I_ptr[i];
                    RGBQUAD color = (N_ptr[i] < iters_max) ? RGBQUAD { (BYTE) (255 - c), (BYTE) (c % 2 * 64), c } : RGBQUAD {};

                    screen_buff[y_i][x_i + i] = color;  
                }
                #endif
            }
        }
            
        printf ("\033[0;0H\033[2K %.0f FPS", txGetFPS());
        #if OUTPUT == 1
        txUpdateWindow(); // разрешить обновление экрана
        #endif
    }

    txDisableAutoPause(); // txIDontWantToHaveAPauseAfterMyProgramBeforeTheWindowWillClose_AndIWillNotBeAskingWhereIsMyPicture ()

    return 0;
}
