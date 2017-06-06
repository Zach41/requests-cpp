#include "session.h"

#include <iostream>
#include <cassert>

int main(void) {
    requests::Session s;
    s.setUrl("https://httpbin.org/cookies");
    s.setParameters({{"arg1", "hello"}, {"args2", "hi"}});
    auto response = s.Get();

    assert(response.error.code == requests::ErrorCode::OK);
    std::cout << "elapsed: " << response.elapsed
              << "\nURL: " << response.url
              << "\nHeader:\n";

    for (auto &item: response.header) {
        std::cout << '\t' << item.first << ": " << item.second << '\n';
    }
    std::cout << "Cookies:\n";
    std::cout << response.cookies.GetEncoded() << '\n';
    std::cout << "Body:\n";
    std::cout << "\t" << response.text << '\n';

    return 0;
}