#pragma once

// *** Функции, предназначенные для работы со строками
namespace String
{
    // *** Конвертировать std::string в const char**
    struct ToCPtr final
    {
    private:
        const char* Source;

    public:
        explicit ToCPtr(std::string_view Input) : Source(Input.data()) {}

        operator const char** ()
        {
            return &Source;
        }
    };

    // *** Безопасный аналог std::atoi
    // (превращение строки в произвольный тип)
    template <class T>
    inline T atoi(const std::string_view Source)
    {
        T Result{};
        std::from_chars(Source.data(), Source.data() + Source.size(), Result);
        return Result;
    }

    // *** Объединение двух строк
    template <class T>
    inline void Unite(std::string& Source, T&& Input)
    {
        Source += std::forward<T>(Input);
    }

    // *** Объединение нескольких строк
    template <class T1, class... T2>
    inline void Unite(std::string& Source, T1&& Input, T2&&... Other)
    {
        Source += std::forward<T1>(Input);
        Unite(Source, std::forward<T2>(Other)...);
    }

    // *** Запуск с соблюдением иерархии директорий
    inline std::string GetPath(std::string_view Input)
    {
        if (Input.empty())
            return std::string();

        std::string Result = "..//";
        Unite(Result, Input);

        return Result;
    }

    // *** Преобразовать данные в string
    template <class T>
    inline std::string ToString(T&& Data)
    {
        return std::to_string(Data);
    }
}
