#include "pch.h"
#include "UI/Renderer.h"

namespace Renderer
{

R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStretchPic = nullptr;
R_ADDCMDDRAWTEXT R_AddCmdDrawText = nullptr;
R_REGISTERFONT R_RegisterFont = nullptr;
MATERIAL_REGISTERHANDLE Material_RegisterHandle = nullptr;

Font_s *pFont = nullptr;
HANDLE MaterialHandle = nullptr;

}
