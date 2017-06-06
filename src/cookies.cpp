#include "cookies.h"
#include "util.h"

#include <sstream>

namespace requests {
    std::string& Cookies::operator[](const std::string &key) {
        return map_[key];
    }

    std::string Cookies::GetEncoded() const {
        std::ostringstream stream;
        for (const auto &item: map_) {
            stream << requests::util::urlEncode(item.first) << "=";
            if (!item.second.empty() && item.second.front() == '"' && item.second.back() == '"') {
                stream << item.second;               
            } else {
                stream << requests::util::urlEncode(item.second);
            }
            stream << "; ";
        }
        return stream.str();
    }
}