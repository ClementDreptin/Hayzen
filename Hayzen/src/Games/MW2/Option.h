#pragma once

#include "Games\MW2\TextElem.h"

namespace MW2
{
    class Option : public TextElem
    {
    public:
        Option() {};
        Option(INT clientNum, CONST std::string& name, INT index);
    private:
        INT m_Index;
    };
}