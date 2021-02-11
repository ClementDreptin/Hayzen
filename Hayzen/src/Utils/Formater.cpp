#include "pch.h"
#include "Formater.h"

char buffer[0x1000];

char* Formater::LinkChar(const char* text, ...)
{
	va_list pArgList;
	va_start(pArgList, text);
	vsprintf(buffer, text, pArgList);
	va_end(pArgList);
	return buffer;
}

wchar_t* Formater::CharToWChar(const char* text)
{
	const size_t cSize = strlen(text) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, cSize, text, cSize);
	return wc;
}