#pragma once

#include <string>

namespace util::encoding {

std::string hmac_sha256(const std::string& secret, const std::string& message);

std::string util_string_to_hex(const unsigned char* data, std::size_t len);
}
