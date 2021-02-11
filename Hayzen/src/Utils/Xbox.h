#pragma once

class Xbox
{
public:
	static void XNotify(const char* Text, XNOTIFYQUEUEUI_TYPE Type = XNOTIFYUI_TYPE_PREFERRED_REVIEW);
private:
	Xbox() {}
};