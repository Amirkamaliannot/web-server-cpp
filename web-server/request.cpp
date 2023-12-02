#include "request.h"

request::request(const std::string& request_str)
{
    std::istringstream request_stream(request_str);
    std::string line;
    std::getline(request_stream, line); // Read the request line

    std::istringstream line_stream(line);

    std::string method_str;
    line_stream >> method_str; // Extract the method
    method_type = StringToMethod(method_str);

    line_stream >> path; // Extract the path

    std::string http_version_str;
    line_stream >> http_version_str; // Extract the HTTP version part
    http_version = http_version_str.substr(http_version_str.find('/') + 1);

    // Parse headers
    while (std::getline(request_stream, line) && line != "\r") {
        auto colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string header_name = line.substr(0, colon_pos);
            std::string header_value = line.substr(colon_pos + 2, line.size() - colon_pos - 3); // Also remove '\r'
            headers[header_name] = header_value;
        }
    }

    // Parse body
    while (std::getline(request_stream, line)) {
        if (!body.empty())
            body += "\n"; // Add newline character if not first line of body
        body += line;
    }
}

std::string request::ToString() const
{
    {
        std::ostringstream request_stream;

        // Start the request line
        request_stream << MethodToString(method_type) << " " << path << " HTTP/" << http_version << "\r\n";

        // Add headers
        for (const auto& header : headers) {
            request_stream << header.first << ": " << header.second << "\r\n";
        }

        request_stream << "\r\n"; // End of headers

        // Add body if it is not empty
        if (!body.empty()) {
            request_stream << body;
        }

        // Convert the string stream into a string
        return request_stream.str();
    }
}

std::string request::MethodToString(Method method) const
{
    switch (method) {
    case Method::GET:    return "GET";
    case Method::POST:   return "POST";
    case Method::PUT:    return "PUT";
    case Method::PATCH:  return "PATCH";
    //case Method::DELETE: return "DELETE";
    default:             return "UNKNOWN";
    }
}

Method request::StringToMethod(const std::string& method_str) const
{
    if (method_str == "GET") return Method::GET;
    if (method_str == "POST") return Method::POST;
    if (method_str == "PUT") return Method::PUT;
    if (method_str == "PATCH") return Method::PATCH;
    //if (method_str == "DELETE") return Method::DELETE;
    return Method::UNKNOWN;
}
