#pragma once

namespace Http
{

std::unordered_map<std::string, std::string> GetResponseHeaders(const std::string &response);

std::string GetResponseBody(const std::string &response);

}
