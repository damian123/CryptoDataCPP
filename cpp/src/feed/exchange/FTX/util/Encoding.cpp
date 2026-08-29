#include <Encoding.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <array>
#include <limits>
#include <stdexcept>

namespace util::encoding 
{

std::string hmac_sha256(const std::string& secret, const std::string& message)
{
	if (secret.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
		throw std::length_error("HMAC secret is too large");
	}
	std::array<unsigned char, EVP_MAX_MD_SIZE> digest{};
	unsigned int digest_length = 0;
	if (HMAC(EVP_sha256(), secret.data(), static_cast<int>(secret.size()),
	         reinterpret_cast<const unsigned char*>(message.data()), message.size(),
	         digest.data(), &digest_length) == nullptr) {
		throw std::runtime_error("HMAC-SHA256 failed");
	}
	return {reinterpret_cast<const char*>(digest.data()), digest_length};
}

namespace {
constexpr char hexmap[] = {'0',
                           '1',
                           '2',
                           '3',
                           '4',
                           '5',
                           '6',
                           '7',
                           '8',
                           '9',
                           'a',
                           'b',
                           'c',
                           'd',
                           'e',
                           'f'};
}

std::string util_string_to_hex(const unsigned char* data, std::size_t len)
{
    std::string s(len * 2, ' ');
    for (std::size_t i = 0; i < len; ++i) {
        s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}
}
