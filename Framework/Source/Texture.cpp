#include "StdAfx.h"

CTexture::CTexture(const GLuint& PreparedID, const int& w, const int& h) : ID(PreparedID), _w(w), _h(h), TexCoords(0.0f), TexBuffer(0.0f)
{
    _wc = 0;
    _hc = 0;

    FramesCount = 0;
    FrameCurrent = 0;
    FramesSpeed = 0;
    FrameTimer = 0;

    UsingCustomTexCoords = false;
}

CTexture::~CTexture()
{
    // Освобождаем память, занятую под текстуру
    if (ID)
    {
        glDeleteTextures(1, &ID);
    }
}

bool CTexture::InitAnimation(tinyxml2::XMLElement* Node)
{
    if (!Node)
        return false;

    // Устанавливаем скорость анимации
    if (!GetValue(Node->FirstChildElement("Speed"), FramesSpeed))
        return false;

    // Получаем количество кадров в листе
    int Buffer = 0;
    if (!GetValue(Node->FirstChildElement("Frames"), Buffer))
        return false;

    // Получаем размеры одного кадра
    int w = 0;
    int h = 0;

    if (!GetValue(Node->FirstChildElement("w"), w))
        return false;

    if (!GetValue(Node->FirstChildElement("h"), h))
        return false;

    // Кадрируем текстуру
    Crope(w, h, Buffer);
    return true;
}

void CTexture::Crope(const int& w, const int& h, const int& Frames)
{
    // Устанавливаем размеры одного кадра
    _wc = w;
    _hc = h;

    // Обновляем количество кадров в анимации
    FramesCount = (Frames > 1) ? Frames : (_w / _wc) * (_w / _hc);

    // Сбрасываем текущую позицию
    TexBuffer.x = 0.0f;
    TexBuffer.y = 0.0f;

    // Обновляем длину кадра
    TexBuffer.z = 1.0f / _w * static_cast<GLfloat>(_wc);

    // Обновляем ширину кадра
    TexBuffer.w = 1.0f / _h * static_cast<GLfloat>(_hc);

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

    // Нельзя проиграть кадр с индексом меньше нуля или большем, чем количество кадров, содержащихся в анимации
    if (FrameIndex < 0 || FrameIndex >= FramesCount)
    {
        DEBUG_WARNING("Could't play frame with index [", FrameIndex, "], because it < 0 or >= frames count!");
        return;
    }

    // Подготавливаем буфер для отрисовки указанного кадра
    TexBuffer.x = (FrameIndex % (_w / _wc)) * TexBuffer.z;
    TexBuffer.y = (FrameIndex / ((_h / _hc) + 1)) * TexBuffer.w;

    TexCoords[0][0] = TexBuffer.x;
    TexCoords[0][1] = TexBuffer.y;

    TexCoords[1][0] = TexBuffer.x + TexBuffer.z;
    TexCoords[1][1] = TexBuffer.y;

    TexCoords[2][0] = TexBuffer.x + TexBuffer.z;
    TexCoords[2][1] = TexBuffer.y + TexBuffer.w;

    TexCoords[3][0] = TexBuffer.x;
    TexCoords[3][1] = TexBuffer.y + TexBuffer.w;

    // Обновляем кадр
    FrameCurrent = FrameIndex;
}

void CTexture::UpdateAnimation()
{
    Uint32 Ticks = SDL_GetTicks();

    if (Ticks >= FrameTimer)
    {
        FrameTimer = Ticks + FramesSpeed;

        ++FrameCurrent;

        if (FrameCurrent >= FramesCount)
        {
            FrameCurrent = 0;
        }

        UpdateFrame(FrameCurrent);
    }
}

void CTexture::GetTexCoords(glm::mat4x2& To)
{
    // Если матрица не проинициализирована - будем использовать стандартную
    if (!UsingCustomTexCoords)
        return;

    To = TexCoords;
}

const GLuint& CTexture::GetID()
{
    return ID;
}

const int& CTexture::w()
{
    // Если текстура кадрирована - используем длину кадра
    return _wc ? _wc : _w;
}

const int& CTexture::h()
{
    // Если текстура кадрирована - используем высоту кадра
    return _hc ? _hc : _h;
}
