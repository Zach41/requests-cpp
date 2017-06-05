#include <memory>
#include <string>
#include <functional>
#include <utility>
#include <curl/curl.h>

#include "session.h"
#include "types.h"

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
        
        auto curl_error = curl_easy_perform(curl);

        return "ok";
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