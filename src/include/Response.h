#ifndef REQUESTS_RESPONSE_H
#define REQUESTS_RESPONSE_H

#include "types.h"
#include "cookies.h"
#include "error.h"

#include <cstdint>
#include <string>
#include <utility>

namespace requests {
    class Response {
    public:
        Response() = default;
        template <typename TextType, typename HeaderType, typename UrlType,
            typename CookiesType, typename ErrorType>
        Response(const std::int32_t status_code, TextType&& p_text, HeaderType&& header,
                 const double elapsed, UrlType&& url, CookiesType&& cookies, ErrorType&& error)
                 : status_code(status_code), text(std::forward<TextType>(p_text)),
                   header(std::forward<HeaderType>(header)), elapsed(elapsed),
                   url(std::forward<UrlType>(url)), cookies(std::forward<CookiesType>(cookies)),
                   error(std::forward<ErrorType>(error)) {}
        std::int32_t status_code;
        double elapsed;
        Header header;
        Url url;
        Cookies cookies;
        Error error;
        Body text;
    };
}
#endif // REQUESTS_RESPONSE_H end here