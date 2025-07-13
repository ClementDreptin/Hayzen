#pragma once

namespace Http
{

struct Url
{
    std::string Hostname;
    std::string Path;
};

HRESULT ParseUrl(const std::string &url, Url &result);

HRESULT SendRequest(Socket &socket, const std::string &domain, const std::string &path);

uint32_t ReadStatus(Socket &socket);

std::unordered_map<std::string, std::string> ReadHeaders(Socket &socket);

std::string ReadBody(Socket &socket);

std::vector<char> FlushInternalBuffer();

}
