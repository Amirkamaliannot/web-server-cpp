#pragma once
#include <iostream>
#include "request.h"


class response
{
public:
	response(std::string request) 
		:received_request(request) {};

	request received_request;
};

