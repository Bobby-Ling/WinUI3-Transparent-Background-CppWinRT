#pragma once
#include "pch.h"


#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Gdi32.Lib")//For CreateRectRgn

#include <dwmapi.h>//For DwmEnableBlurBehindWindow
#include <dispatcherqueue.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Composition.h>
namespace WindowsCompositionHelperNS {
    namespace WS = winrt::Windows::System;
    namespace WUC = winrt::Windows::UI::Composition;
    class WindowsCompositionHelper
    {
    private:
        static WS::DispatcherQueueController DispatcherQueueController_;
        static WS::DispatcherQueue DispatcherQueue_;
        static WUC::Compositor Compositor_;
        static WUC::Compositor InitCompositor();
        static WS::DispatcherQueue InitDispatcherQueue();
    public:
        static WS::DispatcherQueue DispatcherQueue();
        static WUC::Compositor Compositor();
    };
    WS::DispatcherQueue WindowsCompositionHelper::DispatcherQueue()
    {
        return InitDispatcherQueue();
    }
    WUC::Compositor WindowsCompositionHelper::Compositor()
    {
        return InitCompositor();
    }
    WS::DispatcherQueue WindowsCompositionHelper::InitDispatcherQueue()
    {
        if (nullptr == WS::DispatcherQueue::GetForCurrentThread() &&
            nullptr == DispatcherQueueController_) {
            DispatcherQueueOptions DispatcherQueueOptions_;
            DispatcherQueueOptions_.dwSize = sizeof(DispatcherQueueOptions);
            DispatcherQueueOptions_.threadType = DQTYPE_THREAD_CURRENT;
            DispatcherQueueOptions_.apartmentType = DQTAT_COM_STA;

            ::ABI::Windows::System::IDispatcherQueueController* ptr{ nullptr };
            CreateDispatcherQueueController(DispatcherQueueOptions_, &ptr);
            DispatcherQueueController_=WS::DispatcherQueueController{ ptr,winrt::take_ownership_from_abi };
            DispatcherQueue_ = DispatcherQueueController_.DispatcherQueue();
        }
        return DispatcherQueue_;
    }
    WUC::Compositor WindowsCompositionHelper::InitCompositor()
    {
        if (nullptr == Compositor_) {
            InitDispatcherQueue();
            Compositor_ = WUC::Compositor();
        }
        return Compositor_;
    }
    
    WS::DispatcherQueueController WindowsCompositionHelper::DispatcherQueueController_{ nullptr };
    WS::DispatcherQueue WindowsCompositionHelper::DispatcherQueue_{ nullptr };
    WUC::Compositor WindowsCompositionHelper::Compositor_{ nullptr };
}


namespace SystemBackdropHelper {
    namespace WU = winrt::Windows::UI;
    namespace WUX = winrt::Windows::UI::Xaml;
    namespace MUC = winrt::Microsoft::UI::Composition;
    namespace MUX = winrt::Microsoft::UI::Xaml;
    using WindowsCompositionHelperNS::WindowsCompositionHelper;

    class SystemBackdropHelper
    {
    public:
        static void SetSystemBackdrop(MUX::Window, bool isEnabled, WU::Color BackdropColor = WU::Colors::Transparent());
        static void RemoveWin32Backdrop(HWND hWnd);
    };
    void SystemBackdropHelper::SetSystemBackdrop(MUX::Window window, bool isEnabled, WU::Color BackdropColor)
    {
        auto brushHolder = window.as<MUC::ICompositionSupportsSystemBackdrop>();
        if (isEnabled) {
            auto colorBrush = WindowsCompositionHelper::Compositor().CreateColorBrush(BackdropColor);
            brushHolder.SystemBackdrop(colorBrush);
        }
        else {
            brushHolder.SystemBackdrop(nullptr);
        }
    }
    void SystemBackdropHelper::RemoveWin32Backdrop(HWND hWnd)
    {
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
        auto rgn = CreateRectRgn(-2, -2, -1, -1);
        DWM_BLURBEHIND blurBehind = {
            DWM_BB_ENABLE | DWM_BB_BLURREGION,
            TRUE,
            rgn
        };
        DwmEnableBlurBehindWindow(hWnd, &blurBehind);
    }
}
