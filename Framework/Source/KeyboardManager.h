#pragma once

// *** Менеджер, обеспечивающий работу с клавиатурой
class CKeyboardManager final : public ISingleton<CKeyboardManager>
{
private:
    // Контейнер, содержащий состояния нажатых клавиш
    std::unordered_map<SDL_Keycode, Uint8> Keys;

public:
    CKeyboardManager() = default;
    CKeyboardManager(const CKeyboardManager&) = delete;
    CKeyboardManager& operator=(const CKeyboardManager&) = delete;
    ~CKeyboardManager() = default;

public:
    // *** Установить статус активации клавиши
    void Set(SDL_Keycode Key, Uint8 State);

    // *** Получить статус активации клавиши
    bool Get(SDL_Keycode Key);
};

inline std::shared_ptr<CKeyboardManager> KeyboardManager = nullptr;
