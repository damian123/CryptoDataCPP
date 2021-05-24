#pragma once

#include <string>

namespace util::encoding {

std::string hmac(const std::string& secret,
    std::string msg,
    std::size_t signed_len);

std::string util_string_to_hex(unsigned char* data, std::size_t len);
}
