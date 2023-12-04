#pragma once
#include <iostream>
#include "request.h"
#include <vector>
#include <map>
#include <iostream>
#include <streambuf>
#include <string>
#include <fstream>


class response
{
public:
	response(std::string request) 
		:received_request(request) {



		//rols
		if (false) {}

		if (open_file("./html/" + received_request.path)) {

			respone_code = 200;
			headers["Content-Type"] = getContentType();
			headers["Connection"] = "keep-alive";

			body = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			headers["Content-Length"] = std::to_string(body.size());


			//std::cout << 132 <<std::endl;
			//std::cout << headers["Content-Length"] <<std::endl;
			//std::cout << headers["Content-Type"] <<std::endl;
			//std::cout << body[0] <<std::endl;


		}
		else {

			std::cout << received_request.path;

			//open_file("./html/404.html");
			//respone_code = 301;
			respone_code = 404;
			//headers["Location"] = "404.html";

			headers["Connection"] = "close";
			//body = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			//std::cout << 700 << std::endl;
			//std::cout << headers["Content-Length"] << std::endl;
			//std::cout << headers["Content-Type"] << std::endl;
			//std::cout << body[0] << std::endl;
		}
	
	};

	request received_request;

	int respone_code;
	std::map<std::string, std::string> headers;
	std::vector<char> body;


	std::ifstream file;

	bool open_file(std::string PATH);
	int get_file_size();

	std::string getContentType();
	std::string toString() const;
	void make_404();
};

