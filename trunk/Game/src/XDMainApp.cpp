/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*/
#include "pch.h"
#include <chrono>
#include "XDGame.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

HINSTANCE g_Instance;
HWND g_Wnd;
double g_fps_measure;
XDGame Game;

bool g_toggle_line;
bool g_toogle_graphics;
int g_exit_app=0;
int g_backkey_pressed=0;

std::string g_rw_local_folder;
std::string g_ro_local_folder;
float g_screen_w;
float g_screen_h;

static LRESULT WINAPI WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
    case WM_CREATE:
        g_Wnd = wnd;
        g_screen_w = 480;
        g_screen_h = 700;
        Game.Initialize(g_Wnd);
        return 0;
    case WM_DESTROY:
        Game.ShutDown();
        PostQuitMessage(0);
        return 0;
    case WM_MOUSEMOVE: {
        SINPUT input;
        input.pposx =   ( LOWORD( lParam ) );
        input.pposy =   ( HIWORD( lParam ) );
        input.pflag =   FINPUT::P_MOVE;
        Game.OnHandleInput(input);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        SINPUT input;
        input.pposx =   ( LOWORD( lParam ) );
        input.pposy =   ( HIWORD( lParam ) );
        input.pflag =   FINPUT::P_PRESSED;
        Game.OnHandleInput(input);
        return 0;
    }
    case WM_LBUTTONUP: {
        SINPUT input;
        input.pposx =   ( LOWORD( lParam ) );
        input.pposy =   ( HIWORD( lParam ) );
        input.pflag =   FINPUT::P_RELEASED;
        Game.OnHandleInput(input);
        return 0;
    }
    case WM_KEYDOWN: {
        WPARAM param = wParam;
        SINPUT input;
        input.pflag = FINPUT::KEY_DOWN;
        input.pid   = param ;
        Game.OnHandleInput(input);
        if(input.pid == 71 ) {
            g_toogle_graphics = !g_toogle_graphics;
        }
        if(input.pid == 76 ) {
            g_toggle_line = !g_toggle_line;
        }
        if(input.pid == 27 ) {
            g_backkey_pressed = 1;
        }
        return 0;
    }
    case WM_KEYUP : {
        WPARAM param = wParam;
        SINPUT input;
        input.pflag = FINPUT::KEY_UP;
        input.pid   = param ;
        Game.OnHandleInput(input);
    }
    return 0;
    }
    return DefWindowProc(wnd, msg, wParam, lParam);
}

//int WINAPI WinMain(HINSTANCE instance, HINSTANCE, char* cmdLine, int showCmd)
int main() {
    g_toggle_line =false;
    g_toogle_graphics=true;
    g_exit_app = 0;
    g_rw_local_folder ="\\data\\";
    //g_Instance = instance;
    g_Instance = GetModuleHandle(0);
    WNDCLASSEX wndClass;
    ZeroMemory(&wndClass, sizeof(wndClass));
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    wndClass.hbrBackground = NULL;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hInstance = g_Instance;
    wndClass.lpfnWndProc = &WndProc;
    wndClass.lpszClassName = XDGame::Property::WindowTitleWCHAR();
    ATOM classR = RegisterClassEx(&wndClass);
    assert(classR);
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
    DWORD exStyle = 0;
    RECT rect = { 0, 0, (long) XDGame::Property::Windows::Width, (long) XDGame::Property::Windows::Height };
    AdjustWindowRectEx(&rect, style, FALSE, exStyle);
    g_Wnd = CreateWindowEx(
                exStyle,
                XDGame::Property::WindowTitleWCHAR(),
                XDGame::Property::WindowTitleWCHAR(),
                style,
                CW_USEDEFAULT, CW_USEDEFAULT,
                rect.right - rect.left, rect.bottom - rect.top,
                NULL,
                NULL,
                g_Instance,
                0);
    assert(g_Wnd);
    steady_clock::time_point _time_old = steady_clock::now();
    steady_clock::time_point _time_now = _time_old;
    double _min_delta = 1 / 60.0f ;
    MSG msg;
    g_fps_measure = 0;
    double buff_g_fps_measure = 0;
    int ifps = 0;
    steady_clock::time_point tp1= steady_clock::now();
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, 1);
    while( !g_exit_app ) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            steady_clock::time_point tp1= steady_clock::now();
            steady_clock::time_point _time_now = steady_clock::now();
            double _delta_time_seconds = ( duration_cast<milliseconds>(_time_now - _time_old).count() ) / 1000.0f;
            _delta_time_seconds= _delta_time_seconds > _min_delta ? _min_delta : _delta_time_seconds ;
            _time_old = _time_now;
            Game.Update(_delta_time_seconds);
            Game.Render();
            //OutputDebugString("test\n");
#define SAMPLE_FPS 60
            steady_clock::time_point tp2= steady_clock::now();
            buff_g_fps_measure = buff_g_fps_measure + ( 1000.0f / ( duration_cast<milliseconds>(tp2 - tp1).count() ) );
            if(ifps++ == SAMPLE_FPS ) {
                g_fps_measure = buff_g_fps_measure / SAMPLE_FPS;
                ifps = 0;
                buff_g_fps_measure =0;
            }
        }
    }
    return (int)msg.wParam;
}
