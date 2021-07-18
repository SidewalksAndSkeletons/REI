#include "StdAfx.h"

CTexture::CTexture(const int& w, const int& h) : ID(0), _w(w), _h(h), FramesCount(0), TexCoords(0.0f), TexBuffer(0.0f), UsingCustomTexCoords(false)
{
}

CTexture::~CTexture()
{
    // Освобождаем память, занятую под текстуру
    if (ID)
    {
        glDeleteTextures(1, &ID);
    }
}

void CTexture::Crope(const int& w, const int& h)
{
    // Обновляем количество кадров в анимации
    FramesCount = _w / w;

    // Сбрасываем текущую позицию
    TexBuffer.x = 0.0f;
    TexBuffer.y = 0.0f;

    // Обновляем длину кадра
    TexBuffer.z = static_cast<GLfloat>(w) / _w;

    // Обновляем ширину кадра
    TexBuffer.w = static_cast<GLfloat>(h) / _h;

    // Теперь мы можем использовать анимацию
    UsingCustomTexCoords = true;

    // Принудительно устанавливаем активным первый кадр анимации
    UpdateFrame(0);
}

void CTexture::UpdateFrame(const int& FrameIndex)
{
    // Работать с анимациями мы можем только при условии проинициализированного TexCoords
    if (!UsingCustomTexCoords)
    {
        DEBUG_WARNING("Could't play animation: used default TexCoords!");
        return;
    }

    // Нельзя проиграть кадр с большим индексом, чем содержится в спрайте
    if (FrameIndex >= (FramesCount * FramesCount))
    {
        DEBUG_WARNING("Could't play frame with index [", FrameIndex, "], because it >= frames count!");
        return;
    }

    // Подготавливаем буфер для отрисовки указанного кадра
    TexBuffer.x = (FrameIndex % FramesCount) * TexBuffer.z;
    TexBuffer.y = (FrameIndex / FramesCount) * TexBuffer.w;

    TexCoords[0][0] = TexBuffer.x;
    TexCoords[0][1] = TexBuffer.y;

    TexCoords[1][0] = TexBuffer.x + TexBuffer.z;
    TexCoords[1][1] = TexBuffer.y;

    TexCoords[2][0] = TexBuffer.x + TexBuffer.z;
    TexCoords[2][1] = TexBuffer.y + TexBuffer.w;

    TexCoords[3][0] = TexBuffer.x;
    TexCoords[3][1] = TexBuffer.y + TexBuffer.w;
}

void CTexture::GetTexCoords(glm::mat4x2& To)
{
    // Если матрица не проинициализирована - будем использовать стандартную
    if (!UsingCustomTexCoords)
        return;

    To = TexCoords;
}

GLuint& CTexture::GetID()
{
    return ID;
}

const int& CTexture::w()
{
    return _w;
}

const int& CTexture::h()
{
    return _h;
}
