
#include <userver/formats/json/serialize_duration.hpp>
#include <userver/logging/log.hpp>
#include <userver/utest/utest.hpp>

#include "../utils/jwt.hpp"

namespace {

using namespace RobinID::utils::jwt;

inline constexpr std::string_view kUserId{"1f44a1d3-0884-4ffc-b595-4c7dafd5feaa"};

userver::formats::json::Value GetTestConfigJson() {
    userver::formats::json::ValueBuilder builder;
    builder["secret_key"] = "secret_key";
    builder["jwt_expiration_time"] = std::chrono::seconds{86400};
    return builder.ExtractValue();
}

UTEST(JWTManager, GenerateToken) {
    const JWTConfig config{GetTestConfigJson()};
    const JWTManager jwt_manager{config};
    const auto token = jwt_manager.GenerateToken(kUserId.data());
    const auto decoded_token = ::jwt::decode(token);
    const auto id = decoded_token.get_payload_claim("user_id").as_string();
    ASSERT_EQ(id, kUserId);
}

UTEST(JWTManager, VerifyToken) {
    const JWTConfig config{GetTestConfigJson()};
    const JWTManager jwt_manager{config};
    const auto token = jwt_manager.GenerateToken(kUserId.data());
    const auto decoded_token = ::jwt::decode(token);
    try {
        jwt_manager.VerifyToken(decoded_token);
    } catch (const std::exception& ex) {
        FAIL() << "JWTManager::VerifyToken failed";
    }
}

}  // namespace
