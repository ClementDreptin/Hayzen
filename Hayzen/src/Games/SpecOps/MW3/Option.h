#pragma once

#include "Games\SpecOps\MW3\TextElem.h"

namespace SpecOps
{
namespace MW3
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