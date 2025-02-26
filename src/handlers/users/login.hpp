#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "../../utils/jwt.hpp"

namespace RobinID::users::v1::login::post {

class Handler final : public userver::server::handlers::HttpHandlerJsonBase {
   public:
    static constexpr std::string_view kName = "users-v1-login";

    Handler(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request, const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& request_context) const override final;

   private:
    const userver::storages::postgres::ClusterPtr pg_cluster_;
    const utils::jwt::JWTManager jwt_manager_;
};

}  // namespace RobinID::users::v1::login::post
