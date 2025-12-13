#include "headers/utils.hpp"

void error(const char* error_msg, const char* format, ...){
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    throw error_msg;
}