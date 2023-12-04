#include "response.h"


bool response::open_file(std::string PATH)
{
    file.open(PATH, std::ios::binary);
    return file.is_open();
}

int response::get_file_size()
{
    return file.tellg();
}


std::string response::getContentType()
{

    if (received_request.path.find(".html") != std::string::npos) {
        return "text/html";
    }
    else if (received_request.path.find(".css") != std::string::npos) {
        return "text/css";
    }
    else if (received_request.path.find(".js") != std::string::npos) {
        return "text/javascript";
    }    
    else if (received_request.path.find(".xml") != std::string::npos) {
        return "text/xml";
    }
    else if (received_request.path.find(".png") != std::string::npos) {
        return "image/png";
    }
    else if (received_request.path.find(".jpg") != std::string::npos) {
        return "image/jpg";
    }
    else if (received_request.path.find(".jpeg") != std::string::npos) {
        return "image/jpeg";
    }    
    else if (received_request.path.find(".woff2") != std::string::npos) {
        return "font/woff2";
    }
    return "text/html";
}

std::string response::toString() const
{
    {
        std::ostringstream request_stream;

        // Start the request line
        request_stream << "HTTP/" << received_request.http_version << " " << respone_code;

        if (respone_code == 200) {
            request_stream << " OK\r\n";
        }
        else if(respone_code == 404) {
            request_stream << " Not Found\r\n";

        }        
        else if(respone_code == 301) {
            request_stream << " Moved Permanently\r\n";

        }

        // Add headers
        for (const auto& header : headers) {
            request_stream << header.first << ": " << header.second << "\r\n";
        }

        request_stream << "\r\n"; // End of headers

        // Add body if it is not empty
        if (body.size()) {
            request_stream.write(&body[0], body.size());
        }

        //std::cout << body.data() <<std::endl;

        // Convert the string stream into a string
        return request_stream.str();
    }
}


void response::make_404() {

    open_file("./html/404.html");
    
}