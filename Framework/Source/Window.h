#pragma once

// *** Класс-обёртка для SDL_Window*
class CWindow final
{
private:
    // Поле зрения камеры
    CViewPoint ViewPoint;

private:
    // Длина окна
    int _w;

    // Высота окна
    int _h;

private:
    // Индекс текущего дисплея
    int DisplayIndex;

private:
    // Указатель на сырое окно
    SDL_Window* SourceWindow;

    // Структура текущего дисплея
    SDL_DisplayMode DisplayMode;

public:
    CWindow();
    ~CWindow();

    [[nodiscard]] bool Init();

public:
    // *** Получить указатель на сырое окно
    SDL_Window* Get();

public:
    // *** Получить данные о текущем дисплее
    SDL_DisplayMode* GetDisplayMode();

public:
    // *** Получить индекс дисплея
    int GetDisplayIndex() const;

public:
    // *** Получить поле зрения
    CViewPoint& GetViewPoint();

public:
    // *** Получить длину окна
    const int& w();

    // *** Получить высоту окна
    const int& h();
};
