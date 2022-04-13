#include "../../TX/TXLib.h"

#define VERT_SIZE 1080
#define HOR_SIZE 1920

int main (void)
{
    txCreateWindow (HOR_SIZE, VERT_SIZE); // создание окна 1920x1080
    //Win32::_fpreset();
    txBegin();                 // блокирует обновление изображения окна во избежание миганий

    typedef RGBQUAD (&scr_t) [VERT_SIZE][HOR_SIZE];
    scr_t screen_buff = (scr_t) *txVideoMemory(); // буфер экрана

    const int   n_max  = 256;
    const float r_sq_max = 100.f;

    float X_C = HOR_SIZE / 2.f + 300.f;   // Положение начала коондинат системы, связанной с множеством
    float Y_C = VERT_SIZE / 2.f;          // Мандельброта в системе координат буфера экрана

    float scale = 450.f; // коэффициент увеличения

    for (;;)
    {
        if (GetAsyncKeyState (VK_ESCAPE)) // проверяет, нажата ли клавиша Esc
            break;

        if (txGetAsyncKeyState ('D'))  
            X_C += ( txGetAsyncKeyState (VK_SHIFT) ? 100.f : 10.f );

        if (txGetAsyncKeyState ('A')) 
            X_C -= ( txGetAsyncKeyState (VK_SHIFT) ? 100.f : 10.f );

        if (txGetAsyncKeyState ('W'))  
            Y_C += ( txGetAsyncKeyState (VK_SHIFT) ? 100.f : 10.f );
        
        if (txGetAsyncKeyState ('S'))    
            Y_C -= ( txGetAsyncKeyState (VK_SHIFT) ? 100.f : 10.f );
        
        if (txGetAsyncKeyState (VK_UP))      
            scale += ( txGetAsyncKeyState (VK_SHIFT) ? 100.f : 10.f );
        
        if (scale > 20 && txGetAsyncKeyState (VK_DOWN))      
            scale -= ( txGetAsyncKeyState (VK_SHIFT) ? 100.f : 10.f );

        for (int y_i = 0; y_i < VERT_SIZE; y_i++) 
        {
            if (GetAsyncKeyState (VK_ESCAPE)) 
                break;
 
            float y_0 = ((float)y_i - Y_C) / scale;

            for (int x_i = 0; x_i < HOR_SIZE; x_i++)
            {                                                                                        
                float x_0 = ((float)x_i - X_C) / scale;

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
                
                float I = sqrtf (sqrtf ((float)N / (float)n_max)) * 255.f;

                BYTE c = (BYTE) I;
                RGBQUAD color = (N < n_max) ? RGBQUAD { (BYTE) (255 - c), (BYTE) (c%2 * 64), c } : RGBQUAD {};
                screen_buff[y_i][x_i] = color;
            }
        }
            
        printf ("\t\r%.flf", txGetFPS());
        txUpdateWindow(); // разрешить обновление экрана
    }

    txDisableAutoPause(); // txIDontWantToHaveAPauseAfterMyProgramBeforeTheWindowWillClose_AndIWillNotBeAskingWhereIsMyPicture ()
}
