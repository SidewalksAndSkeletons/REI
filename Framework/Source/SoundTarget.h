#pragma once

// *** Класс, обеспечивающий работу со аудио-картой
class CSoundTarget final
{
private:
    // Контейнер со всеми загруженными аудио-треками
    std::unordered_map<const char*, Mix_Music*> Musics;

    // Контейнер со всеми загруженными звуками
    std::unordered_map<const char*, Mix_Chunk*> Sounds;

private:
    // Название последней игравшей музыки
    const char* LastMusic;

    // Название последнего игравшего звука
    const char* LastSound;

public:
    CSoundTarget();
    ~CSoundTarget();

    [[nodiscard]] bool Init();

private:
    // *** Освободить память, занятую музыкой
    void FreeMusic(Mix_Music* Music);

    // *** Освободить память, занятую музыкой (через его название)
    void FreeMusic(const char* Path);

    // *** Освободить память, занятую звуком
    void FreeSound(Mix_Chunk* Sound);

    // *** Освободить память, занятую звуком (через его название)
    void FreeSound(const char* Path);

public:
    // *** Загрузить музыку по указанному пути
    Mix_Music* LoadMusic(const char* Path);

    // *** Загрузить звук по указанному пути
    Mix_Chunk* LoadSound(const char* Path);

public:
    // *** Продолжить играть звук (либо же начать играть новый)
    // [Путь до звука; число повторов]
    void PlaySound(const char* Path = nullptr, int Loops = 0);

    // *** Поставить звуки на паузу
    void PauseSounds();

    // *** Прекратить проигрывание звуков
    void StopSounds();

public:
    // *** Продолжить играть музыку (либо начать новый)
    // [Путь до звука; количество циклов]
    void PlayMusic(const char* Path = nullptr, int Loops = -1);

    // *** Поставить музыку на паузу
    void PauseMusic();

    // *** Прервать проигрывание музыки
    void StopMusic();
};

inline CSoundTarget SoundTarget;
