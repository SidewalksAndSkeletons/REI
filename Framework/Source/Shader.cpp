#include "StdAfx.h"

SShader::SShader(const int& NewID) : ID(NewID) {}

SShader::~SShader()
{
    // Освобождаем память, занятую шейдером
    if (ID)
    {
        glDeleteShader(ID);
        ID = 0;
    }
}
