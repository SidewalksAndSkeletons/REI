#pragma once

// By another author, 2021

namespace tinyxml2
{
	inline std::string GetElementText(XMLElement* CurrentElement)
	{
		return CurrentElement ? CurrentElement->GetText() : std::string();
	}

	// *** ѕолучить boolean-значение из указанного тега
	inline bool GetBool(XMLElement* CurrentElement, bool& Storage)
	{
		std::string Text = GetElementText(CurrentElement);

		if (Text.empty())
			return false;

		// —вер€ем полученный текст с разными представлени€ми true
		Storage = (Text == "1" || Text == "true");

		return true;
	}

	template <class T>
	// *** ѕолучить значение из указанного тега
	inline bool GetValue(XMLElement* CurrentElement, T& Storage)
	{
		std::string Text = GetElementText(CurrentElement);

		if (Text.empty())
			return false;

		//  онвертируем полученную строку в приемливый вид
		Storage = static_cast<T>(String::atoi<T>(Text));

		return true;
	}
}