#include "pch.h"
#include "Modules/Http.h"

namespace Http
{

static std::vector<char> s_LeftoverData;
static const std::string s_ProtocolDelimiter = "://";
static const std::string s_NewLineDelimiter = "\r\n";
static const std::string s_HeadersDelimiter = "\r\n\r\n";

static std::string Trim(const std::string &str)
{
    // Remove whitespaces at the beginning of the string
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return str;

    // Remove whitespaces at the end of the string
    size_t last = str.find_last_not_of(" \t\n\r");

    return str.substr(first, (last - first + 1));
}

HRESULT ParseUrl(const std::string &url, Url &result)
{
    // This parser is anything but complete, it just separates the hostname from the path
    // because the auto updater only needs that

    // Find the end of the protocol
    size_t protocolEndPos = url.find(s_ProtocolDelimiter);
    if (protocolEndPos == std::string::npos)
    {
        DebugPrint("[Hayzen][Http]: Error: \"%s\" is not a valid URL.", url.c_str());
        return E_FAIL;
    }

    // Extract the hostname
    size_t hostnameStartPos = protocolEndPos + s_ProtocolDelimiter.size();
    size_t pathStartPos = url.find('/', hostnameStartPos);
    result.Hostname =
        pathStartPos != std::string::npos
            ? url.substr(hostnameStartPos, pathStartPos - hostnameStartPos)
            : url.substr(hostnameStartPos);

    // Extract the path
    result.Path = pathStartPos != std::string::npos ? url.substr(pathStartPos) : "/";

    return S_OK;
}

HRESULT SendRequest(Socket &socket, const std::string &domain, const std::string &path)
{
    // Build the request
    std::string request = XexUtils::Formatter::Format(
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Hayzen AutoUpdater\r\n"
        "Connection: close\r\n\r\n",
        path.c_str(),
        domain.c_str()
    );

    // Send it through the socket
    int bytesSent = socket.Send(request.c_str(), request.size());
    if (bytesSent < static_cast<int>(request.size()))
    {
        DebugPrint(
            "[Hayzen][Http]: Error: Not all bytes could be sent, expected to send %d but only sent %d.",
            request.size(),
            bytesSent
        );
        return E_FAIL;
    }

    return S_OK;
}

uint32_t ReadStatus(Socket &socket)
{
    uint32_t status = 0;
    char buffer[64] = {};

    // Clear any previous leftover data
    s_LeftoverData.clear();

    // Read from the socket
    int bytesRead = socket.Receive(buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0)
    {
        DebugPrint("[Hayzen][Http]: Error: Couldn't read status line.");
        return status;
    }

    std::string data(buffer, bytesRead);

    // If the new line delimiter hasn't been read, just push what was read into the
    // leftover data and stop
    size_t newLinePos = data.find(s_NewLineDelimiter);
    if (newLinePos == std::string::npos)
    {
        s_LeftoverData.insert(s_LeftoverData.end(), data.begin(), data.end());
        return status;
    }

    // Separate the status line from the rest and push the rest into the leftover data
    std::string statusLine = data.substr(0, newLinePos);
    s_LeftoverData.insert(
        s_LeftoverData.end(),
        data.begin() + newLinePos + s_NewLineDelimiter.size(),
        data.end()
    );

    // Extract status code from status line
    if (sscanf_s(statusLine.c_str(), "HTTP/1.1 %d", &status) != 1)
    {
        DebugPrint(
            "[Hayzen][Http]: Error: Couldn't extract status code from \"%s\"",
            statusLine.c_str()
        );
        return 0;
    }

    return status;
}

std::unordered_map<std::string, std::string> ReadHeaders(Socket &socket)
{
    std::unordered_map<std::string, std::string> headers;
    std::stringstream headerStream;
    char buffer[2048] = {};
    bool headersComplete = false;

    // If any leftover data is present from previous reads, use them before reading from
    // the socket
    if (!s_LeftoverData.empty())
    {
        headerStream.write(s_LeftoverData.data(), s_LeftoverData.size());
        s_LeftoverData.clear();
    }

    // Read until all headers are read
    while (!headersComplete)
    {
        int bytesRead = socket.Receive(buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0)
            break;

        std::string chunk(buffer, bytesRead);

        // If the end of the headers hasn't been reached yet, just push the chunk into the
        // global stream and continue
        size_t headersEndPos = chunk.find(s_HeadersDelimiter);
        if (headersEndPos == std::string::npos)
        {
            headerStream << chunk;
            continue;
        }

        // Push the chunk into the global stream up to the end of the headers
        headerStream << chunk.substr(0, headersEndPos);

        // If the chunk contains data after the end of the headers, push it into the global
        // leftover buffer
        s_LeftoverData.insert(
            s_LeftoverData.end(),
            chunk.begin() + headersEndPos + s_HeadersDelimiter.size(),
            chunk.end()
        );

        headersComplete = true;
    }

    // Read the headers line by line and create the map
    std::string headerLine;
    while (std::getline(headerStream, headerLine, '\n'))
    {
        headerLine = Trim(headerLine);
        size_t colonPos = headerLine.find(':');

        // If the header doesn't contain a ":", then it's malformed, so just skip it
        if (colonPos == std::string::npos)
            continue;

        std::string key = Trim(headerLine.substr(0, colonPos));
        std::string value = Trim(headerLine.substr(colonPos + 1));
        headers[key] = value;
    }

    return headers;
}

std::string ReadBody(Socket &socket)
{
    std::stringstream bodyStream;
    char buffer[2048] = {};

    // If any leftover data is present from previous reads, use them before reading from
    // the socket
    if (!s_LeftoverData.empty())
    {
        bodyStream.write(s_LeftoverData.data(), s_LeftoverData.size());
        s_LeftoverData.clear();
    }

    // Flush the socket
    for (;;)
    {
        int bytesRead = socket.Receive(buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0)
            break;

        buffer[bytesRead] = '\0';
        bodyStream << buffer;
    }

    return bodyStream.str();
}

std::vector<char> FlushInternalBuffer()
{
    std::vector<char> leftoverDataCopy = s_LeftoverData;

    s_LeftoverData.clear();

    return leftoverDataCopy;
}

}
