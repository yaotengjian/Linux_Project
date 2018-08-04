#ifndef _BASE_JSON_H_
#define _BASE_JSON_H_

#include <iostream>
#include <string>
#include "json/json.h"

void val_to_string(Json::Value &v, std::string &s);
void string_to_val(std::string &s, Json::Value &v);

#endif

