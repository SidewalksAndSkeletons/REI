#include "StdAfx.h"

CGameObject::CGameObject() : _x(0), _y(0)
{
}

CGameObject::CGameObject(const int& x, const int& y) : _x(x), _y(y)
{
}

void CGameObject::SetPosition(const int& x, const int& y)
{
    _x = x;
    _y = y;
}

const int& CGameObject::x()
{
    return _x;
}

const int& CGameObject::y()
{
    return _y;
}
