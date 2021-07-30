#include "StdAfx.h"

CWindow::CWindow() : ViewPoint{}, Size{}, Source{}, DisplayMode{}
{
    DisplayIndex = 0;
}

CWindow::~CWindow()
{
    SDL_DestroyWindow(Source);
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
    Size.set(Mode->w, Mode->h);
    ViewPoint.set(Mode->w, Mode->h);

    Uint32 Flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

    // Под отладчиком полный экран нам не нужен, т.к. это может вызвать проблему,
    // в результате которой вмешательство в исходный код будет заблокировано потоком движка.
#ifndef DEBUG
    Flags |= SDL_WINDOW_FULLSCREEN;
#endif

    // Пробуем проинициализировать окно
    Source = SDL_CreateWindow(WINDOW_DETAILS::ENGINE_TITLE, 0, 0, Size.w, Size.h, Flags);

    if (!Source)
    {
        DEBUG_ERROR("SDL error: ", SDL_GetError());
        return false;
    }

    return true;
}

SDL_DisplayMode* CWindow::GetDisplayMode()
{
    return &DisplayMode;
}

int CWindow::GetDisplayIndex() const
{
    return DisplayIndex;
}

void CWindow::GetViewPoint(int& x, int& y, int& w, int& h)
{
    // Коэффициент масштабирования
    double s = fmin
    (
        Size.w / static_cast<double>(ViewPoint.w),
        Size.h / static_cast<double>(ViewPoint.h)
    );

    // Применяем коэффициент
    int sw = static_cast<int>(ViewPoint.w * s);
    int sh = static_cast<int>(ViewPoint.h * s);

    // Высчитываем координаты точки обзора
    x = Size.w / 2 - sw / 2;
    y = Size.h / 2 - sh / 2;
    w = Size.w;
    h = Size.h;
}

void CWindow::GetProjection(int& w, int& h)
{
    w = Size.w;
    h = Size.h;
}

void CWindow::Swap()
{
    SDL_GL_SwapWindow(Source);
}

void CWindow::CreateContext(SDL_GLContext& Context)
{
    Context = SDL_GL_CreateContext(Source);
}
