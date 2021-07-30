#pragma once

#include "IGameObject.h"

class CGameObject : public IGameObject
{
protected:
    TRect<int> Position;

public:
    CGameObject();
    CGameObject(int x, int y);

    virtual ~CGameObject() = default;
};
