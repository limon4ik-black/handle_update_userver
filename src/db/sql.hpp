#pragma once

#include <string_view>

namespace RobinID::db::sql {

inline constexpr std::string_view kCreateUser{R"~(
select robinid.add_new_user($1::citext, $2, $3);
)~"};

inline constexpr std::string_view kGetUserByUsername{R"~(
select robinid.get_user_by_username($1::citext);
)~"};

inline constexpr std::string_view kUpdateUser{R"~(
select robinid.update_user($1::text, $2::citext, $3::varchar, $4::varchar);
)~"};

// $1:uuid
}  // namespace RobinID::db::sql
