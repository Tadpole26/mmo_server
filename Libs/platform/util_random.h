#pragma once
#include "ztype.h"
#include <math.h>
#include <assert.h>
#include <string>
#include <ctime>
#include "util_time.h"

//Éú³ÉUUID
extern int64_t GenId();

//32¦Ëmd5
extern std::string make_md5_32(const std::string& str);
extern std::string make_md5_32(const char* str, size_t len);
//16¦Ëmd5
extern std::string make_md5_16(const std::string& str);
extern std::string make_md5_16(const char* str, size_t len);

