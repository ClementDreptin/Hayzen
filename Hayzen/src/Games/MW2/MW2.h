#pragma once

#include "Core\Game.h"
#include "Elements\HudElem.h"


//--------------------------------------------------------------------------------------
// Name: class MW2
// Desc: Class to run MW2 TU9, inherits from Game.
//--------------------------------------------------------------------------------------
class MW2 : public Game
{
public:
    virtual VOID Init();
};


/* namespace MW2
{
    VOID Init();
    BOOL Verify(INT clientNum);
    VOID SafeReset(); // Resets everything when the game was not reset properly
} */
