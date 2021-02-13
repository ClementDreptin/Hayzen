#pragma once

namespace Xbox
{
	DWORD ResolveFunction(const std::string& moduleName, DWORD ordinal);

	void XNotify(const std::string& text, XNOTIFYQUEUEUI_TYPE type = XNOTIFYUI_TYPE_PREFERRED_REVIEW);
}