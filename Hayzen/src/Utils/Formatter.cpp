#include "pch.h"
#include "Utils\Formatter.h"

char* Formatter::Format(const char* format, ...)
{
	char* buffer = new char[200];
	va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
	va_end(args);
	return buffer;
}

std::wstring Formatter::ToWide(const std::string& narrowString)
{
	std::wstring wideString;
	wideString.assign(narrowString.begin(), narrowString.end());

	return wideString;
}