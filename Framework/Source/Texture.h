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

public:
    CTexture();
    ~CTexture();

public:
    // *** Получить идентификатор текстуры
    GLuint& GetID();

public:
    // *** Установить размеры текстуры
    void SetSize(const int& w, const int& h);

public:
    // *** Получить исходную длину текстуры
    const int& w();

    // *** Получить исходную высоту текстуры
    const int& h();
};
