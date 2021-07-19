#include "StdAfx.h"

CWindow::CWindow() : _w(0), _h(0), DisplayIndex(0), SourceWindow(nullptr), DisplayMode {}
{}

CWindow::~CWindow()
{
    // Корректно освобождаем память, занятую окном
    if (SourceWindow)
    {
        SDL_DestroyWindow(SourceWindow);
    }
}

bool CWindow::Init()
{
    SDL_DisplayMode* Mode = GetDisplayMode();

    if (!Mode)
    {
        DEBUG_ERROR("Window's display mode is nullptr: ", SDL_GetError());
        return false;
    }

    // Устанавливаем размер экрана, равный текущему разрешению
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
