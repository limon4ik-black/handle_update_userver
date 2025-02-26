#include "extractors.hpp"

#include <userver/formats/json/exception.hpp>

#include "consts.hpp"
#include "errors.hpp"

namespace RobinID::utils::extractors {

std::optional<std::string> ExtractValueFromJson(const userver::formats::json::Value& json,
                                                std::string_view field_name, bool required) {
    if (!json.HasMember(field_name)) {
        if (required) {
            throw errors::ValidationError{field_name, consts::kRequired};
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::string> value;
    try {
        value = json[field_name].As<std::string>();
    } catch (const userver::formats::json::TypeMismatchException& ex) {
        throw errors::ValidationError{field_name, consts::kInvalidType};
    }

    return value;
}

}  // namespace RobinID::utils::extractors
