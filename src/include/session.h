#ifndef REQUESTS_SESSION_H
#define REQUESTS_SESSION_H

#include "types.h"
#include "parameters.h"
#include "response.h"

#include <memory>
namespace requests {
    class Session {
    public:
        Session();
        ~Session();

        void setUrl(const Url &url);
        void setParameters(const Parameters& params);
        void setParameters(Parameters&& params);

        Response Get();

    private:
        class Impl;
        std::unique_ptr<Impl> priImpl_;
    };
}
#endif  // SESSION_H end here