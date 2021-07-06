#pragma once

// *** Вывести сообщение
template <class T>
inline void DEBUG_PRINT(T&& Msg)
{
    Debug.PrintMessage(Msg);
}

// *** Вывести сообщения
template <class T1, class... T2>
inline void DEBUG_PRINT(T1&& First, T2&&... End)
{
    DEBUG_PRINT(First);
    DEBUG_PRINT(End...);
}

// *** Вывести сообщение об ошибке (и закрыть игру)
template <class T>
inline void DEBUG_ERROR(T&& Msg)
{
    // Если ошибка не была обнаружена раннее - подготавливаем всё к закрытию
    if (!Debug.GetErrorStatus())
    {
        // Выходим из программы
        Debug.SetErrorStatus(true);

        // Выводим окно с сообщением об ошибке
        //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ENGINE ERROR", Msg, nullptr);
    }

    // Печатаем сообщение
    DEBUG_PRINT(Msg);
}

// *** Вывести сообщения об ошибке (и закрыть игру)
template <class T1, class... T2>
inline void DEBUG_ERROR(T1&& First, T2&&... End)
{
    // Если ошибка не была обнаружена раннее - подготавливаем всё к закрытию
    if (!Debug.GetErrorStatus())
    {
        // Выходим из программы
        Debug.SetErrorStatus(true);

        // Выводим окно с сообщением об ошибке
        //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ENGINE ERROR", "For details check log-file", nullptr);
    }

    // Печатаем сообщения
    DEBUG_PRINT(First);
    DEBUG_PRINT(End...);
}

// *** Вывести сообщение в лог
#define DEBUG_MESSAGE(...) \
{ \
	Debug.PrintTitle(CDebug::EVENT_TYPES::MESSAGE); \
	DEBUG_PRINT(__VA_ARGS__); \
}

// *** Вывести предупреждение (без краша игры)
#define DEBUG_WARNING(...) \
{ \
	Debug.PrintTitle(CDebug::EVENT_TYPES::WARNING, __FILE__, __LINE__, __FUNCTION__); \
	DEBUG_PRINT(__VA_ARGS__); \
}

// *** Вывести сообщение об ошибке и крашнуть игру
#define DEBUG_ERROR(...) \
{ \
	Debug.PrintTitle(CDebug::EVENT_TYPES::ERROR, __FILE__, __LINE__, __FUNCTION__); \
	DEBUG_ERROR(__VA_ARGS__); \
}
