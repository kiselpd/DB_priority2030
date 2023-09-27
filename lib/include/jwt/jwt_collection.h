#ifndef JWT_COLLECTION_H
#define JWT_COLLECTION_H

#include <map>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

typedef std::string token_str;

const size_t DEFAULT_TIMER_DURATION_IN_SECONDS = 10;

class JWTCollection
{
public:
    bool has(const token_str &token);
    bool isEmpty();

    void add(const token_str &token);
    void remove(const token_str &token);

    std::pair<token_str, boost::posix_time::ptime> getOldest(const token_str &token);

private:
    boost::posix_time::ptime getTime_() const;

    std::map<token_str, boost::posix_time::ptime> token_map_;
    std::mutex mut_;
};

class JWTStatusTimer
{
public:
    JWTStatusTimer(std::shared_ptr<boost::asio::io_service> io_ptr, std::shared_ptr<JWTCollection> collection, const size_t &timer_duration_in_minutes = DEFAULT_TIMER_DURATION_IN_SECONDS);

    void start(const token_str &token, const boost::posix_time::ptime &time);
    void stop();

private:
    void go_timer_(const boost::posix_time::time_duration time);
    void timer_heandler_(const boost::system::error_code &error);

    std::shared_ptr<JWTCollection> collection_;

    boost::asio::deadline_timer timer_;
    size_t timer_duration_in_minutes_;
    std::atomic<bool> is_active_;

    token_str tmp_token_;
};

#endif /*JWT_COLLECTION_H*/