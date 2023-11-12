// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;


#include "App.xaml.h"//To access current window
#include <microsoft.ui.xaml.window.h>//For IWindowNative
#include "SystemBackdropHelper.hpp"

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WinUI3_TransparentBackground_CppWinRT::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        HWND hWnd;
        this->try_as<IWindowNative>()->get_WindowHandle(&hWnd);

        SystemBackdropHelper::SystemBackdropHelper::RemoveWin32Backdrop(hWnd);
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

}


void winrt::WinUI3_TransparentBackground_CppWinRT::implementation::MainWindow::SystemBackdropColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
{
    SystemBackdropHelper::SystemBackdropHelper::SetSystemBackdrop(
        App::Window(), true, SystemBackdropColor().Color()
    );
}
