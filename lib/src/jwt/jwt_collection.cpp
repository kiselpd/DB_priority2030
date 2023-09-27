#include "jwt_collection.h"

#include <shared_mutex>

// JWTCollection
bool JWTCollection::has(const token_str &token)
{
    std::shared_lock locker(mut_);

    if (auto it = token_map_.find(token); it != token_map_.end())
    {
        it->second = this->getTime_();
        return true;
    }
    else
        return false;
};

bool JWTCollection::isEmpty()
{
    std::shared_lock locker(mut_);
    return token_map_.empty();
};

void JWTCollection::add(const token_str &token)
{
    std::unique_lock locker(mut_);
    token_map_.emplace(token, this->getTime_());
};

void JWTCollection::remove(const token_str &token)
{
    std::unique_lock locker(mut_);
    token_map_.erase(token);
};

std::pair<token_str, boost::posix_time::ptime> JWTCollection::getOldest(const token_str &token)
{
    std::shared_lock locker(mut_);

    auto min_lambda = [](const std::pair<token_str, boost::posix_time::ptime> &a, const std::pair<token_str, boost::posix_time::ptime> &b)
    {
        return a.second < b.second;
    };

    auto min_it = std::min_element(token_map_.begin(), token_map_.end(), min_lambda);
    return *min_it;
};

// JWTStatusTimer
JWTStatusTimer::JWTStatusTimer(std::shared_ptr<boost::asio::io_service> io_ptr, std::shared_ptr<JWTCollection> collection, const size_t &timer_duration_in_minutes = DEFAULT_TIMER_DURATION_IN_SECONDS) : timer_(*io_ptr),
                                                                                                                                                                                                          collection_(collection),
                                                                                                                                                                                                          timer_duration_in_minutes_(timer_duration_in_minutes),
                                                                                                                                                                                                          is_active_(false){};

void JWTStatusTimer::start(const token_str &token, const boost::posix_time::ptime &time)
{
    tmp_token_ = token;
    this->stop();
    this->go_timer_(boost::posix_time::second_clock::local_time() - time);
};

void JWTStatusTimer::JWTStatusTimer::stop()
{
    if (this->is_active_.load(std::memory_order_acquire))
        timer_.cancel();

    this->is_active_.store(false, std::memory_order_release);
};

void JWTStatusTimer::go_timer_(const boost::posix_time::time_duration time)
{
    this->is_active_.store(true, std::memory_order_release);

    auto wait_handler = [this](const boost::system::error_code &error)
    {
        this->timer_heandler_(error);
    };

    timer_.expires_from_now(boost::posix_time::minutes(timer_duration_in_minutes_) - time);
    timer_.async_wait(wait_handler);
};

void JWTStatusTimer::timer_heandler_(const boost::system::error_code &error)
{
    if (!error)
        collection_->remove(tmp_token_);
};
