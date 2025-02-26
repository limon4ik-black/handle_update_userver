#include "login.hpp"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/secdist/component.hpp>

#include <bcrypt/BCrypt.hpp>

#include "../../db/sql.hpp"
#include "../../db/types.hpp"
#include "../../dto/user.hpp"
#include "../../utils/consts.hpp"
#include "../../utils/errors.hpp"

namespace RobinID::users::v1::login::post {

Handler::Handler(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerJsonBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>(utils::consts::kDbName).GetCluster()),
      jwt_manager_(context.FindComponent<userver::components::Secdist>().Get().Get<utils::jwt::JWTConfig>()) {}

userver::formats::json::Value Handler::HandleRequestJsonThrow(const userver::server::http::HttpRequest& request,
                                                              const userver::formats::json::Value& request_json,
                                                              userver::server::request::RequestContext&) const {
    dto::UsersV1LoginRequest login_request;
    try {
        login_request = request_json.As<dto::UsersV1LoginRequest>();
    } catch (const utils::errors::ValidationError& err) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return err.GetDetails();
    }

    const auto db_result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                                                db::sql::kGetUserByUsername.data(), login_request.username_);
    if (db_result.IsEmpty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return utils::errors::MakeError(utils::consts::kUsernameField, utils::consts::kInvalid);
    }

    const auto user = db_result.AsSingleRow<db::types::User>();
    if (!BCrypt::validatePassword(login_request.password_, user.password_hash_)) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kForbidden);
        return utils::errors::MakeError(utils::consts::kPasswordField, utils::consts::kInvalid);
    }

    userver::formats::json::ValueBuilder builder = dto::UsersProfileResponse{
        std::move(user.id_),
        std::move(user.username_),
        std::move(user.email_),
    };
    builder["token"] = jwt_manager_.GenerateToken(std::move(user.id_));

    return builder.ExtractValue();
}

}  // namespace RobinID::users::v1::login::post
