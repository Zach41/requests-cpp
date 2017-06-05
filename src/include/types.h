#ifndef REQUESTS_TYPES_H
#define REQUESTS_TYPES_H

#include <string>
#include <curl/curl.h>

namespace requests {
    using Url = std::string;
    using Response = std::string;

    struct CurlHolder {
        CURL *handle;
        struct curl_slist *chunk;
        struct curl_httppost *formpost;
        char error[CURL_ERROR_SIZE];
    };
}
#endif // REQUESTS_TYPES_H end here