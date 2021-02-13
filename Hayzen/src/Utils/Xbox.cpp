#include "pch.h"
#include "Utils\Xbox.h"

#include "Utils\Xam.h"
#include "Utils\Formater.h"

void Xbox::XNotify(const std::string& text, Xam::XNOTIFYQUEUEUI_TYPE type)
{
	Xam::XNotifyQueueUI(type, 0xFF, XNOTIFY_SYSTEM, Formater::CharToWChar(text.c_str()), 0);
}