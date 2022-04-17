#include "../../TX/TXLib.h"
#include "immintrin.h"

#define OUTPUT 1

const int VERT_SIZE = 600;
const int HOR_SIZE  = 800;

#if OUTPUT == 1
typedef RGBQUAD (&scr_t) [VERT_SIZE][HOR_SIZE];
#endif

static inline void Check_Buttons (float *X_C, float *Y_C, float *scale, __m256 *scale_256)
{
    if (txGetAsyncKeyState ('D'))  
        *X_C += 10.f;

    if (txGetAsyncKeyState ('A')) 
        *X_C -= 10.f;

    if (txGetAsyncKeyState ('W'))  
        *Y_C += 10.f;
    
    if (txGetAsyncKeyState ('S'))    
        *Y_C -= 10.f;
    
    if (txGetAsyncKeyState (VK_UP))
    {
        *scale *= 0.8f;
        *scale_256 = _mm256_set1_ps (*scale);
    }
        
    if (txGetAsyncKeyState (VK_DOWN))
    {
        *scale *= 1.25f;
        *scale_256 = _mm256_set1_ps (*scale);
    }    
}

int main (void)
{
    #if OUTPUT == 1
    txCreateWindow (HOR_SIZE, VERT_SIZE);
    Win32::_fpreset();
    txBegin();

    scr_t screen_buff = (scr_t) *txVideoMemory();
    #endif

    const int iters_max  = 256;
    const __m256 R_SQ_MAX = _mm256_set1_ps (100.f);
    const __m256 _76543210 = _mm256_set_ps (7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);

    #if OUTPUT == 1
    const __m256 ITERS_MAX = _mm256_castsi256_ps (_mm256_set1_epi32 (256));
    const __m256 _255  = _mm256_set1_ps (255.f);
    #endif

    float X_C = HOR_SIZE * 0.5f + VERT_SIZE * (5.f / 18.f);
    float Y_C = VERT_SIZE * 0.5f;

    float scale = 0.004f;
    __m256 SCALE = _mm256_set1_ps (scale);

    for (;;)
    {
        if (GetAsyncKeyState (VK_ESCAPE))
            break;

        #if OUTPUT == 1
        Check_Buttons (&X_C, &Y_C, &scale, &SCALE);
        #endif

        for (int y_i = 0; y_i < VERT_SIZE; y_i++) 
        {
            __m256 Y_0 = _mm256_set1_ps ( ((float)y_i - Y_C) * scale );

            for (int x_i = 0; x_i < HOR_SIZE; x_i += 8)
            {                                                                                        
                __m256 X_0 =_mm256_mul_ps ( _mm256_sub_ps ( _mm256_add_ps ( _mm256_set1_ps ( (float)x_i ), _76543210 ), _mm256_set1_ps (X_C) ), SCALE );

                __m256 X = X_0;
                __m256 Y = Y_0;

                __m256i inside_iters = _mm256_setzero_si256 ();
                for (int iters_ctr = 0 ; iters_ctr < iters_max; iters_ctr++)
                {                                       
                    __m256 X_SQ = _mm256_mul_ps (X, X);
                    __m256 Y_SQ = _mm256_mul_ps (Y, Y);

                    __m256 R_SQ = _mm256_add_ps (X_SQ, Y_SQ);
                    
                    __m256 cmp_res = _mm256_cmp_ps (R_SQ_MAX, R_SQ, _CMP_GT_OS);

                    int mask = _mm256_movemask_ps (cmp_res);
                    if (!mask)
                        break;
                        
                    inside_iters = _mm256_sub_epi32 (inside_iters, _mm256_castps_si256 (cmp_res));
                                    
                    __m256 XY = _mm256_mul_ps (X, Y);

                    X = _mm256_add_ps (_mm256_sub_ps (X_SQ, Y_SQ), X_0);
                    Y = _mm256_add_ps (_mm256_add_ps (XY, XY), Y_0);
                }

                #if OUTPUT == 1
                __m256 INSIDE_ITERS = _mm256_castsi256_ps (inside_iters);
                __m256 I = _mm256_mul_ps (_mm256_sqrt_ps (_mm256_sqrt_ps (_mm256_div_ps (INSIDE_ITERS, ITERS_MAX))), _255);

                int *N_ptr = (int *)&inside_iters;         
                float    *I_ptr = (float *)   &I;

                for (int i = 0; i < 8; i++)
                {
                    BYTE c = (BYTE) I_ptr[i];
                    RGBQUAD color = (N_ptr[i] < iters_max) ? RGBQUAD { (BYTE) (255 - c), (BYTE) (c % 2 * 32), c } : RGBQUAD {};

                    screen_buff[y_i][x_i + i] = color;  
                }
                #endif
            }
        }
            
        printf ("%.0f FPS\r", txGetFPS());
        #if OUTPUT == 1
        txUpdateWindow();
        #endif
    }

    txDisableAutoPause(); // txIDontWantToHaveAPauseAfterMyProgramBeforeTheWindowWillClose_AndIWillNotBeAskingWhereIsMyPicture ()

    return 0;
}
