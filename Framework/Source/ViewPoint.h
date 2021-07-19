#pragma once

// *** Класс, обеспечивающий работу с полем зрения камеры
class CViewPoint final
{
private:
    // Длина поля зрения
    int _w;

    // Ширина поля зрения
    int _h;

public:
    CViewPoint();

public:
    // *** Получить длину поля зрения
    const int& w();

    // *** Получить ширину поля зрения
    const int& h();

public:
    // *** Обновить значения поля зрения
    void Set(const int& w, const int& h);
};
