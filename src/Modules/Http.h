#pragma once

namespace Http
{

HRESULT SendRequest(Socket &socket, const std::string &domain, const std::string &path);

std::string GetResponse(Socket &socket);

std::unordered_map<std::string, std::string> GetResponseHeaders(const std::string &response);

std::string GetResponseBody(const std::string &response);

}
