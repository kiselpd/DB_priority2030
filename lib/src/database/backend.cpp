#include "backend.h"

// DBBackend
DBBackend::DBBackend(std::shared_ptr<DBConnectionPool> pool) : pool_(pool){};

pqxx::result DBBackend::doRequest(std::shared_ptr<DBBaseRequest> request)
{
    pqxx::result answer;
    auto free_conn = pool_->getFreeConnection();

    if (free_conn)
    {
        auto str_request = request->createRequest();

        if (!str_request.empty())
            answer = free_conn->doRequest(str_request);
    }

    pool_->setFreeConnection(free_conn);
    return answer;
};