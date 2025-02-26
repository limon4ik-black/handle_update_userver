#pragma once

#include <optional>
#include <string>
#include <userver/formats/json.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include "userver/formats/json/value.hpp"

namespace RobinID::dto {

struct UsersV1SignupRequest final {
    std::string username_;
    std::string email_;
    std::string password_;
};

struct UsersV1LoginRequest final {
    std::string username_;
    std::string password_;
};

struct UsersProfileResponse final {
    std::string id_;
    std::string username_;
    std::string email_;
};

struct Updateable_fields {
    std::optional<std::string> new_username_;
    std::optional<std::string> email_;
    std::optional<std::string> password_;
};

struct UsersV1UpdateRequest final {
    std::optional<std::string> username_;
    std::optional<Updateable_fields> payload_;
};


// struct UsersV1UpdateRequest final {
//     std::optional<std::string> username_;
//     std::optional<std::string> new_username_;
//     std::optional<std::string> email_;
//     std::optional<std::string> password_;
// };
// struct UsersV1UpdateRequest final {
//     std::optional<std::string> username_;
//     struct Updateable_fields final{
//         std::optional<std::string> new_username_;
//         std::optional<std::string> email_;
//         std::optional<std::string> password_;
//     }     
// };

UsersV1SignupRequest Parse(const userver::formats::json::Value& json,
                           userver::formats::parse::To<UsersV1SignupRequest>);

UsersV1LoginRequest Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UsersV1LoginRequest>);

userver::formats::json::Value Serialize(const UsersProfileResponse& data,
                                        userver::formats::serialize::To<userver::formats::json::Value>);


UsersV1UpdateRequest Parse(const userver::formats::json::Value& json,
                            userver::formats::parse::To<UsersV1UpdateRequest>);

}  // namespace RobinID::dto
