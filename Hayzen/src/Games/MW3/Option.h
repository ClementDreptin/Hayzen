#pragma once

#include "Games\MW3\TextElem.h"

namespace MW3
{
	class Option : public TextElem
	{
	public:
		Option() {};
		Option(int clientNum, const std::string& name, int index);
	private:
		int m_Index;
	};
}