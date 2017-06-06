#ifndef REQUESTS_COOKIES_H
#define REQUESTS_COOKIES_H

#include <initializer_list>
#include <map>
#include <string>

namespace requests {
    class Cookies {
    public:
        Cookies() {}
        Cookies(const std::initializer_list<std::pair<const std::string, std::string>> &pairs)
            : map_(pairs) {}
        Cookies(const std::map<std::string, std::string> &map): map_(map) {}

        std::string& operator[](const std::string &key);
        std::string GetEncoded() const;
    private:
        std::map<std::string, std::string> map_;    
    };
}
#endif //REQUESTS_COOKIES_H
