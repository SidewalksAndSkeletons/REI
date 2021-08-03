#include "StdAfx.h"

CTextureAnimation::CTextureAnimation() : Size {}, Rows{}, TexCoords(0.0f), TexBuffer(0.0f)
{
    Timer = 0;
    Count = 0;
    Current = 0;
    Speed = 0;
}

bool CTextureAnimation::Init(SRect<int>& Origin, tinyxml2::XMLElement* Node)
{
    // Устанавливаем скорость анимации
    if (!GetValue(Node->FirstChildElement("Speed"), Speed))
        return false;

    // Получаем количество кадров в листе
    int Buffer = 0;
    if (!GetValue(Node->FirstChildElement("Frames"), Buffer))
        return false;

    // Получаем размеры одного кадра
    int w = 0;
    if (!GetValue(Node->FirstChildElement("w"), w))
        return false;

    int h = 0;
    if (!GetValue(Node->FirstChildElement("h"), h))
        return false;

    Crope(Origin, w, h, Buffer);
    return true;
}

void CTextureAnimation::Crope(SRect<int>& Origin, int w, int h, int Frames)
{
    Size = { w, h };

    // Обновляем размеры анимационного листа (округляем в большую сторону)
    Rows.x = static_cast<int>(ceil(static_cast<GLfloat>(Origin.w) / Size.w));
    Rows.y = static_cast<int>(ceil(static_cast<GLfloat>(Origin.h) / Size.h));

    Count = (Frames > 1) ? Frames : Rows.x * Rows.y;

    TexBuffer.x = 0.0f;
    TexBuffer.y = 0.0f;

    // Обновляем длину кадра
    TexBuffer.z = static_cast<GLfloat>(Size.w) / Origin.w;

    // Обновляем высоту кадра
    TexBuffer.w = static_cast<GLfloat>(Size.h) / Origin.h;

    // Во избежания багов принудительно устанавливаем активным первый кадр анимации
    SetFrame(0);
}

void CTextureAnimation::SetFrame(int FrameIndex)
{
    // Нельзя проиграть кадр с индексом меньше нуля или большем, чем количество кадров, содержащихся в анимации
    if (FrameIndex < 0 || FrameIndex >= Count)
    {
        DEBUG_WARNING("Could't play frame with index [", FrameIndex, "], because it < 0 or >= frames count!");
        return;
    }

    // Подготавливаем буфер для отрисовки указанного кадра
    TexBuffer.x = (FrameIndex % Rows.x) * TexBuffer.z;
    TexBuffer.y = (FrameIndex / (Rows.y + 1)) * TexBuffer.w;

    // Настраиваем матрицу текстурных координат
    TexCoords[0][0] = TexBuffer.x;
    TexCoords[0][1] = TexBuffer.y;

    TexCoords[1][0] = TexBuffer.x + TexBuffer.z;
    TexCoords[1][1] = TexBuffer.y;

    TexCoords[2][0] = TexBuffer.x + TexBuffer.z;
    TexCoords[2][1] = TexBuffer.y + TexBuffer.w;

    TexCoords[3][0] = TexBuffer.x;
    TexCoords[3][1] = TexBuffer.y + TexBuffer.w;

    // Обновляем кадр
    Current = FrameIndex;
}

void CTextureAnimation::Update()
{
    // [TODO]: Место потенциальной оптимизации
    Uint32 Ticks = SDL_GetTicks();

    if (Ticks >= Timer)
    {
        Timer = Ticks + Speed;

        ++Current;

        if (Current >= Count)
        {
            Current = 0;
        }

        SetFrame(Current);
    }
}

void CTextureAnimation::GetTexCoords(glm::mat4x2& To)
{
    To = TexCoords;
}

int CTextureAnimation::w()
{
    return Size.w;
}

int CTextureAnimation::h()
{
    return Size.h;
}
