#pragma once

#include "Games\MW2\TextElem.h"

namespace MW2
{
	class Option : public TextElem
	{
	public:
		Option() {};
		Option(const std::string& name, int index);
	private:
		int m_Index;
	};
}