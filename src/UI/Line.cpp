#include "pch.h"
#include "UI/Line.h"

typedef void (*R_ADDCMDDRAWSTRETCHPIC)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *color, HANDLE material);
typedef HANDLE (*MATERIAL_REGISTERHANDLE)(const char *name, int imageTrack);

static R_ADDCMDDRAWSTRETCHPIC R_AddCmdDrawStrechPic = reinterpret_cast<R_ADDCMDDRAWSTRETCHPIC>(0x823BAC18);
static MATERIAL_REGISTERHANDLE Material_RegisterHandle = reinterpret_cast<MATERIAL_REGISTERHANDLE>(0x823B6928);

HANDLE g_Material = nullptr;

void Line::Render(const Props &props)
{
    if (g_Material == nullptr)
        g_Material = Material_RegisterHandle("white", 0);

    float color[4] = {
        ((props.Color & 0x00ff0000) >> 16) / 255.0f,
        ((props.Color & 0x0000ff00) >> 8) / 255.0f,
        ((props.Color & 0x000000ff) >> 0) / 255.0f,
        ((props.Color & 0xff000000) >> 24) / 255.0f,
    };

    R_AddCmdDrawStrechPic(props.X, props.Y, props.Width, props.Height, 0.0f, 0.0f, 1.0f, 1.0f, color, g_Material);
}
