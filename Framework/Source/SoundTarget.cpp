#include "StdAfx.h"

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

    // Освобождаем память, занятую библиотекой
    Mix_Quit();
}

bool CSoundTarget::Init()
{
    using namespace SOUND_DETAILS;

    // Будем поддерживать формат .ogg
    if (!Mix_Init(MIX_INIT_OGG))
    {
        DEBUG_ERROR("SDL error: ", Mix_GetError());
        return false;
    }

    // Инициализируем библиотеку для работы с аудио
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

        LastMusic.clear();
    }

    Mix_FreeMusic(Music);
    Music = nullptr;
}

void CSoundTarget::FreeMusic(const std::string& Path)
{
    if (Path.empty())
    {
        DEBUG_WARNING("Empty path!");
        return;
    }

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

        LastSound.clear();
    }

    Mix_FreeChunk(Sound);
    Sound = nullptr;
}

void CSoundTarget::FreeSound(const std::string& Path)
{
    if (Path.empty())
    {
        DEBUG_WARNING("Empty path!");
        return;
    }

    FreeSound(Sounds[Path]);
    Sounds[Path] = nullptr;
}

bool CSoundTarget::LoadMusic(const std::string& Path)
{
    if (Path.empty())
    {
        DEBUG_WARNING("Empty path!");
        return false;
    }

    // Указываем корректный путь, согласно иерархии приложения
    std::string NewPath = "Sounds/Music/";
    String::Unite(NewPath, Path, SOUND_DETAILS::AUDIO_FILE_TYPE);

    // Пробуем загрузить аудио-файл
    Mix_Music* Music = Mix_LoadMUS(String::GetPath(NewPath).c_str());

    if (!Music)
    {
        DEBUG_WARNING("Can't load this music track: ", NewPath, Mix_GetError());
        return false;
    }

    Musics[Path] = Music;
    return true;
}

bool CSoundTarget::LoadSound(const std::string& Path)
{
    if (Path.empty())
    {
        DEBUG_WARNING("Empty path!");
        return false;
    }

    // Указываем корректный путь, согласно иерархии приложения
    std::string NewPath = "Sounds/";
    String::Unite(NewPath, Path, SOUND_DETAILS::AUDIO_FILE_TYPE);

    // Пробуем загрузить звук
    Mix_Chunk* Sound = Mix_LoadWAV(String::GetPath(NewPath).c_str());

    if (!Sound)
    {
        DEBUG_WARNING("Can't load this sound: ", NewPath, Mix_GetError());
        return false;
    }

    Sounds[Path] = Sound;
    return true;
}

void CSoundTarget::PlaySound(const std::string& Path, int Loops)
{
    if (Path.empty())
    {
        DEBUG_WARNING("Empty path!");
        return;
    }

    Mix_Chunk* Sound = Sounds[Path];

    // Если звук не найден - пытаемся его загрузить
    if (!Sound)
    {
        if (!LoadSound(Path))
            return;

        Sound = Sounds[Path];
    }

    // Проигрываем звук в указанном канале N-ное количество раз
    Mix_PlayChannel(SOUND_DETAILS::AUDIO_CHANNEL, Sound, Loops);

    // Обновляем имя последнего проигранного звука
    LastSound = Path;
}

void CSoundTarget::PauseSounds()
{
    Mix_Pause(SOUND_DETAILS::AUDIO_CHANNEL);
}

void CSoundTarget::StopSounds()
{
    Mix_HaltChannel(SOUND_DETAILS::AUDIO_CHANNEL);
}

void CSoundTarget::PlayMusic(const std::string& Path, int Loops)
{
    Mix_Music* Music = nullptr;

    if (Path.empty())
    {
        if (LastMusic.empty())
        {
            DEBUG_WARNING("Last music track is nullptr!");
            return;
        }

        // Если путь не указан, но есть старый трек - играем его
        Music = Musics[LastMusic];
    }
    else
    {
        Music = Musics[Path];
    }

    // Если музыка не найдена - пробуем её загрузить
    if (!Music)
    {
        if (!LoadMusic(Path))
            return;

        Music = Musics[Path];
    }

    // Проигрываем музыку n-ное количество раз
    Mix_PlayMusic(Music, Loops);

    // Обновляем имя последнего проигранного трека
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
