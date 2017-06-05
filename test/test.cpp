#include "session.h"

int main(void) {
    requests::Session s;
    s.setUrl("https://httpbin.org/get");
    s.setParameters({{"arg1", "hello"}, {"args2", "hi"}});
    s.Get();
}