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
        Option(const std::string& name, int index);
    private:
        int m_Index;
    };
}
}