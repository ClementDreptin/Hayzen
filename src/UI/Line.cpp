#include "pch.h"
#include "UI/Line.h"

#include "UI/Renderer.h"

void Line::Render(const Props &props)
{
    // Turn the color into a float array
    float color[4] = {
        ((props.Color & 0x00ff0000) >> 16) / 255.0f,
        ((props.Color & 0x0000ff00) >> 8) / 255.0f,
        ((props.Color & 0x000000ff) >> 0) / 255.0f,
        ((props.Color & 0xff000000) >> 24) / 255.0f,
    };

    // Render the line
    Renderer::R_AddCmdDrawStretchPic(
        props.X, props.Y,
        props.Width, props.Height,
        0.0f, 0.0f, 1.0f, 1.0f,
        color,
        Renderer::MaterialHandle
    );
}
