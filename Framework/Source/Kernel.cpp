#include "StdAfx.h"

CKernel::CKernel() : RunningStatus(false), CurrentEvent {}
{}

CKernel::~CKernel()
{
    Keys.clear();
}

bool CKernel::Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        DEBUG_ERROR("SDL initialization failed: ", SDL_GetError());
        return false;
    }

    if (!RenderTarget.Init())
    {
        DEBUG_ERROR("RenderTarget initialization failed!");
        return false;
    }

    return true;
}

void CKernel::HandleEvents()
{
    while (SDL_PollEvent(&CurrentEvent))
    {
        switch (CurrentEvent.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            case SDL_QUIT:
            {
                Exit();
            } break;

            case SDL_KEYDOWN:
            {
                switch (CurrentEvent.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        Exit();
                    } break;

                    default:
                    {
                    } break;
                }
            } break;

            default:
            {
            } break;
        }
    }
}

void CKernel::Update()
{}

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
