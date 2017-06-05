#include "util.h"

#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>

namespace requests {
    namespace util {
        std::string urlEncode(const std::string &str) {
            std::ostringstream escaped_str;
            escaped_str.fill('0');
            escaped_str << std::hex;

            for (auto i = str.cbegin(), n = str.cend(); i != n; i++) {
                std::string::value_type ch = (*i);

                if (isalnum(ch) || ch == '-' || ch == '.' || ch == '_' || ch == '~')
                    escaped_str << ch;
                else {
                    escaped_str << '%' << std::setw(2) << std::int32_t((unsigned char)ch);
                }
            }
            return escaped_str.str();
        }
    }
}