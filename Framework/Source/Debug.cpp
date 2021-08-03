#include "StdAfx.h"

CDebug::CDebug()
{
    ErrorStatus = false;
}

CDebug::~CDebug()
{
    if (LogFile.is_open())
    {
        LogFile.close();
    }
}

bool CDebug::Init()
{
    // Без директорий некуда будет создавать файлы
    FS::CreateDirectories("Logs");

    // Создаём лог-файл с указанным названием
    LogFile.open(GetLogName());

    if (!LogFile.is_open())
        return false;

    // Всё прошло успешно
    PrintMessage("Debug initialization is success!");

    return true;
}

std::string CDebug::GetLogName()
{
    // Получаем локальное время
    time_t Time = time(0);
    struct tm LocalTime;
    localtime_s(&LocalTime, &Time);

    // Соединяем все полученные данные в одну строку
    std::string LogName = String::GetPath
    (
        // Устанавливаем папку, в которой будет храниться файл
        "Logs//[" +
        // Получаем текущий день
        String::ToString(LocalTime.tm_mday)
        + '.' +
        // Получаем текущий месяц
        String::ToString(LocalTime.tm_mon + 1)
        + ' ' +
        // Получаем текущий час
        String::ToString(LocalTime.tm_hour)
        + '.' +
        // Получаем текущую минуту
        String::ToString(LocalTime.tm_min)
        // Получаем тип файла
        + "].log"
    );

    return LogName;
}

bool CDebug::GetErrorStatus() const
{
    return ErrorStatus;
}

void CDebug::PrintTitle(EVENT_TYPES Type, const char* File, int Line, const char* Function)
{
    // Начало нового сообщения
    std::string Result = "\n\n";

    // Выбираем титульник сообщения
    switch (Type)
    {
        case EVENT_TYPES::MESSAGE:
        {
            Result += "~ [MESSAGE]\n";
        } break;

        case EVENT_TYPES::WARNING:
        {
            Result += "# [WARNING]\n";
        } break;

        case EVENT_TYPES::ERROR:
        {
            Result += "! [ERROR]\n";
        } break;

        default:
        {
            Result += "? [UNKNOWN TYPE]\n";
        } break;
    }

    // Если надо - указываем файл
    if (File)
    {
        String::Unite(Result, "FILE: \t", File, '\n');

        // ...строку
        if (Line)
        {
            String::Unite(Result, "LINE: \t", String::ToString(Line), '\n');
        }

        // ...функцию
        if (Function)
        {
            String::Unite(Result, "FUNC: \t", Function, '\n');
        }
    }

    // Добавляем описание самого сообщения
    Result += "DESCR: \t";

    // Печатаем титульник сообщения
    PrintMessage(Result);

    // Если произошла ошибка
    if (Type == EVENT_TYPES::ERROR)
    {
        // Если ошибка не была обнаружена раннее - подготавливаем всё к закрытию
        if (!GetErrorStatus())
        {
            // При следующей итерации начинаем выход из программы
            ErrorStatus = true;

            // Выводим окно с сообщением об ошибке
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ENGINE ERROR", "For details check log-file", nullptr);
        }
    }
}
