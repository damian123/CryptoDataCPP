#include <util/Secret.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
    struct BIOFreeAll { void operator()(BIO* p) { BIO_free_all(p); } };
}

namespace util {
    std::string Base64Encode(const std::vector<unsigned char>& binary)
    {
        std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
        BIO* sink = BIO_new(BIO_s_mem());
        if (!b64 || sink == nullptr) {
            if (sink != nullptr) BIO_free(sink);
            throw std::runtime_error("could not create Base64 encoder");
        }
        BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
        if (BIO_push(b64.get(), sink) == nullptr) {
            BIO_free(sink);
            throw std::runtime_error("could not create Base64 encoder");
        }
        if (BIO_write(b64.get(), binary.data(), static_cast<int>(binary.size())) < 0 ||
            BIO_flush(b64.get()) != 1) {
            throw std::runtime_error("Base64 encoding failed");
        }
        const char* encoded;
        const long len = BIO_get_mem_data(sink, &encoded);
        return std::string(encoded, len);
    }

    // Assumes no newlines or extra characters in encoded string
    std::vector<unsigned char> Base64Decode(const char* encoded)
    {
        if (encoded == nullptr) throw std::invalid_argument("Base64 input is required");
        std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
        BIO* source = BIO_new_mem_buf(encoded, -1); // read-only source
        if (!b64 || source == nullptr) {
            if (source != nullptr) BIO_free(source);
            throw std::runtime_error("could not create Base64 decoder");
        }
        BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
        if (BIO_push(b64.get(), source) == nullptr) {
            BIO_free(source);
            throw std::runtime_error("could not create Base64 decoder");
        }
        const int maxlen = strlen(encoded) / 4 * 3 + 1;
        std::vector<unsigned char> decoded(maxlen);
        const int len = BIO_read(b64.get(), decoded.data(), maxlen);
        if (len < 0) {
            throw std::runtime_error("invalid Base64 input");
        }
        decoded.resize(static_cast<std::size_t>(len));
        return decoded;
    }
} // ns cryptodata
