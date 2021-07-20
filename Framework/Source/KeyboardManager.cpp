#include "StdAfx.h"

void CKeyboardManager::Set(const SDL_Keycode& Key, const Uint8& State)
{
    Keys[Key] = State;
}

bool CKeyboardManager::Get(const SDL_Keycode& Key)
{
    return Keys[Key] == SDL_PRESSED;
}
