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
        Option(CONST std::string& name, INT index);
    private:
        INT m_Index;
    };
}
}