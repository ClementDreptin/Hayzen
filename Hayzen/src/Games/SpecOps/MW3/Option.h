#pragma once

#include "Games\SpecOps\MW3\TextElem.h"
#include "Games\SpecOps\MW3\Menu.h"

namespace SpecOps
{
namespace MW3
{
	class Option : public TextElem
	{
	public:
		Option() {};
		Option(const std::string& name, int index, bool active = false);

		std::string GetName() const { return m_Name; }

		void Activate();
		void Deactivate();
	private:
		std::string m_Name;
		int m_Index;
		bool m_Active;
	};
}
}