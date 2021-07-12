#pragma once

// *** Класс, обеспечивающий работу с полем зрения камеры
class CViewPoint final
{
private:
    int _w;
    int _h;

public:
    CViewPoint();

public:
    // *** Получить длину поля зрения
    const int& w();

    // *** Получить высоту поля зрения
    const int& h();

public:
    // *** Обновить значения поля зрения
    void Set(const int& w, const int& h);
};
