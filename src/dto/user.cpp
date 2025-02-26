#include "user.hpp"

#include "../utils/consts.hpp"
#include "../utils/extractors.hpp"
#include "../utils/validators.hpp"
#include <userver/logging/log.hpp>

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

UsersV1UpdateRequest Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UsersV1UpdateRequest>) {
    UsersV1UpdateRequest request;
    LOG_INFO() << "Parsing UsersV1UpdateRequest...";
    
    request.username_ =
        utils::extractors::ExtractValueFromJson(json, utils::consts::kUserIdField, true);
    utils::validators::ValidateUsernameForUpdate(request.username_.value());// сделать проверку на user_id
    
    if(json.HasMember("payload") && json["payload"].IsObject()){
        request.payload_.emplace();
        const auto& payload_json = json["payload"]; 

        request.payload_->email_ = utils::extractors::ExtractValueFromJson(payload_json, utils::consts::kEmailField, false);
        if(request.payload_->email_.has_value())
            utils::validators::ValidateEmail(request.payload_->email_.value());

        request.payload_->new_username_ =
            utils::extractors::ExtractValueFromJson(payload_json, "new_username", false);
        if(request.payload_->new_username_.has_value())
            utils::validators::ValidateUsername(request.payload_->new_username_.value());

        request.payload_->password_ =
            utils::extractors::ExtractValueFromJson(payload_json, utils::consts::kPasswordField, false);
        if(request.payload_->password_.has_value())
            utils::validators::ValidatePassword(request.payload_->password_.value());
    }else{
        utils::validators::ValidatePayload();
    }
    return request;
}


}  // namespace RobinID::dto
