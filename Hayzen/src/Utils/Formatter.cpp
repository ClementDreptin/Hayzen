#include "pch.h"
#include "Formatter.h"

// TODO: Make those 2 functions use strings instead of const char*

char buffer[0x1000];

char* Formatter::LinkChar(const char* text, ...)
{
	va_list pArgList;
	va_start(pArgList, text);
	vsprintf(buffer, text, pArgList);
	va_end(pArgList);
	return buffer;
}

wchar_t* Formatter::CharToWChar(const char* text)
{
	const size_t cSize = strlen(text) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, cSize, text, cSize);
	return wc;
}