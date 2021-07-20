#pragma once

// *** Менеджер, обеспечивающий работу с клавиатурой
class CKeyboardManager final
{
private:
    // Контейнер, содержащий состояния нажатых клавиш
    std::unordered_map<SDL_Keycode, Uint8> Keys;

public:
    CKeyboardManager() = default;
    ~CKeyboardManager() = default;

public:
    // *** Установить статус активации клавиши
    void Set(const SDL_Keycode& Key, const Uint8& State);

    // *** Получить статус активации клавиши
    bool Get(const SDL_Keycode& Key);
};

inline CKeyboardManager KeyboardManager;
