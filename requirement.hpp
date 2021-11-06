#include <boost/asio.hpp>
#include <iostream>
#include "mavlink_helper.hpp"

#ifndef REQUIREMENT
#define REQUIREMENT
template <typename Token>
auto async_requirement(Condition cond, std::shared_ptr<MAVLinkHelper> helper, Token&& token)
{
    using result_type = typename boost::asio::async_result<std::decay_t<Token>, void(void)>;
    typename result_type::completion_handler_type handler(std::forward<Token>(token));

    result_type result(handler);

    helper->add_requirement(cond, handler);

    return result.get ();
};
#endif

