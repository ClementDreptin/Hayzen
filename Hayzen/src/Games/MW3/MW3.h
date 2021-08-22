#pragma once

namespace MW3
{
    VOID Init();
    BOOL Verify(INT clientNum);
    VOID SafeReset(); // Resets everything when the game was not reset properly
}