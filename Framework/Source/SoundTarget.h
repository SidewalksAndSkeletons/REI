#pragma once

// *** Класс, обеспечивающий работу со аудио-картой
class CSoundTarget final : public ISingleton<CSoundTarget>
{
private:
    // Контейнер со всеми загруженными аудио-треками
    std::unordered_map<std::string, Mix_Music*> Musics;

    // Контейнер со всеми загруженными звуками
    std::unordered_map<std::string, Mix_Chunk*> Sounds;

private:
    // Название последней игравшей музыки
    std::string LastMusic;

    // Название последнего игравшего звука
    std::string LastSound;

public:
    CSoundTarget() = default;
    CSoundTarget(const CSoundTarget&) = delete;
    CSoundTarget& operator=(const CSoundTarget&) = delete;
    ~CSoundTarget();

    [[nodiscard]] bool Init();

private:
    // *** Освободить память, занятую музыкой
    void FreeMusic(Mix_Music* Music);

    // *** Освободить память, занятую музыкой (через его название)
    void FreeMusic(const std::string& Path);

    // *** Освободить память, занятую звуком
    void FreeSound(Mix_Chunk* Sound);

    // *** Освободить память, занятую звуком (через его название)
    void FreeSound(const std::string& Path);

public:
    // *** Загрузить музыку по указанному пути
    bool LoadMusic(const std::string& Path);

    // *** Загрузить звук по указанному пути
    bool LoadSound(const std::string& Path);

public:
    // *** Продолжить играть звук (либо же начать играть новый)
    // [Путь до звука; число повторов]
    void PlaySound(const std::string& Path = "", int Loops = 0);

    // *** Поставить звуки на паузу
    void PauseSounds();

    // *** Прекратить проигрывание звуков
    void StopSounds();

public:
    // *** Продолжить играть музыку (либо начать новый)
    // [Путь до звука; количество циклов]
    void PlayMusic(const std::string& Path = "", int Loops = -1);

    // *** Поставить музыку на паузу
    void PauseMusic();

    // *** Прервать проигрывание музыки
    void StopMusic();
};

inline std::shared_ptr<CSoundTarget> SoundTarget = nullptr;
