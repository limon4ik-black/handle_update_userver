#include "update.hpp"

#include "iostream"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>

#include <bcrypt/BCrypt.hpp>

#include "../../db/sql.hpp"
#include "../../dto/user.hpp"
#include "../../utils/consts.hpp"
#include "../../utils/errors.hpp"
#include "userver/components/component_config.hpp"

namespace RobinID::users::v1::update::patch {

Handler::Handler(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerJsonBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>(utils::consts::kDbName).GetCluster()) {}

userver::formats::json::Value Handler::HandleRequestJsonThrow(const userver::server::http::HttpRequest& request,
                                                              const userver::formats::json::Value& request_json,
                                                              userver::server::request::RequestContext&) const {
    dto::UsersV1UpdateRequest update_request;
    try {
        update_request = request_json.As<dto::UsersV1UpdateRequest>();
    } catch (const utils::errors::ValidationError& err) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return err.GetDetails();
    }

    std::string user_id;
    try {
        std::string password_hash;
        if(update_request.payload_->password_.has_value()){
            password_hash = BCrypt::generateHash(update_request.payload_->password_.value());
            } 
        else {
            password_hash = "";
            }
        std::cout << "user_id: " << update_request.username_.value() << "\n";
        std::cout << "new_username: " << update_request.payload_->new_username_.value_or("<null>") << "\n";
        std::cout << "email: " << update_request.payload_->email_.value_or("<null>") << "\n";
        std::cout << "password_hash: " << (password_hash.empty() ? "<null>" : password_hash) << "\n";

        const auto db_result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                                    db::sql::kUpdateUser.data(),update_request.username_.value(), update_request.payload_->new_username_.value_or(""),    
                                                    update_request.payload_->email_.value_or(""), password_hash);

        user_id = db_result.AsSingleRow<std::string>();
    } catch (const userver::storages::postgres::UniqueViolation& ex) {
        const auto constraint = ex.GetServerMessage().GetConstraint();
        throw;
    }

    userver::formats::json::ValueBuilder builder;
    builder["user_id"] = user_id;
    return builder.ExtractValue();
}

}