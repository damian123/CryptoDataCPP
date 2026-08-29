#include "util/HTTP.h"
#include "util/Encoding.h"
#include "util/TimeUtil.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/host_name_verification.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <chrono>
#include <stdexcept>

namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;

namespace util {

void HTTPSession::configure(std::string _uri,
                            std::string _api_key,
                            std::string _api_secret,
                            std::string _subaccount_name)
{
	if (_uri.empty()) {
		throw std::invalid_argument("HTTPS host is required");
	}
	if (_api_key.empty() != _api_secret.empty()) {
		throw std::invalid_argument("API key and secret must be provided together");
	}
	uri = std::move(_uri);
	api_key = std::move(_api_key);
	api_secret = std::move(_api_secret);
	subaccount_name = std::move(_subaccount_name);
}

http::response<http::string_body> HTTPSession::get(const std::string target)
{
    std::string endpoint = "/api/" + target;
    http::request<http::string_body> req{http::verb::get, endpoint, 11};
    return request(req);
}

http::response<http::string_body> HTTPSession::post(const std::string target,
                                                    const std::string payload)
{
    std::string endpoint = "/api/" + target;
    http::request<http::string_body> req{http::verb::post, endpoint, 11};
    req.body() = payload;
    req.prepare_payload();
    return request(req);
}

http::response<http::string_body> HTTPSession::delete_(const std::string target)
{
    std::string endpoint = "/api/" + target;
    http::request<http::string_body> req{http::verb::delete_, endpoint, 11};
    return request(req);
}

http::response<http::string_body> HTTPSession::request(
  http::request<http::string_body> req)
{
    req.set(http::field::host, uri.c_str());
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	    ssl::context ctx{ssl::context::tls_client};
	    ctx.set_default_verify_paths();

	    tcp::resolver resolver{ioc};
	    beast::ssl_stream<beast::tcp_stream> stream{ioc, ctx};
	    stream.set_verify_mode(ssl::verify_peer);
	    stream.set_verify_callback(ssl::host_name_verification(uri));

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), uri.c_str())) {
        boost::system::error_code ec{static_cast<int>(::ERR_get_error()),
                                     net::error::get_ssl_category()};
        throw boost::system::system_error{ec};
    }

	    auto const results = resolver.resolve(uri, "443");
	    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(10));
	    beast::get_lowest_layer(stream).connect(results);
	    stream.handshake(ssl::stream_base::client);

	    if (!api_key.empty()) {
	        authenticate(req);
	    }

    if (req.method() == http::verb::post) {
        req.set(http::field::content_type, "application/json");
    }

    http::write(stream, req);
    boost::beast::flat_buffer buffer;
	    http::response<http::string_body> response;
	    http::read(stream, buffer, response);
	    if (response.result_int() < 200 || response.result_int() >= 300) {
	        throw std::runtime_error("HTTPS request failed with status " +
	                                 std::to_string(response.result_int()));
	    }

    boost::system::error_code ec;
    stream.shutdown(ec);
	    if (ec == boost::asio::error::eof || ec == ssl::error::stream_truncated) {
        // Rationale:
        // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
        ec.assign(0, ec.category());
    }

    return response;
}

void HTTPSession::authenticate(http::request<http::string_body>& req)
{

    std::string method(req.method_string());
    std::string path(req.target());
    std::string body(req.body());

	    const auto ts = get_ms_timestamp(current_time()).count();
    std::string data = std::to_string(ts) + method + path;
    if (!body.empty()) {
        data += body;
    }
	    std::string hmacced = encoding::hmac_sha256(api_secret, data);
	    std::string sign =
	      encoding::util_string_to_hex(
	        reinterpret_cast<const unsigned char*>(hmacced.data()), hmacced.size());

    req.set("FTX-KEY", api_key);
    req.set("FTX-TS", std::to_string(ts));
    req.set("FTX-SIGN", sign);
    if (!subaccount_name.empty()) {
        req.set("FTX-SUBACCOUNT", subaccount_name);
    }
}
}
