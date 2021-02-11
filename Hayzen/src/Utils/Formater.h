#pragma once

class Formater
{
public:
	static char* LinkChar(const char* text, ...);
	static wchar_t* CharToWChar(const char* text);
private:
	Formater() {}
};