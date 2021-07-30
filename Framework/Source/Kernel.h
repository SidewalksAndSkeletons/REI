#pragma once

// *** Класс, представляющий собой ядро движка
class CKernel final : public ISingleton<CKernel>
{
private:
    // Ключи, указанные при запуске
    std::vector<std::string> Keys;

private:
    // Структура текущего SDL-события
    SDL_Event CurrentEvent;

private:
    // Статус работы приложения
    bool RunningStatus;

public:
    CKernel();
    CKernel(const CKernel&) = delete;
    CKernel& operator=(const CKernel&) = delete;
    ~CKernel();

    [[nodiscard]] bool Init();

public:
    // *** Обработка SDL-событий
    void HandleEvents();

    // *** Обновление логики
    void Update();

    // *** Отрисовка
    void Render();

public:
    // *** Корректный выход из приложения
    void Exit();

public:
    // *** Получить статус работы приложения
    bool GetRunningStatus() const;

public:
    // *** Перебрать ключи, указанные при запуске программы
    // [Количество ключей; массив строк]
    void ParseKeys(int KeysCount, char** SourceKeys);

    // *** Был ли указан ключ при запуске?
    bool FindKey(std::string_view Key);
};
