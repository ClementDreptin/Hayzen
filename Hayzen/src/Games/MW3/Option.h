#pragma once

#include "Games\MW3\TextElem.h"

namespace MW3
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