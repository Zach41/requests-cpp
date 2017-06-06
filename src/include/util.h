#ifndef REQUESTS_UTIL_H
#define REQUESTS_UTIL_H

#include "types.h"

#include <string>
#include <vector>

namespace requests {
    namespace util {
        std::string urlEncode(const std::string &str);

        size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

        std::vector<std::string> split(const std::string& to_split, char delimiter);

        Header parserHeader(const std::string& headers);

        void trim_str(std::string &str);      
    }
}  // namespace requests
#endif  // UTIL_H end here