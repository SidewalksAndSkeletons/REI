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

// *** Точка входа в программу
int main(int, char**)
{
	auto GetErrorCode = [](EXIT_TYPE ERROR_CODE) -> int
	{
		return static_cast<int>(ERROR_CODE);
	};

	return GetErrorCode(EXIT_TYPE::SUCCESS);
}
