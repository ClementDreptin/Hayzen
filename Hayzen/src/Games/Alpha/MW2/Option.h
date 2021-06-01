#pragma once

#include "Games\Alpha\MW2\TextElem.h"

namespace Alpha
{
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
}