#pragma once

namespace Formatter
{
	char* Format(const char* format, ...);

	std::wstring ToWide(const std::string& narrowString);
}