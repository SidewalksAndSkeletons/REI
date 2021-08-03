#include "StdAfx.h"

// *** Коды завершения программы
enum class EXIT_TYPE : int
{
	// Успешно
	SUCCESS = 0,

	// Ошибка отладчика
	DEBUG_ERROR,

	// Ошибка инициализации
	INIT_ERROR,

	// Ошибка во время выполнения
	RUNTIME_ERROR,

	// Неизвестная ошибка
	UNDEFINED_ERROR
};

int main(int KeysCount, char** Keys)
{
    auto GetErrorCode = [](EXIT_TYPE ERROR_CODE) -> int
    {
        return static_cast<int>(ERROR_CODE);
    };

    Debug = Debug->CreateInstance();

    // Инициализируем отладочный модуль
    if (!Debug->Init())
    {
        return GetErrorCode(EXIT_TYPE::DEBUG_ERROR);
    }

    std::shared_ptr<CKernel> Kernel = Kernel->CreateInstance();

    // Инициализируем ядро движка
    if (!Kernel->Init())
    {
        return GetErrorCode(EXIT_TYPE::INIT_ERROR);
    }

    Kernel->ParseKeys(KeysCount, Keys);

    while (Kernel->GetRunningStatus() && !Debug->GetErrorStatus())
    {
        Kernel->Render();

        Kernel->HandleEvents();

        Kernel->Update();
    }

    return GetErrorCode(Debug->GetErrorStatus() ? EXIT_TYPE::RUNTIME_ERROR : EXIT_TYPE::SUCCESS);
}
