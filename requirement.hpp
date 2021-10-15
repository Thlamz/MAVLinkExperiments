#include <boost/asio.hpp>
#include <iostream>

typedef std::function<bool(mavlink_message_t)> Condition;

#ifndef REQUIREMENT
#define REQUIREMENT
struct requirement {
    requirement(Condition cond) {
        requirement::condition = [this, cond](mavlink_message_t msg) { 
            return cond(msg); 
        };
    };

    public:
        Condition condition;
};
#endif