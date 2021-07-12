#include "StdAfx.h"

CViewPoint::CViewPoint() : _w(0), _h(0) {}

const int& CViewPoint::w()
{
    return _w;
}

const int& CViewPoint::h()
{
    return _h;
}

void CViewPoint::Set(const int& w, const int& h)
{
    _w = w;
    _h = h;
}
