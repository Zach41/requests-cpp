#ifndef REQUESTS_TYPES_H
#define REQUESTS_TYPES_H

#include <string>
#include <cctype>
#include <map>
#include <curl/curl.h>

namespace requests {
    using Url = std::string;

    struct CaseInsensitiveCompare {
        bool operator()(const std::string &str1, const std::string &str2) const noexcept {
            return std::lexicographical_compare(
                str1.begin(), str1.end(), str2.begin(), str2.end(),
                [](unsigned char ch1, unsigned char ch2) { return std::tolower(ch1) < std::tolower(ch2); }
            );
        }
    };

    using Header = std::map<std::string, std::string, CaseInsensitiveCompare>;
    using Body = std::string;
    // using Response = std::string;

    struct CurlHolder {
        CURL *handle;
        struct curl_slist *chunk;
        struct curl_httppost *formpost;
        char error[CURL_ERROR_SIZE];
    };
}
#endif // REQUESTS_TYPES_H end here