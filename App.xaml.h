// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"

namespace winrt::WinUI3_TransparentBackground_CppWinRT::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        static winrt::Microsoft::UI::Xaml::Window Window() { return window; };
        //To access current window because Window::Current() is always null in destop application
    private:
        static winrt::Microsoft::UI::Xaml::Window window;
    };
}
