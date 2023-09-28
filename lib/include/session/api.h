#ifndef API_H
#define API_H

#include "api_answer.h"
#include "api_builder.h"
#include "api_request.h"

#include "jwt_manager.h"
#include "backend.h"

namespace method
{
    const std::string POST = "POST";
    const std::string GET = "GET";
    const std::string PUT = "PUT";
    const std::string DELETE = "DELETE";
};

class RequestAPI
{
public:
    RequestAPI(std::shared_ptr<DBBackend> db_backend, std::shared_ptr<JWTAuthenticationManager> jwt_manager);

    std::string doRequest(const std::string &request_str);

private:
    std::shared_ptr<BaseAPIAnswer> doAuthenticatedRequest_(const RequestToAPI &request_to_api);
    std::shared_ptr<BaseAPIAnswer> doNotAuthenticatedRequest_(const RequestToAPI &request_to_api);

    std::shared_ptr<BaseAPIBuilder> getAPIBuilder_(const RequestToAPI &request_to_api);

    std::shared_ptr<DBBackend> db_backend_;
    std::shared_ptr<JWTAuthenticationManager> jwt_manager_;
};

#endif /*API_H*/