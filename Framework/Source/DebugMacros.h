#pragma once

template <class T>
inline void DEBUG_PRINT(T&& Msg)
{
    Debug->PrintMessage(Msg);
}

template <class T1, class... T2>
inline void DEBUG_PRINT(T1&& First, T2&&... End)
{
    DEBUG_PRINT(First);
    DEBUG_PRINT(End...);
}

// *** Вывести сообщение в лог
#define DEBUG_MESSAGE(...) \
{ \
	Debug->PrintTitle(CDebug::EVENT_TYPES::MESSAGE); \
	DEBUG_PRINT(__VA_ARGS__); \
}

// *** Вывести предупреждение (без краша игры)
#define DEBUG_WARNING(...) \
{ \
	Debug->PrintTitle(CDebug::EVENT_TYPES::WARNING, __FILE__, __LINE__, __FUNCTION__); \
	DEBUG_PRINT(__VA_ARGS__); \
}

// *** Вывести сообщение об ошибке и крашнуть игру
#define DEBUG_ERROR(...) \
{ \
	Debug->PrintTitle(CDebug::EVENT_TYPES::ERROR, __FILE__, __LINE__, __FUNCTION__); \
	DEBUG_PRINT(__VA_ARGS__); \
}
