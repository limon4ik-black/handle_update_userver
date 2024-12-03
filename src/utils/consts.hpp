#pragma once

#include <regex>
#include <string_view>

namespace RobinID::utils::consts {

// DB
inline constexpr std::string_view kDbName{"postgres-db-1"};

// Error messages
inline constexpr std::string_view kInvalid{"invalid"};
inline constexpr std::string_view kEmpty{"cannot be empty"};
inline constexpr std::string_view kRequired{"required"};
inline constexpr std::string_view kInvalidType{"invalid type"};
inline constexpr std::string_view kLengthErrorMessage{"{} length must be between {} and {} characters"};

// email
inline constexpr std::string_view kEmailField{"email"};
inline const std::regex kEmailPattern{"(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"};

// username
inline constexpr std::string_view kUsernameField{"username"};
inline const std::regex kUsernamePattern{"^[a-zA-Z0-9_]+$"};
inline const int kUsernameLengthMin = 3;
inline const int kUsernameLengthMax = 32;

// password
inline constexpr std::string_view kPasswordField{"password"};
inline const std::regex kPasswordPattern{
    "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[!@#$%^&*()_\\-+=\\[\\]{};':\"\\|,.<>/?]).+$"};
inline const int kPasswordLengthMin = 16;
inline const int kPasswordLengthMax = 256;

}  // namespace RobinID::utils::consts
