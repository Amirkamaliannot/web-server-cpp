#pragma once
#include <string>
#include <map>
#include <sstream>

enum class Method {
    POST,
    GET,
    PUT,
    PATCH,
    //DELETE,
    UNKNOWN
};

class request {
public:
    Method method_type;
    std::string path;
    std::string http_version;
    std::map<std::string, std::string> headers;
    std::string body;

    request()
        : method_type(Method::GET),
        http_version("1.1"),
        path("/"),
        headers({ {"Host", "example.com"},
                  {"Connection", "close"} }) {
    }

    // Parser constructor
    request(const std::string& request_str);
    std::string ToString() const;

private:
    std::string MethodToString(Method method) const;
    Method StringToMethod(const std::string& method_str) const;
};