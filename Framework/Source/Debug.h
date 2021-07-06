#pragma once

// *** Класс, обеспечивающий работу с отладкой
class CDebug final
{
private:
    // Типы событий в логе
    enum class EVENT_TYPES
    {
        MESSAGE,
        WARNING,
        ERROR
    };

private:
    std::ofstream LogFile;

private:
    // Статус пойманной ошибки
    bool ErrorStatus;

public:
    CDebug();
    ~CDebug();

    [[nodiscard]] bool Init();

public:
    // *** Получить запись вида: "месяц.число часы.минуты"
    std::string GetLogName();

public:
    // *** Установить статус пойманной ошибки
    void SetErrorStatus(const bool Value);

    // *** Получить статус пойманной ошибки
    bool GetErrorStatus() const;

private:
    // *** Вывести заголовок
    void PrintTitle(EVENT_TYPES Type, const char* File, int Line, const char* Function);

    // *** Вывести сообщение
    template <class T>
    void PrintMessage(T&& Msg)
    {
        if (Msg)
        {
            if (LogFile.is_open())
            {
                LogFile << Msg;
            }

#ifdef DEBUG
            PrintToSubSystem(Msg);
#endif
        }
    }

#ifdef DEBUG
    // *** Вывести сообщение в консоль
    template <class T>
    inline void PrintToSubSystem(T&& Msg)
    {
        if (std::cout)
        {
            std::cout << Msg;
        }
    }
#endif
};

inline CDebug Debug;
