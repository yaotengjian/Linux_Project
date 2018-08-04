#include "base_json.h"

void val_to_string(Json::Value &v, std::string &s)
{
#ifdef _FAST_
	Json::FastWriter w;
#else
	Json::StyledWriter w;
#endif
	s = w.write(v);
}

void string_to_val(std::string &s, Json::Value &v)
{
	Json::Reader r;
	r.parse(s, v, false);
}
