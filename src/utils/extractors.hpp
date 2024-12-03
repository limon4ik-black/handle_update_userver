#pragma once

#include <userver/formats/json/value.hpp>
#include <userver/server/http/http_request.hpp>

namespace RobinID::utils::extractors {

std::optional<std::string> ExtractValueFromJson(const userver::formats::json::Value& json,
                                                std::string_view field_name, bool required);

}  // namespace RobinID::utils::extractors
