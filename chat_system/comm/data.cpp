#include "data.h"


void data::serialize(std::string &sendString)
{
	Json::Value root;
	root["nickName"] = nickName;
	root["school"] = school;
	root["msg"] = msg;
	root["cmd"] = cmd;

	val_to_string(root, sendString);
}

void data::deserialize(std::string &recvString)
{
	Json::Value root;
	string_to_val(recvString, root);
	nickName = root["nickName"].asString();//asInt
	school = root["school"].asString();
	msg = root["msg"].asString();
	cmd = root["cmd"].asString();
}





