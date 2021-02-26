#include "pch.h"
#include "Utils\Formatter.h"

std::string Formatter::Format(const char* format, ...)
{
	char buffer[200];
	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, 200, format, args);
	va_end(args);
	return std::string(buffer);
}

std::wstring Formatter::ToWide(const std::string& narrowString)
{
	std::wstring wideString;
	wideString.assign(narrowString.begin(), narrowString.end());

	return wideString;
}