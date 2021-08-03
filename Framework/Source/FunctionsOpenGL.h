#pragma once

// *** Функции, предназначенные для работы с OpenGL
namespace OpenGL
{
    // *** Получить середину текстуры
    inline glm::vec2 GetCenter(int x, int y, int w, int h)
    {
        return { static_cast<GLfloat>(x + (w / 2)), static_cast<GLfloat>(y + (h / 2)) };
    }

    // *** Произвести вращение относительно координат
    inline void Rotate(glm::vec2& Pos, GLfloat Angle, const glm::vec2& Rotate)
    {
        GLfloat Radians = glm::radians(Angle);

        GLfloat sa = sin(Radians);
        GLfloat ca = cos(Radians);

        Pos.x -= Rotate.x;
        Pos.y -= Rotate.y;

        GLfloat xnew = Pos.x * ca - Pos.y * sa;
        GLfloat ynew = Pos.x * sa + Pos.y * ca;

        Pos.x = xnew + Rotate.x;
        Pos.y = ynew + Rotate.y;
    }
}
