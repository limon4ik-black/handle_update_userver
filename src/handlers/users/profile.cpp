#include "profile.hpp"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>

#include "../../db/sql.hpp"
#include "../../db/types.hpp"
#include "../../dto/user.hpp"
#include "../../utils/consts.hpp"
#include "../../utils/errors.hpp"
#include "../../utils/validators.hpp"

namespace RobinID::users::v1::profile::get {

Handler::Handler(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerJsonBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>(utils::consts::kDbName).GetCluster()) {}

userver::formats::json::Value Handler::HandleRequestJsonThrow(const userver::server::http::HttpRequest& request,
                                                              const userver::formats::json::Value&,
                                                              userver::server::request::RequestContext&) const {
    const auto username = request.GetArg(utils::consts::kUsernameField);
    try {
        utils::validators::ValidateUsername(username);
    } catch (const utils::errors::ValidationError& err) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnprocessableEntity);
        return err.GetDetails();
    }

    const auto db_result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                                                db::sql::kGetUserByUsername.data(), username);
    if (db_result.IsEmpty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return utils::errors::MakeError(utils::consts::kUsernameField, utils::consts::kInvalid);
    }

    const auto user = db_result.AsSingleRow<db::types::User>();
    userver::formats::json::ValueBuilder builder = dto::UsersProfileResponse{
        std::move(user.id_),
        std::move(user.username_),
        std::move(user.email_),
    };

    return builder.ExtractValue();
}

}  // namespace RobinID::users::v1::profile::get
