#pragma once
#include "ztype.h"
#include <math.h>
#include <assert.h>
#include <string>
#include <ctime>
#include "util_time.h"

//����UUID
extern int64_t GenId();

//32��md5
extern std::string make_md5_32(const std::string& str);
extern std::string make_md5_32(const char* str, size_t len);
//16��md5
extern std::string make_md5_16(const std::string& str);
extern std::string make_md5_16(const char* str, size_t len);

