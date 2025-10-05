#include "pch.h"
#include "Modules/Console.h"

#include "Core/Config.h"
#include "Core/UI.h"

#define MAX_CHAR_COUNT 128
#define MAX_HISTORY_SIZE 30
#define MAX_AUTOCOMPLETE_MATCHES 25

static const float s_SafeAreaPercentage = 90.0f;
static const float s_SafeAreaWidth = UI::DisplayWidth * s_SafeAreaPercentage / 100.0f;
static const float s_SafeAreaHeight = UI::DisplayHeight * s_SafeAreaPercentage / 100.0f;
static const float s_SafeAreaOffsetX = (UI::DisplayWidth - s_SafeAreaWidth) / 2.0f;
static const float s_SafeAreaOffsetY = (UI::DisplayHeight - s_SafeAreaHeight) / 2.0f;

namespace StringUtils
{

std::string ConvertWideCharToAscii(wchar_t wideChar);

bool IContains(const std::string &haystack, const std::string &needle);

bool IEquals(const std::string &str1, const std::string &str2);

}

Console::Props::Props(CBUF_ADDTEXT Cbuf_AddText, DVAR_FOREACH Dvar_ForEach)
    : Cbuf_AddText(Cbuf_AddText), Dvar_ForEach(Dvar_ForEach)
{
}

Console::Console(const Props &props)
    : m_Props(props), m_Open(false), m_CursorPosition(0), m_HistoryIndex(0), m_AutocompleteIndex(static_cast<size_t>(-1))
{
    m_Command.reserve(MAX_CHAR_COUNT);
}

void Console::Update()
{
    XINPUT_KEYSTROKE keystroke = {};
    uint32_t result = XInputGetKeystroke(0, XINPUT_FLAG_KEYBOARD, &keystroke);

    if (result == ERROR_EMPTY || result == ERROR_DEVICE_NOT_CONNECTED)
        return;

    if (result != ERROR_SUCCESS)
    {
        DebugPrint("[Hayzen][Console]: Unexpected error while getting keystroke: %X.", result);
        return;
    }

    // Flags set to 0 means no keys were pressed at the current frame, we also ignore keyup events
    if (keystroke.Flags == 0 || keystroke.Flags & XINPUT_KEYSTROKE_KEYUP)
        return;

    // Toggle the console with VK_OEM_3 ("~" on qwerty keyboard)
    if (keystroke.VirtualKey == VK_OEM_3)
    {
        m_Open = !m_Open;
        return;
    }

    // Close the console with the escape key
    if (keystroke.VirtualKey == VK_ESCAPE)
    {
        m_Open = false;
        return;
    }

    // Move cursor left
    if (keystroke.VirtualKey == VK_LEFT)
    {
        if (m_CursorPosition > 0)
            m_CursorPosition--;

        return;
    }

    // Move cursor right
    if (keystroke.VirtualKey == VK_RIGHT)
    {
        if (m_CursorPosition < m_Command.size())
            m_CursorPosition++;

        return;
    }

    // Send cursor to end of line
    if (keystroke.VirtualKey == VK_END)
    {
        m_CursorPosition = m_Command.size();
        return;
    }

    // Get previous command in history
    if (keystroke.VirtualKey == VK_UP)
    {
        if (m_HistoryIndex > 0)
        {
            m_HistoryIndex--;
            SetCommand(m_History[m_HistoryIndex]);
        }

        return;
    }

    if (keystroke.VirtualKey == VK_DOWN)
    {
        // Get next command in history
        if (!m_History.empty() && m_HistoryIndex < m_History.size())
        {
            // We clear the command when we reach the bottom of the history, so after the most recent command
            m_HistoryIndex++;
            SetCommand(m_HistoryIndex < m_History.size() ? m_History[m_HistoryIndex] : "");
            return;
        }

        // Cycle through autocomplete matches if we're at the end of the history
        if (!m_AutocompleteMatches.empty())
            CycleThroughAutocompleteMatches(false);

        return;
    }

    // Cycle through autocomplete matches
    if (keystroke.VirtualKey == VK_TAB)
    {
        if (!m_AutocompleteMatches.empty())
            CycleThroughAutocompleteMatches((keystroke.Flags & XINPUT_KEYSTROKE_SHIFT) != 0);

        return;
    }

    // Send command with the enter key
    if (keystroke.VirtualKey == VK_RETURN)
    {
        Submit();
        return;
    }

    // Erase previous character with back key
    if (keystroke.VirtualKey == VK_BACK)
    {
        if (!m_Command.empty() && m_CursorPosition > 0)
        {
            m_Command.erase(m_CursorPosition - 1, 1);
            m_CursorPosition--;
            UpdateAutocompleteMatches();
        }

        return;
    }

    // Erase next character with delete key
    if (keystroke.VirtualKey == VK_DELETE)
    {
        if (!m_Command.empty() && m_CursorPosition < m_Command.size())
        {
            m_Command.erase(m_CursorPosition, 1);
            UpdateAutocompleteMatches();
        }

        return;
    }

    // Insert character if it's unicode valid
    if (keystroke.Unicode != 0 && m_Command.size() < MAX_CHAR_COUNT)
    {
        std::string ascii = StringUtils::ConvertWideCharToAscii(keystroke.Unicode);
        m_Command.insert(m_CursorPosition, ascii);
        m_CursorPosition += ascii.size();
        UpdateAutocompleteMatches();

        return;
    }
}

void Console::Render()
{
    XASSERT(UI::pConsoleFont != nullptr);

    if (!m_Open)
        return;

    // Use the console font
    UI::pFont = UI::pConsoleFont;

    // Draw the command
    {
        UI::TextProps props = {};
        props.X = s_SafeAreaOffsetX;
        props.Y = s_SafeAreaOffsetY;
        props.Text = m_Command;
        props.Color = g_Config.TextColor;
        props.BackgroundColor = g_Config.BackgroundColor;
        props.BackgroundWidth = s_SafeAreaWidth;
        props.BorderWidth = 1.0f;
        props.BorderColor = g_Config.Color;
        props.BorderPosition = UI::BorderPosition_All;
        UI::DrawText(props);
    }

    // Draw the cursor
    {
        UI::LineProps props = {};
        props.X = s_SafeAreaOffsetX + g_Config.Padding + UI::GetTextWidth(m_Command.substr(0, m_CursorPosition));
        props.Y = s_SafeAreaOffsetY + g_Config.Padding;
        props.Color = g_Config.Color;
        props.Width = 2.0f;
        props.Height = UI::GetFontHeight();
        UI::DrawLine(props);
    }

    // Draw the autocomplete matches
    if (!m_AutocompleteMatches.empty())
        DrawAutocompleteMatches();

    // If there's an exact match, draw extended details about the dvar
    if (m_AutocompleteMatches.size() == 1)
        DrawExactMatchExtendedDetails();

    // Restore the default font
    UI::pFont = UI::pDefaultFont;
}

void Console::Submit()
{
    XASSERT(m_Props.Cbuf_AddText != nullptr);

    // It is common to prepend commands with a slash when using the official consoles
    // but this leading slash is removed before calling Cbuf_AddText
    const std::string &commandWithoutLeadingSlash =
        !m_Command.empty() && m_Command[0] == '/'
            ? m_Command.substr(1)
            : m_Command;

    // Execute the command
    m_Props.Cbuf_AddText(0, commandWithoutLeadingSlash.c_str());

    AppendToHistory();

    // Clear the input
    SetCommand("");
    ResetAutocompleteMatches();
}

void Console::SetCommand(const std::string &command)
{
    m_Command = command;
    m_CursorPosition = m_Command.size();
}

bool Console::AnySpaceAfterCommand()
{
    auto firstNonWhitespace = std::find_if(m_Command.begin(), m_Command.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });

    if (firstNonWhitespace == m_Command.end())
        return false;

    auto firstWhitespaceAfterCommand = std::find_if(firstNonWhitespace, m_Command.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });

    return firstWhitespaceAfterCommand != m_Command.end();
}

void Console::AppendToHistory()
{
    m_History.emplace_back(m_Command);

    // Make the history rotate
    if (m_History.size() > MAX_HISTORY_SIZE)
        m_History.pop_front();

    m_HistoryIndex = m_History.size();
}

void Console::UpdateAutocompleteMatches()
{
    XASSERT(m_Props.Dvar_ForEach != nullptr);

    ResetAutocompleteMatches();
    GenerateAutocompleteQuery();

    if (m_AutocompleteQuery.empty())
        return;

    bool requiresExactMatch = AnySpaceAfterCommand();
    if (requiresExactMatch)
        m_Props.Dvar_ForEach(FindExactDvarCallback, this);
    else
        m_Props.Dvar_ForEach(FindMatchingDvarsCallback, this);
}

void Console::FindMatchingDvarsCallback(const dvar_t *dvar, void *data)
{
    Console *This = static_cast<Console *>(data);

    if (This->m_AutocompleteMatches.size() >= MAX_AUTOCOMPLETE_MATCHES)
        return;

    if (StringUtils::IContains(dvar->name, This->m_AutocompleteQuery))
        This->m_AutocompleteMatches.emplace_back(dvar);
}

void Console::FindExactDvarCallback(const dvar_t *dvar, void *data)
{
    Console *This = static_cast<Console *>(data);

    if (!This->m_AutocompleteMatches.empty())
        return;

    if (StringUtils::IEquals(dvar->name, This->m_AutocompleteQuery))
        This->m_AutocompleteMatches.emplace_back(dvar);
}

void Console::GenerateAutocompleteQuery()
{
    auto firstNonWhitespace = std::find_if(m_Command.begin(), m_Command.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });

    if (firstNonWhitespace == m_Command.end())
        m_AutocompleteQuery.clear();

    auto firstWhitespaceAfterCommand = std::find_if(firstNonWhitespace, m_Command.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });

    m_AutocompleteQuery = std::string(firstNonWhitespace, firstWhitespaceAfterCommand);
}

void Console::CycleThroughAutocompleteMatches(bool shiftPressed)
{
    size_t lastIndex = m_AutocompleteMatches.size() - 1;

    // clang-format off

    // Cycle backwards if shift is held
    if (shiftPressed)
    {
        if (m_AutocompleteIndex > 0 && m_AutocompleteIndex <= lastIndex)
            m_AutocompleteIndex--;
        else
            m_AutocompleteIndex = lastIndex;
    }
    else
    {
        if (m_AutocompleteIndex < lastIndex)
            m_AutocompleteIndex++;
        else
            m_AutocompleteIndex = 0;
    }

    // clang-format on

    SetCommand(m_AutocompleteMatches[m_AutocompleteIndex]->name);
}

void Console::ResetAutocompleteMatches()
{
    m_AutocompleteMatches.clear();
    m_AutocompleteQuery.clear();
    m_AutocompleteIndex = static_cast<size_t>(-1);
}

void Console::DrawAutocompleteMatches()
{
    float columnWidth = s_SafeAreaWidth / 16;
    float oneLineHeight = UI::GetFontHeight() + g_Config.Padding * 2;
    float textInputHeight = oneLineHeight;
    float headerHeight = oneLineHeight;

    // Common text props
    UI::TextProps commonText = {};
    commonText.Y = s_SafeAreaOffsetY + textInputHeight + g_Config.Padding * 2;
    commonText.Color = g_Config.TextColor;
    commonText.BackgroundColor = g_Config.BackgroundColor;
    commonText.BorderColor = g_Config.Color;
    commonText.BorderWidth = 1.0f;
    commonText.BorderPosition = UI::BorderPosition_Left | UI::BorderPosition_Top;

    // Draw name header
    UI::TextProps nameHeader = commonText;
    nameHeader.X = s_SafeAreaOffsetX;
    nameHeader.Text = "Name";
    nameHeader.BackgroundWidth = columnWidth * 4;
    UI::DrawText(nameHeader);

    // Draw value header
    UI::TextProps valueHeader = commonText;
    valueHeader.X = nameHeader.X + nameHeader.BackgroundWidth;
    valueHeader.Text = "Value";
    valueHeader.BackgroundWidth = columnWidth * 2;
    UI::DrawText(valueHeader);

    // Draw description header
    UI::TextProps descriptionHeader = commonText;
    descriptionHeader.X = valueHeader.X + valueHeader.BackgroundWidth;
    descriptionHeader.Text = "Description";
    descriptionHeader.BackgroundWidth = columnWidth * 10;
    descriptionHeader.BorderPosition = UI::BorderPosition_All;
    UI::DrawText(descriptionHeader);

    // Draw the info of each dvar
    for (size_t i = 0; i < m_AutocompleteMatches.size(); i++)
    {
        const dvar_t &dvar = *m_AutocompleteMatches[i];

        commonText.Y = nameHeader.Y + headerHeight + (UI::GetFontHeight() + g_Config.Padding) * i;
        commonText.BorderPosition = UI::BorderPosition_Left | UI::BorderPosition_Right;

        if (i == 0)
            commonText.BorderPosition = commonText.BorderPosition | UI::BorderPosition_Top;
        if (i == m_AutocompleteMatches.size() - 1)
            commonText.BorderPosition = commonText.BorderPosition | UI::BorderPosition_Bottom;

        // Draw name
        UI::TextProps name = commonText;
        name.X = nameHeader.X;
        name.Text = dvar.name;
        name.BackgroundWidth = nameHeader.BackgroundWidth;
        UI::DrawText(name);

        // Draw value
        UI::TextProps value = commonText;
        value.X = valueHeader.X;
        value.Text = DvarValueToString(dvar, dvar.current);
        value.BackgroundWidth = valueHeader.BackgroundWidth;
        UI::DrawText(value);

        // Draw description
        UI::TextProps description = commonText;
        description.X = descriptionHeader.X;
        description.Text = dvar.description;
        description.BackgroundWidth = descriptionHeader.BackgroundWidth;
        UI::DrawText(description);
    }
}

void Console::DrawExactMatchExtendedDetails()
{
    XASSERT(m_AutocompleteMatches.size() == 1);

    float oneLineHeight = UI::GetFontHeight() + g_Config.Padding * 2;
    float textInputHeight = oneLineHeight;
    float headerHeight = oneLineHeight;
    float autocompleteMatchesHeight = headerHeight + m_AutocompleteMatches.size() * oneLineHeight;

    const dvar_t &dvar = *m_AutocompleteMatches[0];

    UI::TextProps props = {};
    props.X = s_SafeAreaOffsetX;
    props.Y = s_SafeAreaOffsetY + textInputHeight + autocompleteMatchesHeight + g_Config.Padding * 4;
    props.Text = Formatter::Format("Default: %s\n%s", DvarValueToString(dvar, dvar.reset).c_str(), DvarDomainToString(dvar).c_str());
    props.Color = g_Config.TextColor;
    props.BackgroundColor = g_Config.BackgroundColor;
    props.BackgroundWidth = s_SafeAreaWidth;
    props.BorderColor = g_Config.Color;
    props.BorderWidth = 1.0f;
    props.BorderPosition = UI::BorderPosition_All;
    UI::DrawText(props);
}

std::string Console::DvarValueToString(const dvar_t &dvar, const DvarValue &value)
{
    if (dvar.type == DVAR_TYPE_BOOL)
        return value.enabled ? "1" : "0";

    if (dvar.type == DVAR_TYPE_FLOAT)
        return Formatter::Format("%g", value.value);

    if (dvar.type == DVAR_TYPE_FLOAT_2)
        return Formatter::Format("%g %g", value.vector[0], value.vector[1]);

    if (dvar.type == DVAR_TYPE_FLOAT_3_COLOR || dvar.type == DVAR_TYPE_FLOAT_3)
        return Formatter::Format(
            "%g %g %g",
            value.vector[0], value.vector[1], value.vector[2]
        );

    if (dvar.type == DVAR_TYPE_FLOAT_4)
        return Formatter::Format(
            "%g %g %g %g",
            value.vector[0], value.vector[1], value.vector[2], value.vector[3]
        );

    if (dvar.type == DVAR_TYPE_INT)
        return Formatter::Format("%i", value.integer);

    if (dvar.type == DVAR_TYPE_ENUM)
    {
        XASSERT(value.integer < dvar.domain.enumeration.stringCount);
        return dvar.domain.enumeration.strings[value.integer];
    }

    if (dvar.type == DVAR_TYPE_STRING)
        return value.string;

    if (dvar.type == DVAR_TYPE_COLOR)
        return Formatter::Format(
            "%d %d %d %d",
            value.color[0], value.color[1], value.color[2], value.color[3]
        );

    // We should never end up here
    return "UNKNOWN_TYPE";
}

std::string Console::DvarDomainToString(const dvar_t &dvar)
{
    if (dvar.type == DVAR_TYPE_BOOL)
        return "Domain is 0 or 1.";

    if (dvar.type == DVAR_TYPE_FLOAT)
    {
        // This should be FLT_MIN but this is how it's implemented in the Dvar_DomainToString function from the games
        const float minFloat = -FLT_MAX;
        const float maxFloat = FLT_MAX;

        float min = dvar.domain.value.min;
        float max = dvar.domain.value.max;

        if (min == minFloat && max == maxFloat)
            return "Domain is any number";

        if (min == minFloat)
            return Formatter::Format("Domain is any number %g or smaller.", max);

        if (max == maxFloat)
            return Formatter::Format("Domain is any number %g or bigger.", min);

        return Formatter::Format("Domain is any number from %g to %g.", min, max);
    }

    if (dvar.type == DVAR_TYPE_FLOAT_2)
        return DvarVectorDomainToString(2, dvar);

    if (dvar.type == DVAR_TYPE_FLOAT_3_COLOR || dvar.type == DVAR_TYPE_FLOAT_3)
        return DvarVectorDomainToString(3, dvar);

    if (dvar.type == DVAR_TYPE_FLOAT_4)
        return DvarVectorDomainToString(4, dvar);

    if (dvar.type == DVAR_TYPE_INT)
    {
        int min = dvar.domain.integer.min;
        int max = dvar.domain.integer.max;

        if (min == INT_MIN && max == INT_MAX)
            return "Domain is any integer.";

        if (min == INT_MIN)
            return Formatter::Format("Domain is any integer %i or smaller.", max);

        if (max == INT_MAX)
            return Formatter::Format("Domain is any integer %i or bigger.", min);

        return Formatter::Format("Domain is any integer from %i to %i.", min, max);
    }

    if (dvar.type == DVAR_TYPE_ENUM)
    {
        std::stringstream result;
        result << "Domain is one of the following:";

        size_t stringCount = static_cast<size_t>(dvar.domain.enumeration.stringCount);
        for (size_t i = 0; i < stringCount; i++)
            result << Formatter::Format("\n  %2i: %s", i, dvar.domain.enumeration.strings[i]);

        return result.str();
    }

    if (dvar.type == DVAR_TYPE_STRING)
        return "Domain is any text.";

    if (dvar.type == DVAR_TYPE_COLOR)
        return "Domain is any 4-component color, in RGBA format.";

    // We should never end up here
    return "UNKNOWN_TYPE";
}

std::string Console::DvarVectorDomainToString(size_t componentCount, const dvar_t &dvar)
{
    // This should be FLT_MIN but this is how it's implemented in the Dvar_VectorDomainToString function from the games
    const float minFloat = -FLT_MAX;
    const float maxFloat = FLT_MAX;

    float min = dvar.domain.vector.min;
    float max = dvar.domain.vector.max;

    if (min == minFloat && max == maxFloat)
        return Formatter::Format("Domain is any %iD vector.", componentCount);

    if (min == minFloat)
        return Formatter::Format("Domain is any %iD vector with components %g or smaller.", componentCount, max);

    if (max == maxFloat)
        return Formatter::Format("Domain is any %iD vector with components %g or bigger.", componentCount, min);

    return Formatter::Format("Domain is any %iD vector with components from %g to %g.", componentCount, min, max);
}

std::string StringUtils::ConvertWideCharToAscii(wchar_t wideChar)
{
    char ascii[3] = {};
    int converted = 0;
    errno_t error = wctomb_s(&converted, ascii, sizeof(ascii), wideChar);

    if (error != 0)
    {
        DebugPrint(
            "[Hayzen][Console]: Couldn't convert unicode character to multibyte array: %d.\n"
            "                       error: %d\n"
            "                       bytes: %02hhX %02hhX",
            error,
            reinterpret_cast<uint8_t *>(&wideChar)[0],
            reinterpret_cast<uint8_t *>(&wideChar)[1]
        );
        return "";
    }

    if (converted == -1)
    {
        DebugPrint(
            "[Hayzen][Console]: Couldn't convert wchar_t to multibyte array: %02hhX %02hhX",
            reinterpret_cast<uint8_t *>(&wideChar)[0],
            reinterpret_cast<uint8_t *>(&wideChar)[1]
        );
        return "";
    }

    return ascii;
}

// Case insensitive version of std::string::find
bool StringUtils::IContains(const std::string &haystack, const std::string &needle)
{
    auto it = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end(), [](unsigned char ch1, unsigned char ch2) {
        return std::tolower(ch1) == std::tolower(ch2);
    });

    return it != haystack.end();
}

// Case insensitive string equality
bool StringUtils::IEquals(const std::string &str1, const std::string &str2)
{
    return str1.size() == str2.size() &&
           std::equal(str1.begin(), str1.end(), str2.begin(), [](unsigned char a, unsigned char b) {
               return std::tolower(a) == std::tolower(b);
           });
}
