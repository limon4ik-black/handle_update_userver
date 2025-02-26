#include <userver/server/http/http_request.hpp>
#include <userver/utest/http_server_mock.hpp>
#include <userver/utest/utest.hpp>

#include "../utils/consts.hpp"
#include "../utils/errors.hpp"
#include "../utils/extractors.hpp"

namespace {

using namespace RobinID::utils;

UTEST(ExtractorsTest, ExtractValueFromJson_FieldPresent) {
    userver::formats::json::ValueBuilder builder;
    builder[consts::kEmailField.data()] = "test@example.com";
    auto json = builder.ExtractValue();

    EXPECT_EQ(extractors::ExtractValueFromJson(json, consts::kEmailField, true).value(), "test@example.com");
}

UTEST(ExtractorsTest, ExtractValueFromJson_FieldAbsentRequired) {
    userver::formats::json::ValueBuilder builder;
    auto json = builder.ExtractValue();

    EXPECT_THROW({ extractors::ExtractValueFromJson(json, consts::kEmailField, true); },
                 errors::ValidationError);
}

UTEST(ExtractorsTest, ExtractValueFromJson_FieldAbsentNotRequired) {
    userver::formats::json::ValueBuilder builder;
    auto json = builder.ExtractValue();

    EXPECT_FALSE(extractors::ExtractValueFromJson(json, consts::kEmailField, false).has_value());
}

}  // namespace
