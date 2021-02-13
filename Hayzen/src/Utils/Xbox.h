#pragma once

#include "Utils\Xam.h"

namespace Xbox
{
	DWORD ResolveFunction(const std::string& moduleName, DWORD ordinal);

	void XNotify(const std::string& text, Xam::XNOTIFYQUEUEUI_TYPE type = Xam::XNOTIFYUI_TYPE_PREFERRED_REVIEW);
}