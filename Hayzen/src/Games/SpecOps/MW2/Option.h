#pragma once

#include "Games\SpecOps\MW2\TextElem.h"

namespace SpecOps
{
namespace MW2
{
    class Option : public TextElem
    {
    public:
        Option() {};
        Option(CONST std::string& name, INT index);
    private:
        INT m_Index;
    };
}
}