#include "StdAfx.h"

CTexture::CTexture(GLuint PreparedID, int w, int h) : Size{ w, h }, ID{ PreparedID },
    Rows{}, FrameSize{}, TexCoords(0.0f), TexBuffer(0.0f)
{
    Shader = "Default";
    FramesCount = 0;
    FrameCurrent = 0;
    FrameSpeed = 0;
    FrameTimer = 0;

    UsingCustomTexCoords = false;
}

CTexture::~CTexture()
{
    if (ID)
    {
        glDeleteTextures(1, &ID);
    }
}

bool CTexture::InitAnimation(tinyxml2::XMLElement* Node)
{
    if (FrameSpeed)
    {
        DEBUG_WARNING("Texture animation already initialized!");
        return true;
    }

    if (!Node)
        return false;

    // Устанавливаем скорость анимации
    if (!GetValue(Node->FirstChildElement("Speed"), FrameSpeed))
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

    // Получаем название шейдера
    std::string ShaderName = GetElementText(Node->FirstChildElement("Shader"));

    if (ShaderName.empty())
    {
        DEBUG_WARNING("Shader type not defined, used default!");
    }
    else
    {
        Shader = std::move(ShaderName);
    }

    return true;
}

void CTexture::Crope(int w, int h, int Frames)
{
    // Устанавливаем размеры одного кадра
    FrameSize = { w, h };

    // Обновляем размеры анимационного листа
    Rows.x = static_cast<int>(ceil(static_cast<GLfloat>(Size.w) / FrameSize.w));
    Rows.y = static_cast<int>(ceil(static_cast<GLfloat>(Size.h) / FrameSize.h));

    // Обновляем количество кадров в анимации
    FramesCount = (Frames > 1) ? Frames : Rows.x * Rows.y;

    // Сбрасываем текущую позицию
    TexBuffer.x = 0.0f;
    TexBuffer.y = 0.0f;

    // Обновляем длину кадра
    TexBuffer.z = static_cast<GLfloat>(FrameSize.w) / Size.w;

    // Обновляем ширину кадра
    TexBuffer.w = static_cast<GLfloat>(FrameSize.h) / Size.h;

    // Теперь мы можем использовать анимацию
    UsingCustomTexCoords = true;

    // Принудительно устанавливаем активным первый кадр анимации
    UpdateFrame(0);
}

void CTexture::UpdateFrame(int FrameIndex)
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
    FrameCurrent = FrameIndex;
}

void CTexture::UpdateAnimation()
{
    Uint32 Ticks = SDL_GetTicks();

    if (Ticks >= FrameTimer)
    {
        FrameTimer = Ticks + FrameSpeed;

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
    return FrameSize.w ? FrameSize.w : Size.w;
}

const int& CTexture::h()
{
    // Если текстура кадрирована - используем высоту кадра
    return FrameSize.h ? FrameSize.h : Size.h;
}

const std::string& CTexture::GetShader()
{
    return Shader;
}
