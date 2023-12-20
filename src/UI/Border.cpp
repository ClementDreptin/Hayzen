#include "pch.h"
#include "UI/Border.h"

void Border::Render(const Props &props)
{
    // Return early if no borders need to be rendered
    if (props.Position == Border_None || props.Thickness == 0)
        return;

    // Render the left border if needed
    if (props.Position & Border_Left)
    {
        Line::Props lineProps = {};
        lineProps.X = props.X - props.Thickness;
        lineProps.Y = props.Y;
        lineProps.Width = props.Thickness;
        lineProps.Height = props.Height + (props.Position & Border_Bottom ? props.Thickness : 0.0f);
        lineProps.Color = props.Color;

        m_Left.Render(lineProps);
    }

    // Render the right border if needed
    if (props.Position & Border_Right)
    {
        Line::Props lineProps = {};
        lineProps.X = props.X + props.Width;
        lineProps.Y = props.Y - (props.Position & Border_Top ? props.Thickness : 0.0f);
        lineProps.Width = props.Thickness;
        lineProps.Height = props.Height + (props.Position & Border_Top ? props.Thickness : 0.0f);
        lineProps.Color = props.Color;

        m_Right.Render(lineProps);
    }

    // Render the top border if needed
    if (props.Position & Border_Top)
    {
        Line::Props lineProps = {};
        lineProps.X = props.X - (props.Position & Border_Left ? props.Thickness : 0.0f);
        lineProps.Y = props.Y - props.Thickness;
        lineProps.Width = props.Width + (props.Position & Border_Left ? props.Thickness : 0.0f);
        lineProps.Height = props.Thickness;
        lineProps.Color = props.Color;

        m_Top.Render(lineProps);
    }

    // Render the bottom border if needed
    if (props.Position & Border_Bottom)
    {
        Line::Props lineProps = {};
        lineProps.X = props.X;
        lineProps.Y = props.Y + props.Height;
        lineProps.Width = props.Width + (props.Position & Border_Right ? props.Thickness : 0.0f);
        lineProps.Height = props.Thickness;
        lineProps.Color = props.Color;

        m_Bottom.Render(lineProps);
    }
}
