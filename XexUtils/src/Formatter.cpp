#include "pch.h"
#include "Formatter.h"

namespace XexUtils
{
namespace Formatter
{
	std::string Format(const char* format, ...)
	{
		char buffer[200];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 200, format, args);
		va_end(args);
		return std::string(buffer);
	}

	std::wstring ToWide(const std::string& narrowString)
	{
		std::wstring wideString;
		wideString.assign(narrowString.begin(), narrowString.end());

		return wideString;
	}
}
}