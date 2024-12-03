from testsuite.utils import matching

from models import User


UUID_REGEX = (
    "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[1-5][0-9a-fA-F]{3}-"
    "[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$"
)

TOKEN_REGEX = r'^[\w-]+\.[\w-]+\.[\w-]+$'


def validate_signup_response(response) -> None:
    assert response.json() == {
        'user_id': matching.RegexString(UUID_REGEX),
    }


def validate_login_response(user: User, response) -> None:
    assert response.json() == {
        'id': matching.RegexString(UUID_REGEX),
        'username': user.username,
        'email': user.email,
        'token': matching.RegexString(TOKEN_REGEX),
    }


def validate_profile_response(user: User, response) -> None:
    assert response.json() == {
        'id': matching.RegexString(UUID_REGEX),
        'username': user.username,
        'email': user.email,
    }
