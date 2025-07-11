#include "pch.h"
#include "Modules/Json.h"

namespace Json
{

HRESULT ReadUpToKey(HJSONREADER reader, const std::string &key)
{
    HRESULT hr = S_OK;

    char buffer[128] = {};
    JSONTOKENTYPE tokenType = Json_NotStarted;
    DWORD unused = 0;

    for (;;)
    {
        // Parse the body
        hr = XJSONReadToken(reader, &tokenType, &unused, &unused);
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][Json]: Error: Couldn't parse JSON body: %X.", hr);
            return hr;
        }

        // End of body reached
        if (hr == S_FALSE)
        {
            DebugPrint("[Hayzen][Json]: Error: Reached end of JSON without finding \"%s\".", key.c_str());
            return E_FAIL;
        }

        if (tokenType != Json_FieldName)
            continue;

        // Get the key name
        hr = XJSONGetTokenValue(reader, buffer, sizeof(buffer));
        if (FAILED(hr))
        {
            DebugPrint("[Hayzen][Json]: Error: Couldn't read key from JSON: %X.", hr);
            return hr;
        }

        // If the current key is the key we're looking for, stop
        if (strncmp(buffer, key.c_str(), sizeof(buffer)) == 0)
            break;
    }

    return hr;
}

HRESULT ReadTokenType(HJSONREADER reader, JSONTOKENTYPE expectedType)
{
    HRESULT hr = S_OK;

    JSONTOKENTYPE actualType = Json_NotStarted;
    DWORD unused = 0;

    // Parse the value at the version key
    hr = XJSONReadToken(reader, &actualType, &unused, &unused);
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][Json]: Error: Couldn't read token from JSON: %X.", hr);
        return hr;
    }

    // If the value of the version key is not a string, something is wrong in the response
    if (actualType != expectedType)
    {
        DebugPrint(
            "[Hayzen][Json]: Error: Unexpected token type found, expected %d and got %d.",
            expectedType,
            actualType
        );
        return E_FAIL;
    }

    return hr;
}

}
