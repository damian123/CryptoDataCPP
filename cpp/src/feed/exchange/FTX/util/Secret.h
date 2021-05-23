#pragma once

#include <string>
#include <vector>

namespace util {
	
    std::string Base64Encode(const std::vector<unsigned char>& binary);    

    // Assumes no newlines or extra characters in encoded string
    std::vector<unsigned char> Base64Decode(const char* encoded);    
	
} // ns binapi

