#include "user.hpp"

#include "../utils/consts.hpp"
#include "../utils/extractors.hpp"
#include "../utils/validators.hpp"

namespace RobinID::dto {

UsersV1SignupRequest Parse(const userver::formats::json::Value& json,
                           userver::formats::parse::To<UsersV1SignupRequest>) {
    UsersV1SignupRequest request;

    request.email_ = utils::extractors::ExtractValueFromJson(json, utils::consts::kEmailField, true).value();
    utils::validators::ValidateEmail(request.email_);

    request.username_ =
        utils::extractors::ExtractValueFromJson(json, utils::consts::kUsernameField, true).value();
    utils::validators::ValidateUsername(request.username_);

    request.password_ =
        utils::extractors::ExtractValueFromJson(json, utils::consts::kPasswordField, true).value();
    utils::validators::ValidatePassword(request.password_);

    return request;
}

UsersV1LoginRequest Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UsersV1LoginRequest>) {
    UsersV1LoginRequest request;

    request.username_ =
        utils::extractors::ExtractValueFromJson(json, utils::consts::kUsernameField, true).value();
    utils::validators::ValidateUsername(request.username_);

    request.password_ =
        utils::extractors::ExtractValueFromJson(json, utils::consts::kPasswordField, true).value();
    utils::validators::ValidatePassword(request.password_);

    return request;
}

userver::formats::json::Value Serialize(const UsersProfileResponse& data,
                                        userver::formats::serialize::To<userver::formats::json::Value>) {
    userver::formats::json::ValueBuilder builder;
    builder["id"] = data.id_;
    builder["username"] = data.username_;
    builder["email"] = data.email_;

    return builder.ExtractValue();
}

}  // namespace RobinID::dto
