#include "../../TX/TXLib.h"

#define OUTPUT 1

const int VERT_SIZE  = 600;
const int HOR_SIZE = 800;

#if OUTPUT == 1
typedef RGBQUAD (&scr_t) [VERT_SIZE][HOR_SIZE];
#endif

static inline void Check_Buttons (float *X_C, float *Y_C, float *scale)
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
        *scale *= 0.8f;
        
    if (txGetAsyncKeyState (VK_DOWN))
        *scale *= 1.25f;
}

int main (void)
{
    #if OUTPUT == 1
    txCreateWindow (HOR_SIZE, VERT_SIZE); // создание окна 1920x1080
    Win32::_fpreset();
    txBegin();                 // блокирует обновление изображения окна во избежание миганий

    scr_t screen_buff = (scr_t) *txVideoMemory(); // буфер экрана
    #endif

    const int   n_max  = 256;
    const float r_sq_max = 100.f;

    float X_C = HOR_SIZE / 2.f + VERT_SIZE * (5.f / 18.f);   // Положение начала коондинат системы, связанной с множеством
    float Y_C = VERT_SIZE / 2.f;          // Мандельброта в системе координат буфера экрана

    float scale = 0.002f; // коэффициент увеличения

    for (;;)
    {
        #if OUTPUT == 1
        if (GetAsyncKeyState (VK_ESCAPE)) // проверяет, нажата ли клавиша Esc
            break;

        Check_Buttons (&X_C, &Y_C, &scale);
        #endif

        for (int y_i = 0; y_i < VERT_SIZE; y_i++) 
        {
            float y_0 = ((float)y_i - Y_C) * scale;

            for (int x_i = 0; x_i < HOR_SIZE; x_i++)
            {                                                                                        
                float x_0 = ((float)x_i - X_C) * scale;

                int N = 0;
                for (float x = x_0, y = y_0; N < n_max; N++)
                {
                    float x_sq = x * x, y_sq = y * y, xy = x * y;

                    float r_sq = x_sq + y_sq;
                    if (r_sq >= r_sq_max)
                        break;
                        
                    x = x_sq - y_sq + x_0;
                    y = xy + xy + y_0;
                }
                
                #if OUTPUT == 1
                float I = sqrtf (sqrtf ((float)N / (float)n_max)) * 255.f;

                BYTE c = (BYTE) I;
                RGBQUAD color = (N < n_max) ? RGBQUAD { (BYTE) (255 - c), (BYTE) (c%2 * 64), c } : RGBQUAD {};
                screen_buff[y_i][x_i] = color;
                #endif
            }
        }
            
        printf ("\033[0;0H %.0f FPS\033[1;0H", txGetFPS());
        #if OUTPUT == 1
        txUpdateWindow(); // разрешить обновление экрана
        #endif
    }

    #if OUTPUT == 1
    txDisableAutoPause(); // txIDontWantToHaveAPauseAfterMyProgramBeforeTheWindowWillClose_AndIWillNotBeAskingWhereIsMyPicture ()
    #endif

    return 0;
}
