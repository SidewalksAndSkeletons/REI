#pragma once

template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
struct TRect final
{
public:
    T x;
    T y;

public:
    TRect() : x{}, y{} {};

    TRect(T _1) : x{ _1 }, y{ _1 } {};

    TRect(T _1, T _2) : x{ _1 }, y{ _2 } {};

    ~TRect() = default;

public:
    void set(T _1)
    {
        x = _1;
        y = _1;
    }

    void set(T _1, T _2)
    {
        x = _1;
        y = _2;
    }

    T sum()
    {
        return x + y;
    }
};

template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
struct SRect final
{
public:
    T w;
    T h;

public:
    SRect() : w{}, h{} {};

    SRect(T _1) : w{ _1 }, h{ _1 } {};

    SRect(T _1, T _2) : w{ _1 }, h{ _2 } {};

    ~SRect() = default;

public:
    void set(T _1)
    {
        w = _1;
        h = _1;
    }

    void set(T _1, T _2)
    {
        w = _1;
        h = _2;
    }
};
