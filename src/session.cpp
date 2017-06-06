#include <memory>
#include <string>
#include <functional>
#include <utility>
#include <curl/curl.h>

#include "session.h"
#include "types.h"
#include "util.h"

namespace requests {
    class Session::Impl {
    public:
        Impl();

        void setUrl(const Url& url);
        void setParameters(const Parameters& params);
        void setParameters(Parameters&& params);

        Response Get();

    private:
        std::unique_ptr<CurlHolder, std::function<void(CurlHolder*)>> curl_;
        Url url_;
        Parameters parameters_;

        Response makeRequest(CURL *curl);
        static void freeHolder(CurlHolder* holder);
        static CurlHolder *newHolder();
    };

    Session::Impl::Impl() {
        curl_ = std::unique_ptr<CurlHolder, std::function<void(CurlHolder*)>>(newHolder(), &Impl::freeHolder);
        auto curl = curl_ -> handle;
        if (curl_) {
            auto version_info = curl_version_info(CURLVERSION_NOW);
            auto version = std::string("curl/") + std::string{version_info -> version};
            curl_easy_setopt(curl, CURLOPT_USERAGENT, version.data());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_ -> error);
            curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        }
    }

    void Session::Impl::freeHolder(CurlHolder *holder) {
        curl_easy_cleanup(holder -> handle);
        curl_slist_free_all(holder -> chunk);
        curl_formfree(holder -> formpost);
        delete holder;
    }

    CurlHolder* Session::Impl::newHolder() {
        CurlHolder* holder = new CurlHolder();
        holder -> handle = curl_easy_init();
        holder -> chunk = NULL;
        holder -> formpost = NULL;
        return holder;
    }

    void Session::Impl::setUrl(const Url& url) {
        url_ = url;
    }

    void Session::Impl::setParameters(const Parameters& params) {
        parameters_ = params;
    }

    void Session::Impl::setParameters(Parameters&& params) {
        parameters_ = std::move(params);
    }

    Response Session::Impl::makeRequest(CURL *curl) {
        if (!parameters_.content.empty()) {
            Url new_url{url_ + "?" + parameters_.content};
            curl_easy_setopt(curl, CURLOPT_URL, new_url.data());
        } else {
            curl_easy_setopt(curl, CURLOPT_URL, url_.data());
        }

        curl_ -> error[0] = '\0';
        
        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, requests::util::writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        auto curl_error = curl_easy_perform(curl);

        char* raw_url;
        long response_code;
        double elapsed;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &raw_url);

        Error error(curl_error, curl_ -> error);

        Cookies cookies;
        struct curl_slist* raw_cookies;
        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &raw_cookies);
        for (struct curl_slist* nc = raw_cookies; nc; nc = nc -> next) {
            auto tokens = requests::util::split(nc -> data, '\t');
            auto value = tokens.back();
            tokens.pop_back();
            cookies[tokens.back()] = value;
        }
        curl_slist_free_all(raw_cookies);

        auto header = requests::util::parserHeader(header_string);
        return Response {
            static_cast<std::int32_t>(response_code),
            response_string,
            header,
            elapsed,
            raw_url,
            cookies,
            error
        };
    }

    Response Session::Impl::Get() {
        auto curl = curl_ -> handle;
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_POST, 0L);
            curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        }
        return makeRequest(curl);
    }

    Session::Session(): priImpl_{ new Impl()} {}
    Session::~Session() {}

    void Session::setParameters(const Parameters& params) {
        priImpl_ -> setParameters(params);
    }

    void Session::setParameters(Parameters&& params) {
        priImpl_ -> setParameters(params);
    }

    void Session::setUrl(const Url &url) {
        priImpl_ -> setUrl(url);
    }

    Response Session::Get() {
        return priImpl_ -> Get();
    }
}