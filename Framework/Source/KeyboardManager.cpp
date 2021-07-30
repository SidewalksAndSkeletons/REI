#include "StdAfx.h"

void CKeyboardManager::Set(SDL_Keycode Key, Uint8 State)
{
    Keys[Key] = State;
}

bool CKeyboardManager::Get(SDL_Keycode Key)
{
    return Keys[Key] == SDL_PRESSED;
}
