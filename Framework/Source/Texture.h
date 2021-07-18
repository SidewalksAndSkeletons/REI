#pragma once

class CTexture final
{
private:
    // Идентификатор текстуры
    GLuint ID;

private:
    // Длина текстуры
    int _w;

    // Высота текстуры
    int _h;

private:
    // Количество кадров в анимации (x * x)
    int FramesCount;

private:
    // Использование собственной матрицы с текстурными координатами
    bool UsingCustomTexCoords;

    // Матрица, содержащая текстурные координаты
    glm::mat4x2 TexCoords;

    // Буфер для работы с матрицей текстурных координат
    glm::vec4 TexBuffer;

public:
    CTexture(const int& w, const int& h);
    ~CTexture();

    // Разрезать текстуру на кадры
    void Crope(const int& w, const int& h);

public:
    // *** Обновить кадр анимации
    void UpdateFrame(const int& FrameIndex);

public:
    // *** Получить (если есть) уникальную матрицу с текстурными координатами
    void GetTexCoords(glm::mat4x2& To);

public:
    // *** Получить идентификатор текстуры
    GLuint& GetID();

public:
    // *** Получить исходную длину текстуры
    const int& w();

    // *** Получить исходную высоту текстуры
    const int& h();
};
