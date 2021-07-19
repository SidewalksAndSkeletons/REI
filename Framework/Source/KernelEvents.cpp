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
