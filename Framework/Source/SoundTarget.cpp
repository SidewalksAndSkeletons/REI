#include "StdAfx.h"

CSoundTarget::CSoundTarget() : LastMusic(nullptr), LastSound(nullptr)
{
}

CSoundTarget::~CSoundTarget()
{
    for (auto& It : Musics)
    {
        FreeMusic(It.second);
    }

    Musics.clear();

    for (auto& It : Sounds)
    {
        FreeSound(It.second);
    }

    Sounds.clear();

    // Выход из библиотеки
    Mix_Quit();
}

bool CSoundTarget::Init()
{
    // Используем именное пространство, содержащее константы для корректного 
    // взаимодействия с аудио-картой устройства
    using namespace SOUND_DETAILS;

    // Инициализируем библиотеку для работы с аудио
    if (!Mix_Init(MIX_INIT_OGG))
    {
        DEBUG_ERROR("SDL error: ", Mix_GetError());
        return false;
    }

    if (Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS_COUNT, AUDIO_CHUNKSIZE))
    {
        DEBUG_ERROR("SDL error: ", Mix_GetError());
        return false;
    }

    return true;
}

void CSoundTarget::FreeMusic(Mix_Music* Music)
{
    if (!Music)
    {
        DEBUG_WARNING("Music already destroyed!");
        return;
    }

    // Если мы собираемся удалить музыку, которая играет последней
    // или играет сейчас
    if (Musics[LastMusic] == Music)
    {
        // Прекращаем проигрывать музыку
        StopMusic();

        // Зачищаем название
        LastMusic = nullptr;
    }

    // Освобождаем память, занятую музыкой
    Mix_FreeMusic(Music);
    Music = nullptr;
}

void CSoundTarget::FreeMusic(const char* Path)
{
    if (!Path)
    {
        DEBUG_WARNING("Empty path!");
        return;
    }

    // Освобождаем память из-под трека
    FreeMusic(Musics[Path]);
    Musics[Path] = nullptr;
}

void CSoundTarget::FreeSound(Mix_Chunk* Sound)
{
    if (!Sound)
    {
        DEBUG_WARNING("Sound already destroyed!");
        return;
    }

    // Если мы собираемся удалить звук, который играл последним
    // или играет сейчас
    if (Sounds[LastSound] == Sound)
    {
        // Прекращаем проигрывать звук в канале
        StopSounds();

        // Зачищаем название
        LastSound = nullptr;
    }

    // Корректно освобождаем память, занятую звуком
    Mix_FreeChunk(Sound);
    Sound = nullptr;
}

void CSoundTarget::FreeSound(const char* Path)
{
    if (!Path)
    {
        DEBUG_WARNING("Empty path!");
        return;
    }

    // Освобождаем память из-под звука
    FreeSound(Sounds[Path]);
    Sounds[Path] = nullptr;
}

Mix_Music* CSoundTarget::LoadMusic(const char* Path)
{
    if (!Path)
    {
        DEBUG_WARNING("Empty path!");
        return nullptr;
    }

    // Указываем новый путь
    std::string NewPath = "../Sounds/Music/";
    String::Unite(NewPath, Path, SOUND_DETAILS::AUDIO_FILE_TYPE);

    // Пробуем проинициализировать музыку
    Mix_Music* Music = Mix_LoadMUS(NewPath.c_str());

    // Не удалось проинициализировать звук
    if (!Music)
    {
        DEBUG_WARNING("Can't load this music track: ", NewPath, Mix_GetError());
        return nullptr;
    }

    // Перемещаем элемент в контейнер
    Musics[Path] = Music;

    // Возвращаем элемент в качестве указателя
    return Music;
}

Mix_Chunk* CSoundTarget::LoadSound(const char* Path)
{
    if (!Path)
    {
        DEBUG_WARNING("Empty path!");
        return nullptr;
    }

    // Указываем новый путь
    std::string NewPath = "../Sounds/";
    String::Unite(NewPath, Path, SOUND_DETAILS::AUDIO_FILE_TYPE);

    // Пробуем проинициализировать звук
    Mix_Chunk* Sound = Mix_LoadWAV(NewPath.c_str());

    // Не удалось проинициализировать звук
    if (!Sound)
    {
        DEBUG_WARNING("Can't load this sound: ", NewPath, Mix_GetError());
        return nullptr;
    }

    // Перемещаем элемент в контейнер
    Sounds[Path] = Sound;

    // Возвращаем элемент в качестве указателя
    return Sound;
}

void CSoundTarget::PlaySound(const char* Path, int Loops)
{
    if (!Path)
    {
        DEBUG_WARNING("Empty path!");
        return;
    }

    // Пробуем получить звук из хранилища
    Mix_Chunk* Sound = Sounds[Path];

    // Если звук не найден -
    if (!Sound)
    {
        // Пробуем его подгрузить
        Sound = LoadSound(Path);

        if (!Sound)
        {
            DEBUG_WARNING("Sound is nullptr!");
            return;
        }
    }

    // Проигрываем звук в указанном канале N-ное количество раз
    Mix_PlayChannel(SOUND_DETAILS::AUDIO_CHANNEL, Sound, Loops);

    // Перезаписываем название текущего звука
    LastSound = Path;
}

void CSoundTarget::PauseSounds()
{
    // Приостанавливаем работу аудио-канала
    Mix_Pause(SOUND_DETAILS::AUDIO_CHANNEL);
}

void CSoundTarget::StopSounds()
{
    // Прерываем работу аудио-канала
    Mix_HaltChannel(SOUND_DETAILS::AUDIO_CHANNEL);
}

void CSoundTarget::PlayMusic(const char* Path, int Loops)
{
    // Если путь не указан
    if (!Path)
    {
        if (!LastMusic)
        {
            DEBUG_WARNING("Last music track is nullptr!");
            return;
        }

        // Но есть старый трек - играем его
        Path = LastMusic;
    }

    // Получаем указатель на трек в контейнере
    Mix_Music* Music = Musics[Path];

    // Если музыка не получена, значит
    if (!Music)
    {
        // Пробуем её загрузить
        Music = LoadMusic(Path);

        if (!Music)
        {
            DEBUG_WARNING("Music is nullptr!");
            return;
        }
    }

    // Проигрываем музыку
    Mix_PlayMusic(Music, Loops);

    // Перезаписываем текущую музыку
    LastMusic = Path;
}

void CSoundTarget::PauseMusic()
{
    Mix_PauseMusic();
}

void CSoundTarget::StopMusic()
{
    Mix_HaltMusic();
}
