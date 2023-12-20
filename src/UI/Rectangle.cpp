#include "pch.h"
#include "UI/Rectangle.h"

#include "UI/Renderer.h"

void Rectangle::Render(const Props &props)
{
    bool hasBorder = props.BorderPosition != Border::Border_None && props.BorderWidth > 0;

    // Turn the color into a float array
    float color[4] = {
        ((props.Color & 0x00ff0000) >> 16) / 255.0f,
        ((props.Color & 0x0000ff00) >> 8) / 255.0f,
        ((props.Color & 0x000000ff) >> 0) / 255.0f,
        ((props.Color & 0xff000000) >> 24) / 255.0f,
    };

    // Render the rectangle
    Renderer::R_AddCmdDrawStretchPic(
        props.X, props.Y,
        props.Width, props.Height,
        0.0f, 0.0f, 1.0f, 1.0f,
        color,
        Renderer::MaterialHandle
    );

    // Render the border if needed
    if (hasBorder)
    {
        Border::Props borderProps = {};
        borderProps.X = props.X;
        borderProps.Y = props.Y;
        borderProps.Thickness = props.BorderWidth;
        borderProps.Color = props.BorderColor;
        borderProps.Position = props.BorderPosition;
        borderProps.Width = props.Width;
        borderProps.Height = props.Height;

        m_Border.Render(borderProps);
    }
}
