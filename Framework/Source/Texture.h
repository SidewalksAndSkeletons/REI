#pragma once

#include "TextureAnimation.h"

// *** Класс, представляющий собой обёртку для OpenGL-текстуры
class CTexture final 
{
private:
    // Информация об анимации
    CTextureAnimation* AnimationDetails;

private:
    // Размеры оригинальной текстуры
    SRect<int> Size;

private:
    // Имя используемого шейдера
    std::string Shader;

    // Идентификатор текстуры
    GLuint ID;

public:
    CTexture(GLuint PreparedID, int w, int h);
    ~CTexture();

    // *** Подгрузить данные об анимации из конфига
    // [Основная нода уже открытого конфига; секция с данными об анимации]
    [[nodiscard]] bool InitAnimation(tinyxml2::XMLElement* Node);

public:
    // *** Обновление логики
    void Update();

public:
    // *** Получить (если есть) уникальную матрицу с текстурными координатами
    void GetTexCoords(glm::mat4x2& To);

public:
    // *** Получить идентификатор текстуры
    const GLuint& GetID();

public:
    // *** Получить актуальную длину текстуры
    int w();

    // *** Получить актуальную ширину текстуры
    int h();

public:
    // *** Получить название использующегося шейдера
    const std::string& GetShader();
};
