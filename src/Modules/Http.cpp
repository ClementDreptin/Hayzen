#include "pch.h"
#include "Modules/Http.h"

namespace Http
{

HRESULT SendRequest(Socket &socket, const std::string &domain, const std::string &path)
{
    std::string request = XexUtils::Formatter::Format(
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Hayzen AutoUpdater\r\n"
        "Connection: close\r\n\r\n",
        path.c_str(),
        domain.c_str()
    );

    int bytesSent = socket.Send(request.c_str(), request.size());
    if (bytesSent < static_cast<int>(request.size()))
    {
        DebugPrint(
            "[Hayzen][AutoUpdater]: Error: Not all bytes could be sent, "
            "expected to send %d but only sent %d.",
            request.size(),
            bytesSent
        );
        return E_FAIL;
    }

    return S_OK;
}

std::string GetResponse(Socket &socket)
{
    std::stringstream responseStream;
    char buffer[2048] = {};

    for (;;)
    {
        int bytesRead = socket.Receive(buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0)
            break;

        buffer[bytesRead] = '\0';
        responseStream << buffer;
    }

    return responseStream.str();
}

std::unordered_map<std::string, std::string> GetResponseHeaders(const std::string &response)
{
    std::unordered_map<std::string, std::string> headers;

    size_t headersEndPos = response.find("\r\n\r\n");
    if (headersEndPos == std::string::npos)
    {
        DebugPrint("[Hayzen][Http]: Error: Couldn't find the headers' end.");
        return headers;
    }

    const std::string newLine = "\r\n";

    std::string headersSection = response.substr(0, headersEndPos);
    size_t start = 0;
    size_t end = headersSection.find(newLine);

    // Skip the status line
    start = end + newLine.size();
    end = headersSection.find(newLine, start);

    while (end != std::string::npos)
    {
        std::string headerLine = headersSection.substr(start, end - start);
        size_t colonPos = headerLine.find(":");

        if (colonPos != std::string::npos)
        {
            std::string name = headerLine.substr(0, colonPos);
            std::string value = headerLine.substr(colonPos + 2); // Skip ": "
            headers[name] = value;
        }

        start = end + newLine.size();
        end = headersSection.find(newLine, start);
    }

    return headers;
}

std::string GetResponseBody(const std::string &response)
{
    const std::string headersEnd = "\r\n\r\n";

    size_t headersEndPos = response.find(headersEnd);
    if (headersEndPos == std::string::npos)
    {
        DebugPrint("[Hayzen][Http]: Error: Couldn't separate headers from body of HTTP response.");
        return "";
    }

    return response.substr(headersEndPos + headersEnd.size());
}

}
