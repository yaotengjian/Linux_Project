#ifndef _DATA_H_
#define _DATA_H_

#include <iostream>
#include <string>
#include "base_json.h"

class data{
	public:
		void serialize(std::string &sendString);
		void deserialize(std::string &recvString);
	public:
		std::string nickName;
		std::string school;
		std::string msg;
		std::string cmd;
};

#endif
