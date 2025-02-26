#pragma once

#include <userver/formats/json.hpp>

namespace RobinID::utils::validators {

bool CheckSize(std::string_view value, int min, int max);

void ValidateEmail(std::string_view email);
void ValidateUsername(std::string_view username);
void ValidatePassword(std::string_view password);
void ValidatePayload();
void ValidateUsernameForUpdate(std::string_view username);
}  // namespace RobinID::utils::validators
