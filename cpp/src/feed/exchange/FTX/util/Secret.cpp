#include <util/Secret.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace {
    struct BIOFreeAll { void operator()(BIO* p) { BIO_free_all(p); } };
}

namespace util {
    std::string Base64Encode(const std::vector<unsigned char>& binary)
    {
        std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
        BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
        BIO* sink = BIO_new(BIO_s_mem());
        BIO_push(b64.get(), sink);
        BIO_write(b64.get(), binary.data(), binary.size());
        BIO_flush(b64.get());
        const char* encoded;
        const long len = BIO_get_mem_data(sink, &encoded);
        return std::string(encoded, len);
    }

    // Assumes no newlines or extra characters in encoded string
    std::vector<unsigned char> Base64Decode(const char* encoded)
    {
        std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
        BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
        BIO* source = BIO_new_mem_buf(encoded, -1); // read-only source
        BIO_push(b64.get(), source);
        const int maxlen = strlen(encoded) / 4 * 3 + 1;
        std::vector<unsigned char> decoded(maxlen);
        const int len = BIO_read(b64.get(), decoded.data(), maxlen);
        decoded.resize(len);
        return decoded;
    }
} // ns cryptodata
