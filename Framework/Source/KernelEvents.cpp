#include "StdAfx.h"

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

            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                KeyboardManager->Set(CurrentEvent.key.keysym.sym, CurrentEvent.key.state);
            } break;

            default:
            {
            } break;
        }
    }
}
