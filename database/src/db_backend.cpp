#include "db_backend.h"

// DBBackend
DBBackend::DBBackend(std::shared_ptr<DBConnectionPool> pool){
    pool_ = pool;
    parser_ = std::make_shared<ClientParser>();
};

std::string DBBackend::doRequest(const std::string& json_request){
    request_parser::error error;
    auto [db_request_title, db_request] = parser_->parseFromJson(json_request, error);
    
    if(!error){
        auto free_conn = pool_->getFreeConnection();
        if(free_conn->isActive()){
            AnswerBody answer_body(free_conn->doRequest(db_request));
            AnswerTitle answer_title(answer_parser::status::OK, "20.08.2023");
            auto client_answer = parser_->parseToJson(answer_title, answer_body);
            pool_->setFreeConnection(free_conn);
            std::cout << db_request->createRequest() << std::endl;
            return client_answer;
        }
    }

    std::cout << error.getError() << std::endl;
    return "SHIT";
};