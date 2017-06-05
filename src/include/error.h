#ifndef REQUESTS_ERROR_H
#define REQUESTS_ERROR_H

#include <string>
#include <cstdint>


namespace requests {
    enum class ErrorCode {
        OK = 0,
        CONNECTION_FAILURE,
        EMPTY_RESPONSE,
        HOST_RESOLUTION_FAILURE,
        INTERNAL_ERROR,
        INVALID_URL_FORMAT,
        NETWORK_RECEIVE_ERROR,
        NETWORK_SEND_FAILURE,
        OPERATION_TIMEDOUT,
        PROXY_RESOLUTION_FAILURE,
        SSL_CONNECT_ERROR,
        SSL_LOCAL_CERTIFICATE_ERROR,
        SSL_REMOTE_CERTIFICATE_ERROR,
        SSL_CACERT_ERROR,
        GENERIC_SSL_ERROR,
        UNSUPPORTED_PROTOCOL,
        UNKNOWN_ERROR = 1000,
    };
    class Error {   
    public:
        Error(): code(ErrorCode::OK) {}

        template <typename TextType>
        Error(const std::int32_t curl_code, TextType &&p_error_message)
            : code(getErrorCodeForCurlError(curl_code)), message(std::forward<TextType>(p_error_message)) {}
        ErrorCode code;
        std::string message;

    private:
        static ErrorCode getErrorCodeForCurlError(std::int32_t curl_code);
    };
}
#endif // REQUESTS_ERROR_H end here