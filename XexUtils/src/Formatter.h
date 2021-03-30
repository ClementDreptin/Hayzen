#pragma once

namespace XexUtils
{
namespace Formatter
{
	std::string Format(const char* format, ...);

	std::wstring ToWide(const std::string& narrowString);
}
}