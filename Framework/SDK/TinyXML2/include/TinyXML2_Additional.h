#pragma once

// By another author, 2021

namespace tinyxml2
{
	inline std::string GetElementText(XMLElement* CurrentElement)
	{
		return CurrentElement ? CurrentElement->GetText() : std::string();
	}

	// *** �������� boolean-�������� �� ���������� ����
	inline bool GetBool(XMLElement* CurrentElement, bool& Storage)
	{
		std::string Text = GetElementText(CurrentElement);

		if (Text.empty())
			return false;

		// ������� ���������� ����� � ������� ��������������� true
		Storage = (Text == "1" || Text == "true");

		return true;
	}

	template <class T>
	// *** �������� �������� �� ���������� ����
	inline bool GetValue(XMLElement* CurrentElement, T& Storage)
	{
		std::string Text = GetElementText(CurrentElement);

		if (Text.empty())
			return false;

		// ������������ ���������� ������ � ���������� ���
		Storage = static_cast<T>(String::atoi<T>(Text));

		return true;
	}
}