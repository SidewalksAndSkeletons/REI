#pragma once

// *** Класс-обёртка для SDL_Window*
class CWindow final : public ISingleton<CWindow>
{
private:
    // Размеры обозреваемой области
    SRect<int> ViewPoint;

    // Размеры окна
    SRect<int> Size;

private:
    // Индекс текущего дисплея
    int DisplayIndex;

private:
    // Указатель на сырое окно
    SDL_Window* Source;

    // Структура текущего дисплея
    SDL_DisplayMode DisplayMode;

public:
    CWindow();
    CWindow(const CWindow&) = delete;
    CWindow& operator=(const CWindow&) = delete;
    ~CWindow();

    [[nodiscard]] bool Init();

public:
    // *** Получить данные о текущем дисплее
    SDL_DisplayMode* GetDisplayMode();

public:
    // *** Получить индекс дисплея
    int GetDisplayIndex() const;

public:
    // *** Получить размеры области отрисовки
    void GetViewPoint(int& x, int& y, int& w, int& h);

    // *** Получить данные для проекционной матрицы
    void GetProjection(int& w, int& h);

public:
    // *** Обновить картинку на экране
    void Swap();

    // *** Создать OpenGL-контекст в текущем окне
    void CreateContext(SDL_GLContext& Context);
};
