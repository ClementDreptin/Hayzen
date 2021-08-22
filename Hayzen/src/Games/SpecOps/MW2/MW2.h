#pragma once

namespace SpecOps
{
namespace MW2
{
    VOID Init();
    BOOL Verify(INT clientNum);
    VOID SafeReset(); // Resets everything when the game was not reset properly
}
}