//
// Created by Andrew Huang on 7/9/2022.
//

#include <Windows.h>
#include <glad/gl.h>
#include <glad/wgl.h>

#include "game.h"
#include "input/keyboard.h"
#include "system/timer.h"

static auto ENGINE_NAME = TEXT("Game Engine");

HWND g_hWnd = nullptr;


static inline wchar_t *ToWideStr(const char *multiByteStr) {
    int wideStrLen = MultiByteToWideChar(CP_UTF8, 0, multiByteStr, -1, nullptr, 0);

    auto wideStrBuffer = new wchar_t[wideStrLen];
    MultiByteToWideChar(CP_UTF8, 0, multiByteStr, -1, wideStrBuffer, wideStrLen);
    return wideStrBuffer;
}

void RequestQuit() {
    PostQuitMessage(0);
}

void SetVSync(bool enabled) {
    if (!GLAD_WGL_EXT_swap_control) return;
    wglSwapIntervalEXT(enabled ? (GLAD_WGL_EXT_swap_control_tear ? -1 : 1) : 0);
}

void SetTitle(const std::string &title) {
    if (!g_hWnd) {
        return;
    }

#ifdef UNICODE

    auto wideStr = ToWideStr(title.c_str());
    SetWindowText(g_hWnd, wideStr);
    delete[] wideStr;

#else

    SetWindowText(g_hWnd, title.c_str());

#endif
}

IntVec2 CalcAdjustedWindowSize(const IntVec2 &size, DWORD dwStyle) {
    RECT rc{0, 0, size.x, size.y};
    AdjustWindowRect(&rc, dwStyle, false);
    return {rc.right - rc.left, rc.bottom - rc.top};
}

// win32 message callback
LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    auto game = reinterpret_cast<Game *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (!game) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    switch (message) {
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
    case WM_KEYDOWN:
    case WM_KEYUP:
        Keyboard::ProcessMessage(message, wParam, lParam);
        break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_SYSCHAR:
        Keyboard::ProcessMessage(message, wParam, lParam);
        // pressing alt will trigger the system menu and the game will freeze
        // here I simply disabled all system shortcuts
        // there might be a better way to handle this
        return 0;
    case WM_SIZE:
        game->OnResize({LOWORD(lParam), HIWORD(lParam)});
        break;
    case WM_GETMINMAXINFO:
        if (lParam) {
            IntVec2 size = CalcAdjustedWindowSize({320, 240}, WS_OVERLAPPEDWINDOW);

            auto info = reinterpret_cast<MINMAXINFO *>(lParam);
            info->ptMinTrackSize.x = size.x;
            info->ptMinTrackSize.y = size.y;
        }
        break;
    case WM_CLOSE:
        game->OnClose();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MENUCHAR:
        return MAKELRESULT(0, MNC_CLOSE);
    default:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nShowCmd) {
    // create window class

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wc.lpszClassName = ENGINE_NAME;
    if (!RegisterClass(&wc)) {
        MessageBox(nullptr, TEXT("Failed to register window class!"), ENGINE_NAME, MB_ICONERROR);
        return nullptr;
    }

    // calculate window size

    IntVec2 size = CalcAdjustedWindowSize({800, 600}, WS_OVERLAPPEDWINDOW);

    // create window

    HWND hWnd = CreateWindow(
            ENGINE_NAME, // lpClassName
            TEXT("Game Engine"), // lpWindowName
            WS_OVERLAPPEDWINDOW, // dwStyle
            CW_USEDEFAULT, // x
            CW_USEDEFAULT, // y
            size.x, // nWidth
            size.y, // nHeight
            nullptr, // hWndParent
            nullptr, // hMenu
            hInstance, // hInstance
            nullptr // lpParam
    );
    if (!hWnd) {
        MessageBox(nullptr, TEXT("Failed to create window!"), ENGINE_NAME, MB_ICONERROR);
        return nullptr;
    }

    ShowWindow(hWnd, nShowCmd);

    return hWnd;
}

HGLRC CreateGLContext(HWND hWnd, HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.dwFlags      = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;
    pfd.cDepthBits   = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType   = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
        MessageBox(hWnd, TEXT("Failed to set compatible pixel format!"), ENGINE_NAME, MB_ICONERROR);
        return nullptr;
    }

    // temporary opengl context
    HGLRC tempContext = wglCreateContext(hdc);
    if (!tempContext) {
        MessageBox(hWnd, TEXT("Failed to create initial gl context!"), ENGINE_NAME, MB_ICONERROR);
        return nullptr;
    }

    // load glad wgl
    wglMakeCurrent(hdc, tempContext);
    gladLoaderLoadWGL(hdc);

    // request opengl 4.6 core profile
    constexpr int wglAttributes[]{
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
    };

    // final opengl context
    HGLRC glContext = wglCreateContextAttribsARB(hdc, nullptr, wglAttributes);
    if (!glContext) {
        MessageBox(hWnd, TEXT("Failed to create final gl context!"), ENGINE_NAME, MB_ICONERROR);
        return nullptr;
    }

    // switch to final context
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tempContext);
    wglMakeCurrent(hdc, glContext);

    // load glad gl
    if (!gladLoaderLoadGL()) {
        MessageBox(hWnd, TEXT("Failed to load glad!"), ENGINE_NAME, MB_ICONERROR);
        return nullptr;
    }

    return glContext;
}

void MainLoop(HWND hWnd, HDC hdc) {
    auto game = NewGame();
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(game.get()));

    {
        // trigger the initial OnResize

        RECT rc;
        GetClientRect(hWnd, &rc);
        game->OnResize({rc.right - rc.left, rc.bottom - rc.top});
    }

    // frame timer
    Timer timer;
    bool  running = true;
    while (running) {
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
        }

        Keyboard::Update();

        game->OnTick(static_cast<float>(timer.tick()));

        SwapBuffers(hdc);
    }

    SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    HWND &hWnd = g_hWnd;

    hWnd = CreateGameWindow(hInstance, nShowCmd);
    if (!hWnd) {
        return -1;
    }

    HDC hdc = GetDC(hWnd);
    if (!hdc) {
        MessageBox(hWnd, TEXT("Failed to get device context!"), ENGINE_NAME, MB_ICONERROR);
        return -1;
    }

    HGLRC glContext = CreateGLContext(hWnd, hdc);
    if (!glContext) {
        return -1;
    }

    MainLoop(hWnd, hdc);

    // release resources
    wglDeleteContext(glContext);
    ReleaseDC(hWnd, hdc);
    DestroyWindow(hWnd);

    return 0;
}
