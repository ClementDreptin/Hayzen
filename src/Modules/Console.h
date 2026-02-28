#pragma once

#include "Games/Common/Structs.h"

template<typename TDvar>
class Console
{
public:
    typedef void (*CBUF_ADDTEXT)(int localClientNum, const char *text);
    typedef void (*DVAR_FOREACH)(void (*callback)(const TDvar *dvar, void *data), void *userData);

    struct Props
    {
        Props(CBUF_ADDTEXT Cbuf_AddText, DVAR_FOREACH Dvar_ForEach);
        CBUF_ADDTEXT Cbuf_AddText;
        DVAR_FOREACH Dvar_ForEach;
    };

    Console(const Props &props);

    void Update();

    void Render();

private:
    Props m_Props;

    bool m_Open;
    size_t m_CursorPosition;
    std::string m_Command;

    std::deque<std::string> m_History;
    size_t m_HistoryIndex;

    std::vector<const TDvar *> m_AutocompleteMatches;
    std::string m_AutocompleteQuery;
    size_t m_AutocompleteIndex;

    void Submit();

    void SetCommand(const std::string &command);

    bool AnySpaceAfterCommand();

    void AppendToHistory();

    void UpdateAutocompleteMatches();

    static void FindMatchingDvarsCallback(const TDvar *dvar, void *data);

    static void FindExactDvarCallback(const TDvar *dvar, void *data);

    void GenerateAutocompleteQuery();

    void CycleThroughAutocompleteMatches(bool shiftPressed);

    void ResetAutocompleteMatches();

    void DrawAutocompleteMatches();

    void DrawExactMatchExtendedDetails();

    std::string DvarValueToString(const TDvar &dvar, const DvarValue &value);

    std::string DvarDomainToString(const TDvar &dvar);

    std::string DvarVectorDomainToString(size_t componentCount, const TDvar &dvar);
};
