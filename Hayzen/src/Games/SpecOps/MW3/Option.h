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
        Option(CONST std::string& name, INT index);
    private:
        INT m_Index;
    };
}
}