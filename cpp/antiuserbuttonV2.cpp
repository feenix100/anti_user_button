#include <windows.h>
#include <windowsx.h>
#include <random>
#include <cmath>

static constexpr int   BTN_W        = 200;
static constexpr int   BTN_H        = 50;
static constexpr float NEAR_DIST    = 100.0f;  // repel radius
static constexpr float REPEL_STEP   = 120.0f;  // how far to push away when close
static constexpr UINT  TICK_MS      = 33;      // ~30fps jitter
static constexpr float JITTER_RANGE = 10.0f;   // idle wiggle

struct AppState {
    HWND  hButton = nullptr;   // exactly one
    float x = 0.0f;
    float y = 0.0f;
    int   cw = 800;            // client width
    int   ch = 600;            // client height
    std::mt19937 rng{ std::random_device{}() };
};

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void update_client_size(HWND hwnd, AppState* s) {
    RECT rc; GetClientRect(hwnd, &rc);
    s->cw = rc.right - rc.left;
    s->ch = rc.bottom - rc.top;
}

static void keep_in_bounds(AppState* s) {
    float maxX = static_cast<float>(std::max(0, s->cw - BTN_W));
    float maxY = static_cast<float>(std::max(0, s->ch - BTN_H));
    s->x = clampf(s->x, 0.0f, maxX);
    s->y = clampf(s->y, 0.0f, maxY);
}

static void place_button(HWND /*hwndParent*/, AppState* s) {
    if (!s->hButton) return;
    keep_in_bounds(s);
    MoveWindow(s->hButton, static_cast<int>(s->x), static_cast<int>(s->y), BTN_W, BTN_H, TRUE);
}

static void jitter_step(AppState* s) {
    std::uniform_real_distribution<float> d(-JITTER_RANGE, JITTER_RANGE);
    s->x += d(s->rng);
    s->y += d(s->rng);
    keep_in_bounds(s);
}

// REPULSE ONLY WHEN CLOSE
static void repel_if_close(AppState* s, float mx, float my) {
    // Button’s top-left is (x,y). Use the button center for distance checks.
    float bx = s->x + BTN_W * 0.5f;
    float by = s->y + BTN_H * 0.5f;

    float dx = bx - mx;   // vector from cursor -> button center
    float dy = by - my;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 0.0f && dist < NEAR_DIST) {
        // Unit vector away from cursor
        float ux = dx / dist;
        float uy = dy / dist;

        // Push the button away by REPEL_STEP along that vector
        s->x += ux * REPEL_STEP;
        s->y += uy * REPEL_STEP;
        keep_in_bounds(s);
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

        update_client_size(hwnd, s);

        // Start centered
        s->x = static_cast<float>((s->cw - BTN_W) / 2);
        s->y = static_cast<float>((s->ch - BTN_H) / 2);
        keep_in_bounds(s);

        // Create exactly one button
        s->hButton = CreateWindowExA(
            0, "BUTTON",
            "Click here to keep\r\nan idiot busy for hours!",
            WS_CHILD | WS_VISIBLE | BS_MULTILINE | BS_CENTER | BS_VCENTER,
            static_cast<int>(s->x), static_cast<int>(s->y), BTN_W, BTN_H,
            hwnd, reinterpret_cast<HMENU>(1), cs->hInstance, nullptr);

        g_oldBtnProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtr(s->hButton, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ButtonProc))
        );

        SetTimer(hwnd, 1, TICK_MS, nullptr);
        return 0;
    }
    case WM_TIMER:
        if (wParam == 1 && s) {
            jitter_step(s);          // remove this line if you want zero idle motion
            place_button(hwnd, s);
        }
        return 0;

    case WM_MOUSEMOVE:
        if (s) {
            float mx = static_cast<float>(GET_X_LPARAM(lParam));
            float my = static_cast<float>(GET_Y_LPARAM(lParam));
            repel_if_close(s, mx, my);   // repel only if within NEAR_DIST
            place_button(hwnd, s);
        }
        return 0;

    case WM_SIZE:
        if (s) {
            update_client_size(hwnd, s);
            keep_in_bounds(s);
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
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
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
