#include "StdAfx.h"

CKernel::CKernel() : RunningStatus(false), CurrentEvent {}
{}

CKernel::~CKernel()
{
    Keys.clear();
}

bool CKernel::Init()
{
    // Используемые библиотеки
    Uint32 Flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;

    // Инициализация библиотеки SDL2 и её вспомогательных модулей
    if (SDL_Init(Flags) != 0)
    {
        DEBUG_ERROR("SDL initialization failed: ", SDL_GetError());
        return false;
    }

    // Инициализация рендера
    if (!RenderTarget.Init())
    {
        DEBUG_ERROR("RenderTarget initialization failed!");
        return false;
    }

    // Инициализация обработчика звуков
    if (!SoundTarget.Init())
    {
        DEBUG_ERROR("SoundTarget initialization failed!");
        return false;
    }

    return true;
}

void CKernel::Update()
{
    // Выход из программы по нажатию кнопки ESC
    if (KeyboardManager.Get(SDLK_ESCAPE))
    {
        Exit();
    }
}

void CKernel::Render()
{
    RenderTarget.RenderFirstPhase();

    RenderTarget.RenderSecondPhase();
}

void CKernel::Exit()
{
    RunningStatus = false;
}

void CKernel::ParseKeys(int KeysCount, char** SourceKeys)
{
    // Ничего специфического - скипаем
    if (KeysCount < 1)
        return;

    // Перебираем ключи и их значения
    for (size_t CurrentKey = 0; CurrentKey < static_cast<size_t>(KeysCount); ++CurrentKey)
    {
        // Добавляем их в хранилище
        Keys.push_back(SourceKeys[CurrentKey]);
    }
}

bool CKernel::GetRunningStatus() const
{
    return RunningStatus;
}

void CKernel::SetRunningStatus(const bool Value)
{
    RunningStatus = Value;
}

bool CKernel::FindKey(const char* Key)
{
    return std::find(Keys.cbegin(), Keys.cend(), Key) != Keys.end();
}
