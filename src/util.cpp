#include "util.h"

#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <functional>

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

        size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
            data -> append((char*)ptr, size * nmemb);
            return size * nmemb;
        }

        std::vector<std::string> split(const std::string& to_split, char delimiter) {
            std::vector<std::string> tokens;
            std::istringstream stream(to_split);

            std::string line;
            while (std::getline(stream, line, delimiter)) {
                tokens.push_back(line);
            }
            return tokens;
        }

        Header parserHeader(const std::string& headers) {
            Header header;
            std::vector<std::string> lines;
            std::istringstream stream(headers);
            {
                std::string line;
                while (std::getline(stream, line, '\n'))
                    lines.push_back(line);
            }
            for (auto &line: lines) {
                if (line.substr(0, 5) == "HTTP/") {
                    header.clear();
                }
                if (line.length() > 0) {
                    auto found = line.find(":");
                    if (found != std::string::npos) {
                        auto value = line.substr(found+1);
                        // trim the value
                        // value.erase(0, value.find_first_not_of("\t "));
                        // value.resize(std::min(value.size(), value.find_last_not_of("\t\n\r ") + 1));
                        trim_str(value);
                        header[line.substr(0, found)] = value;
                    }
                }
            }
            return header;
        }

        void trim_str(std::string &str) {
            auto check_fn = [](char ch) -> bool { return std::isspace(ch); };
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::function<bool(char)>(check_fn))));
            str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::function<bool(char)>(check_fn))).base(), str.end());
        }
    }
}