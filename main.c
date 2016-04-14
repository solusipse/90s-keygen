#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <mmsystem.h>

#include "resource.h"
#include "ufmod/ufmod.h"


// animation speed in ms
const int anim_speed = 75;

// Main winapi program's instance
HINSTANCE instance;
HDC hdc, canvas;

// images related settings
HBITMAP images[2];
int frame = 0;


BOOL CALLBACK MainDialog(HWND d, UINT m, WPARAM w, LPARAM l) {
    switch(m)
        case WM_INITDIALOG: {
            /* Start playing module file with R_MUSIC id
             * see resource.h and resource.rc for more info
             */
            uFMOD_PlaySong((void*)R_MUSIC, instance, XM_RESOURCE);

            // set window to borderless
            SetWindowLong(d, GWL_EXSTYLE, GetWindowLong(d, GWL_EXSTYLE) | WS_EX_LAYERED);

            // select transparency color
            SetLayeredWindowAttributes(d, RGB(255,0,255), 0, LWA_COLORKEY);

            // initialize image array
            unsigned int i;
            for (i=0;i<sizeof(images)/sizeof(images[0]);i++) {
                images[i] = LoadBitmap(instance, MAKEINTRESOURCE(R_BG_1 + i));
            }

            // set canvas for drawing images
            canvas = CreateCompatibleDC(hdc);

            PAINTSTRUCT ps;
            hdc = BeginPaint(d, &ps);

            // create animation thread
            SetTimer(d, 1, 100, NULL);

            // set icons if available (small and large)
            HICON is, il;
            is = LoadIcon(instance, MAKEINTRESOURCE(R_ICON1616));
            il = LoadIcon(instance, MAKEINTRESOURCE(R_ICON3232));
            if(is)
                SendMessage(d, WM_SETICON, ICON_SMALL, (LPARAM)is);
            if(il)
                SendMessage(d, WM_SETICON, ICON_BIG, (LPARAM)il);

            break;

        case WM_ERASEBKGND:


            break;

        case WM_PAINT:
            SelectObject(canvas, images[frame]);
            StretchBlt(hdc, 0, 0, 640, 320, canvas, 0, 0, 64, 32, SRCCOPY);

            break;

        case WM_TIMER:
            SelectObject(canvas, images[frame]);
            RedrawWindow(d, NULL, NULL, 1);

            frame += 1;

            if (frame == sizeof(images)/sizeof(images[0]))
                frame = 0;
            
            break;

        case WM_LBUTTONDOWN:
            SendMessage(d, WM_NCLBUTTONDOWN, HTCAPTION, 0);

            break;

        case WM_CLOSE:
            uFMOD_StopSong();
            EndDialog(d, 0);
            break;

    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE curinst, HINSTANCE previnst, LPSTR cmd, int c) {
    /* This is winapi's main function, we instantiate the main dialog here
     * which is described in resource.rc
     */
    instance=curinst;
    return DialogBox(curinst, MAKEINTRESOURCE(R_MAINDIALOG), NULL, (DLGPROC)MainDialog);
}
