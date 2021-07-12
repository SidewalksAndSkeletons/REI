#include "StdAfx.h"

CWindow::CWindow() : SourceWindow(nullptr), DisplayMode {}
{
    _w = 0;
    _h = 0;

    DisplayIndex = 0;
}

CWindow::~CWindow()
{
    // Корректно освобождаем память, занятую окном
    SDL_DestroyWindow(SourceWindow);
}

bool CWindow::Init()
{
    // Устанавливаем размер экрана, равный текущему разрешению
    SDL_DisplayMode* Mode = GetDisplayMode();
    SDL_GetCurrentDisplayMode(GetDisplayIndex(), Mode);

    // ...и обновляем данные
    _w = Mode->w;
    _h = Mode->h;
    ViewPoint.Set(_w, _h);

    Uint32 Flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

    // Под отладчиком полный экран нам не нужен, т.к. это может вызвать проблему,
    // в результате которой вмешательство в исходный код будет заблокировано потоком движка.
#ifndef DEBUG
    Flags |= SDL_WINDOW_FULLSCREEN;
#endif

    // Пробуем проинициализировать окно
    SourceWindow = SDL_CreateWindow(WINDOW_DETAILS::ENGINE_TITLE, 0, 0, _w, _h, Flags);

    if (!SourceWindow)
    {
        DEBUG_ERROR("SDL error: ", SDL_GetError());
        return false;
    }

    return true;
}

SDL_Window* CWindow::Get()
{
    return SourceWindow;
}

SDL_DisplayMode* CWindow::GetDisplayMode()
{
    return &DisplayMode;
}

int CWindow::GetDisplayIndex() const
{
    return DisplayIndex;
}

CViewPoint& CWindow::GetViewPoint()
{
    return ViewPoint;
}

const int& CWindow::w()
{
    return _w;
}

const int& CWindow::h()
{
    return _h;
}
