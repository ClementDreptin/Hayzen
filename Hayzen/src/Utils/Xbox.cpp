#include "pch.h"
#include "Xbox.h"

#include "Formater.h"

void Xbox::XNotify(const char* Text, XNOTIFYQUEUEUI_TYPE Type)
{
	XNotifyQueueUI(Type, 0xFF, XNOTIFY_SYSTEM, Formater::CharToWChar(Text), 0);
}