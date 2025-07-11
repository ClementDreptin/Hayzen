#pragma once

namespace Json
{

HRESULT ReadUpToKey(HJSONREADER reader, const std::string &key);

HRESULT ReadTokenType(HJSONREADER reader, JSONTOKENTYPE expectedType);

}
