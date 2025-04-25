#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <d3d11.h>
#include <dwmapi.h>

#include "../src/mem/mem.h"
#include "../src/pascuware/pascuware.hpp"
#include "../src/game/game.hpp"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>



void UpdateWindowInteractivity(HWND hwnd, bool allowInteraction) {
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

    if (allowInteraction) {
        exStyle &= ~WS_EX_TRANSPARENT;
    }
    else {
        exStyle |= WS_EX_TRANSPARENT;
    }

    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
}

void SyncWindowInteractivity(HWND hwnd) {
    static bool last = config::activeMenu;
    if (last != config::activeMenu) {
        UpdateWindowInteractivity(hwnd, config::activeMenu);
        last = config::activeMenu;
    }
}




extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0L;
    }
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return 0L;


    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Window Class", NULL };
    RegisterClassExW(&wc);


    int screenWidth = config::screenSize.GetWidth();
    int screenHeight = config::screenSize.GetHeight();

    const HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        L"Window Class",
        L"Pascuware",
        WS_POPUP,
        0, 0, screenWidth, screenHeight,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );


    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        RECT windowRect;
        GetWindowRect(hwnd, &windowRect);

        POINT diff;
        ClientToScreen(hwnd, &diff);

        const MARGINS margins = { diff.x, diff.y, screenWidth - (windowRect.right - windowRect.left), screenHeight - (windowRect.bottom - windowRect.top) };


        DwmExtendFrameIntoClientArea(hwnd, &margins);
    }


    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;



    constexpr D3D_FEATURE_LEVEL featureLevels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;
    D3D_FEATURE_LEVEL featureLevel;


    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels,
        2,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        &featureLevel,
        &context
    );

    ID3D11Texture2D* backBuffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (!backBuffer)
        return 1;

    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    ImGui::CreateContext();
    ImGui::StyleColorsClassic();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);




    const DWORD pid = memory::GetProcessId(L"cs2.exe");
    if (!pid)
    {
        MessageBoxA(0, "Process not found", "Error", MB_OK);
        return 1;
    }

    game::clientBase = memory::GetModuleBaseAddress(pid, L"client.dll");
    if (!game::clientBase)
    {
        MessageBoxA(0, "Module not found", "Error", MB_OK);
        return 1;
    }




    while (config::running)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                config::running = false;
                break;
            }
        }


		SyncWindowInteractivity(hwnd);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


    

        pascuware::Run();


        ImGui::Render();
        context->OMSetRenderTargets(1, &renderTargetView, nullptr);
        context->ClearRenderTargetView(renderTargetView, game::clearColor);


        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        swapChain->Present(1, 0);




    }



    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (swapChain) swapChain->Release();
    if (renderTargetView) renderTargetView->Release();
    if (context) context->Release();
    if (device) device->Release();
    if (hwnd) DestroyWindow(hwnd);
    if (wc.hInstance) UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}