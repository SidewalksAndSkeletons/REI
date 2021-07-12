#pragma once

// *** Константы, связанные с окном
namespace WINDOW_DETAILS
{
    // Заголовок окна движка
    constexpr const char* ENGINE_TITLE = "REI";
}

// *** Константы, связанные с отрисовкой графики
namespace GL_DETAILS
{
    // Максимальное количество объектов, которые могут находиться в VBO
    constexpr GLuint VBO_OBJECTS_CAPACITY = 2500;
}

// *** Константы, связанные с работой со звуком
namespace SOUND_DETAILS
{
    // Идентификатор текущего аудио-канала
    constexpr int AUDIO_CHANNEL = -1;

    // Количество аудио-каналов
    constexpr int AUDIO_CHANNELS_COUNT = 2;

    // Поддерживаемая частота
    constexpr int AUDIO_FREQUENCY = 44100;

    // Размер одного чанка
    constexpr int AUDIO_CHUNKSIZE = 4096;

    // Поддерживаемый тип аудиофайлов
    constexpr const char* AUDIO_FILE_TYPE = ".ogg";
}
