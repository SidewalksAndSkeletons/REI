#pragma once

#include "IGameObject.h"

class CGameObject : public IGameObject
{
protected:
    int _x;
    int _y;

public:
    CGameObject();
    CGameObject(const int& x, const int& y);

    virtual ~CGameObject() = default;

public:
    // *** Обновить позицию объекта
    void SetPosition(const int& x, const int& y);

public:
    // *** Получить расположение объекта по оси x
    const int& x();

    // *** Получить расположение объекта по оси y
    const int& y();
};
