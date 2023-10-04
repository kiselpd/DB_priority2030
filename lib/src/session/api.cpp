#include "api.h"

#include "api_request.h"
#include "api_builder.h"

// RequestAPI
RequestAPI::RequestAPI(std::shared_ptr<DBBackend> db_backend, std::shared_ptr<JWTAuthenticationManager> jwt_manager) : db_backend_(db_backend),
                                                                                                                       jwt_manager_(jwt_manager){};

std::string RequestAPI::doRequest(const std::string &request_str)
{
    RequestToAPI request_to_api;
    std::shared_ptr<BaseAPIAnswer> answer;

    if (request_to_api.fill(request_str))
    {
        if (jwt_manager_->isAuthenticated(request_to_api.getToken()))
            answer = this->doAuthenticatedRequest_(request_to_api);
        else
            answer = this->doNotAuthenticatedRequest_(request_to_api);
    }
    else
        answer = std::make_shared<ErrorAPIAnswer>(StatusAPICode::BadRequest, "Request structure error!");

    return answer->get();
};

std::shared_ptr<BaseAPIAnswer> RequestAPI::doAuthenticatedRequest_(const RequestToAPI &request_to_api)
{
    auto api_builder = this->getAPIBuilder_(request_to_api);
    std::shared_ptr<BaseAPIAnswer> answer;

    if (api_builder)
    {
        auto request_to_db = api_builder->build();

        if (request_to_db)
            answer = std::make_shared<ResultAPIAnswer>(StatusAPICode::OK, db_backend_->doRequest(request_to_db));
        else
            answer = std::make_shared<ErrorAPIAnswer>(StatusAPICode::BadRequest, "Request parametrs error!");
    }
    else
        answer = std::make_shared<ErrorAPIAnswer>(StatusAPICode::BadRequest, "Request method error!");

    return answer;
};

std::shared_ptr<BaseAPIAnswer> RequestAPI::doNotAuthenticatedRequest_(const RequestToAPI &request_to_api)
{
    std::shared_ptr<BaseAPIBuilder> api_builder = std::make_shared<AuthAPIBuilder>(request_to_api.getRequest());
    std::shared_ptr<BaseAPIAnswer> answer;

    if (api_builder)
    {
        auto request_to_db = api_builder->build();

        if (request_to_db)
        {
            auto result = db_backend_->doRequest(request_to_db);

            if (!result.empty())
            {
                auto token = jwt_manager_->addNewJWT(result[0][0].c_str(), result[0][4].c_str());
                answer = std::make_shared<ResultAPIAnswer>(StatusAPICode::OK, token);
            }
            else
                answer = std::make_shared<ErrorAPIAnswer>(StatusAPICode::NotFound, "Incorrect login or password!");
        }
        else
            answer = std::make_shared<ErrorAPIAnswer>(StatusAPICode::BadRequest, "Request parametrs error!");
    }
    else
        answer = std::make_shared<ErrorAPIAnswer>(StatusAPICode::Unauthorized, "You are not authorized!");

    return answer;
};

std::shared_ptr<BaseAPIBuilder> RequestAPI::getAPIBuilder_(const RequestToAPI &request_to_api)
{
    auto [user_id, user_role] = jwt_manager_->getInfo(request_to_api.getToken());
    std::shared_ptr<BaseAPIBuilder> api_builder;

    if (!(user_id.empty() || user_role.empty()))
    {
        if (request_to_api.getMethod() == method::GET)
            return std::make_shared<SelectAPIBuilder>(user_id, request_to_api.getRequest());

        if (request_to_api.getMethod() == method::POST)
            return std::make_shared<UpdateAPIBuilder>(user_id, request_to_api.getRequest());

        if (request_to_api.getMethod() == method::PUT)
            return std::make_shared<InsertAPIBuilder>(user_id, request_to_api.getRequest());

        if (request_to_api.getMethod() == method::DELETE)
            return std::make_shared<DeleteAPIBuilder>(user_id, request_to_api.getRequest());
    }

    return nullptr;
};