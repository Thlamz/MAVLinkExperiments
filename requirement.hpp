#include <boost/asio.hpp>
#include "mavlink_helper.hpp"

#ifndef REQUIREMENT_HEADER
#define REQUIREMENT_HEADER
/**
 * @brief Function sets a requirement on the MAVLinkHelper that will be checked for completion against any recieved telemetry.
 * An optional timeout is allowed and will interrupt and resolve the requirement with a false value.
 * 
 * Sets timed_out error code if the operation times out.
 * 
 * @tparam Token Boost completion token type
 * @param cond Condition function that should be called to check for the requiremement's completion. Should return true when the requirement is resolved.
 * @param helper MAVLinkHelper instance
 * @param token Token instance
 * @param duration Seconds before the requirement forcibly resolves (optional).
 * @param timer Instance of the timer that should be used to register the timeout (required if duration is set).
 * @return void
 */
template <typename Token>
void async_requirement(Condition cond, std::shared_ptr<MAVLinkHelper> helper, Token&& token, int timeout = -1, boost::asio::steady_timer* timer = nullptr)
{
    using result_type = typename boost::asio::async_result<std::decay_t<Token>, bool(void)>;
    typename result_type::completion_handler_type handler(std::forward<Token>(token));

    result_type result(handler);

    helper->add_requirement(cond, handler);

    if(timeout >= 0) {
        assert(timer != nullptr);

        timer->expires_after(std::chrono::seconds(timeout));
        timer->async_wait([&helper, completer = std::function<void (boost::system::error_code)>(handler)](const boost::system::error_code& ec) {
            if(!ec) {
                helper->cancel_requirement();
                completer(boost::asio::error::timed_out);
            }
        });
    }

    return result.get();
};
#endif

