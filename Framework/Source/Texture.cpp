#include "StdAfx.h"

CTexture::CTexture() : ID(0), _w(0), _h(0)
{
}

CTexture::~CTexture()
{
}

GLuint& CTexture::GetID()
{
    return ID;
}

void CTexture::SetSize(const int& w, const int& h)
{
    _w = w;
    _h = h;
}

const int& CTexture::w()
{
    return _w;
}

const int& CTexture::h()
{
    return _h;
}
