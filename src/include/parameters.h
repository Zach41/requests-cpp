#ifndef REQUESTS_PARAMETERTS_H
#define REQUESTS_PARAMETERTS_H

#include <initializer_list>
#include <string>
#include <utility>

namespace requests {
    struct Parameter {
        template <typename KeyType, typename ValueType>
        Parameter(KeyType &&key, ValueType &&value)
            : key(std::forward<KeyType>(key)),
                value(std::forward<ValueType>(value)) {}

        std::string key;
        std::string value;
    };

    struct Parameters {
        Parameters() = default;
        Parameters(const std::initializer_list<Parameter> &il);

        void addParamter(const Parameter &param);

        std::string content;
    };
}  // namespace requests
#endif