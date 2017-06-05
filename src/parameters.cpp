#include "parameters.h"
#include "util.h"

#include <string>

namespace requests {
    Parameters::Parameters(const std::initializer_list<Parameter> &il) {
        for (auto &param: il) {
            addParamter(param);
        }
    }

    void Parameters::addParamter(const Parameter &param) {
        if (!content.empty())
            content += "&";
        auto escapedKey = requests::util::urlEncode(param.key);
        if (param.value.empty())
            content += escapedKey;
        else {
            auto escapedValue = requests::util::urlEncode(param.value);
            content += escapedKey + "=" + escapedValue;
        }
    }
}