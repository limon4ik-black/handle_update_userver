#include <userver/utest/utest.hpp>

#include "../utils/errors.hpp"
#include "../utils/validators.hpp"

namespace {

using namespace RobinID::utils::validators;
using namespace RobinID::utils::errors;

UTEST(ValidatorsTest, CheckSize) {
    EXPECT_TRUE(CheckSize("ValidRange", 3, 15));
    EXPECT_FALSE(CheckSize("No", 3, 15));
    EXPECT_FALSE(CheckSize(std::string(20, 'a'), 3, 15));
}

UTEST(ValidatorsTest, ValidateEmail) {
    EXPECT_NO_THROW({ ValidateEmail("test@example.com"); });
    EXPECT_THROW({ ValidateEmail(""); }, ValidationError);
    EXPECT_THROW({ ValidateEmail("invalid-email"); }, ValidationError);
}

UTEST(ValidatorsTest, ValidateUsername) {
    EXPECT_NO_THROW({ ValidateUsername("valid_user123"); });
    EXPECT_THROW({ ValidateUsername("invalid-user!"); }, ValidationError);
    EXPECT_THROW({ ValidateUsername(""); }, ValidationError);
    EXPECT_THROW({ ValidateUsername("неправильно"); }, ValidationError);
    EXPECT_THROW({ ValidateUsername("with spaces"); }, ValidationError);
}

UTEST(ValidatorsTest, ValidatePassword) {
    EXPECT_NO_THROW({ ValidatePassword("StrongSuperP@ss1"); });
    EXPECT_NO_THROW({ ValidatePassword("#n5WTedJ18qS+J83i*G("); });
    EXPECT_THROW({ ValidatePassword("WeakPassword"); }, ValidationError);
    EXPECT_THROW({ ValidatePassword("Sh@rt1"); }, ValidationError);
    EXPECT_THROW({ ValidatePassword(std::string(300, 'a') + "1A@"); }, ValidationError);
}

}  // namespace
