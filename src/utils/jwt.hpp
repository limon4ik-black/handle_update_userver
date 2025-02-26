#pragma once

#include <jwt-cpp/jwt.h>
#include <userver/formats/json/value.hpp>

namespace RobinID::utils::jwt {

using DecodedToken = ::jwt::decoded_jwt<::jwt::traits::kazuho_picojson>;

struct JWTConfig final {
    JWTConfig(const userver::formats::json::Value& config);

    const std::string secret_key_;
    const std::chrono::seconds token_expiration_time_;
};

class JWTManager final {
   public:
    JWTManager(const JWTConfig& config);

    std::string GenerateToken(std::string user_id) const;

    void VerifyToken(const DecodedToken& token) const;

   private:
    const JWTConfig config_;
    ::jwt::verifier<::jwt::default_clock, ::jwt::traits::kazuho_picojson> verifier_{::jwt::verify()};
};

}  // namespace RobinID::utils::jwt
