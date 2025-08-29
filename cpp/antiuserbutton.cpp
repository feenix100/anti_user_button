#include <windows.h>
#include <windowsx.h>
#include <random>
#include <cmath>

static constexpr int   BTN_W        = 200;
static constexpr int   BTN_H        = 50;
static constexpr float NEAR_DIST    = 100.0f;
static constexpr float NUDGE_NEAR   = 100.0f;
static constexpr float NUDGE_FAR    = 10.0f;
static constexpr UINT  TICK_MS      = 33;       // ~30fps jitter
static constexpr float JITTER_RANGE = 10.0f;

struct AppState {
    HWND  hButton = nullptr;   // exactly one
    float x = 0.0f;
    float y = 0.0f;
    int   cw = 800;
    int   ch = 600;
    std::mt19937 rng{ std::random_device{}() };
};

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void place_button(HWND hwndParent, AppState* s, bool repaint = true) {
    float maxX = static_cast<float>(std::max(0, s->cw - BTN_W));
    float maxY = static_cast<float>(std::max(0, s->ch - BTN_H));
    int nx = static_cast<int>(clampf(s->x, 0.0f, maxX));
    int ny = static_cast<int>(clampf(s->y, 0.0f, maxY));
    if (s->hButton) {
        // Move without copying old bits to avoid trails
        SetWindowPos(s->hButton, nullptr, nx, ny, BTN_W, BTN_H,
                     SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS |
                     (repaint ? 0 : SWP_NOREDRAW));
        if (repaint) {
            // Repaint parent & children cleanly (prevents “duplicate” ghosts)
            RedrawWindow(hwndParent, nullptr, nullptr,
                         RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
        }
    }
}

static void jitter_step(AppState* s) {
    std::uniform_real_distribution<float> d(-JITTER_RANGE, JITTER_RANGE);
    float maxX = static_cast<float>(std::max(0, s->cw - BTN_W));
    float maxY = static_cast<float>(std::max(0, s->ch - BTN_H));
    s->x = clampf(s->x + d(s->rng), 0.0f, maxX);
    s->y = clampf(s->y + d(s->rng), 0.0f, maxY);
}

static void evade_or_nudge(AppState* s, float mx, float my) {
    float dx = mx - s->x;
    float dy = my - s->y;
    float dist = std::sqrt(dx * dx + dy * dy);
    float maxX = static_cast<float>(std::max(0, s->cw - BTN_W));
    float maxY = static_cast<float>(std::max(0, s->ch - BTN_H));

    if (dist < NEAR_DIST) {
        std::uniform_real_distribution<float> ang(0.0f, 6.28318530718f); // 2π
        float a = ang(s->rng);
        s->x = clampf(s->x + std::cos(a) * NUDGE_NEAR, 0.0f, maxX);
        s->y = clampf(s->y + std::sin(a) * NUDGE_NEAR, 0.0f, maxY);
    } else {
        auto sgn = [](float v) { return (v > 0) - (v < 0); };
        s->x = clampf(s->x + static_cast<float>(sgn(dx)) * NUDGE_FAR, 0.0f, maxX);
        s->y = clampf(s->y + static_cast<float>(sgn(dy)) * NUDGE_FAR, 0.0f, maxY);
    }
}

// Make button un-pressable
static WNDPROC g_oldBtnProc = nullptr;
static LRESULT CALLBACK ButtonProc(HWND hBtn, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_SPACE || wParam == VK_RETURN) return 0;
            break;
        case BM_CLICK:
            return 0;
    }
    return CallWindowProc(g_oldBtnProc, hBtn, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    AppState* s = reinterpret_cast<AppState*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg) {
    case WM_CREATE: {
        auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        s = new AppState();
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(s));

        RECT rc; GetClientRect(hwnd, &rc);
        s->cw = rc.right - rc.left;
        s->ch = rc.bottom - rc.top;

        s->x = static_cast<float>(std::max(0, (s->cw - BTN_W) / 2));
        s->y = static_cast<float>(std::max(0, (s->ch - BTN_H) / 2));

        if (!s->hButton) { // **guard: create exactly one**
            s->hButton = CreateWindowExA(
                0, "BUTTON",
                "Click here to keep\r\nan idiot busy for hours!",
                WS_CHILD | WS_VISIBLE | BS_MULTILINE | BS_CENTER | BS_VCENTER,
                static_cast<int>(s->x), static_cast<int>(s->y), BTN_W, BTN_H,
                hwnd, reinterpret_cast<HMENU>(1), cs->hInstance, nullptr);

            g_oldBtnProc = reinterpret_cast<WNDPROC>(
                SetWindowLongPtr(s->hButton, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ButtonProc))
            );
        }

        SetTimer(hwnd, 1, TICK_MS, nullptr);
        return 0;
    }
    case WM_TIMER:
        if (wParam == 1 && s) {
            jitter_step(s);
            place_button(hwnd, s);
        }
        return 0;

    case WM_MOUSEMOVE:
        if (s) {
            float mx = static_cast<float>(GET_X_LPARAM(lParam));
            float my = static_cast<float>(GET_Y_LPARAM(lParam));
            evade_or_nudge(s, mx, my);
            place_button(hwnd, s);
        }
        return 0;

    case WM_SIZE:
        if (s) {
            s->cw = LOWORD(lParam);
            s->ch = HIWORD(lParam);
            float maxX = static_cast<float>(std::max(0, s->cw - BTN_W));
            float maxY = static_cast<float>(std::max(0, s->ch - BTN_H));
            s->x = clampf(s->x, 0.0f, maxX);
            s->y = clampf(s->y, 0.0f, maxY);
            place_button(hwnd, s);
        }
        return 0;

    // Ignore clicks entirely
    case WM_COMMAND:
        return 0;

    case WM_DESTROY:
        if (s) {
            KillTimer(hwnd, 1);
            if (s->hButton && g_oldBtnProc) {
                SetWindowLongPtr(s->hButton, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_oldBtnProc));
                g_oldBtnProc = nullptr;
            }
            delete s;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    const char* CLASS_NAME = "MovingButtonWin32Class";

    WNDCLASSA wc{};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassA(&wc);

    // Use WS_EX_COMPOSITED for smoother child-window moves (prevents “duplicates”)
    HWND hwnd = CreateWindowExA(
        WS_EX_COMPOSITED,                // <= double-buffer the parent
        CLASS_NAME, "Moving Button",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 700,
        nullptr, nullptr, hInst, nullptr);

    if (!hwnd) return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}
