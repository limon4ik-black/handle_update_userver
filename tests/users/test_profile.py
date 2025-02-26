import pytest
from http import HTTPStatus

from testsuite.daemons.service_client import Client

from models import User
from endpoints import user_profile
from validators import validate_profile_response


async def test_get_profile(
    service_client: Client,
    loggined_user: User,
) -> None:
    response = await user_profile(service_client, loggined_user)

    assert response.status == HTTPStatus.OK
    validate_profile_response(loggined_user, response)


@pytest.mark.parametrize(
    ('jwt_token',),
    [
        pytest.param(None, id='None_token'),
        pytest.param('some-token', id='invalid_format'),
        pytest.param(
            (
                'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9'
                '.eyJleHAiOjE3MzMzMjUzODIsInVzZXJfaWQ'
                'iOiJiY2VkNGQ1ZS1hOWU4LTQzOGUtYmE4OC0wZjFkYzcwYTFkNTMifQ'
                '.SoTE2VuY7BzJVP0IutxMDIGChijm0tkjaW31mZGZATw'
            ),
            id='invalid_jwt_token',
        ),
    ],
)
async def test_get_invalid_token(
    service_client: Client,
    loggined_user: User,
    jwt_token: str,
) -> None:
    loggined_user.token = jwt_token
    response = await user_profile(service_client, loggined_user)

    assert response.status == HTTPStatus.UNAUTHORIZED


async def test_user_not_found(
    service_client: Client,
    loggined_user: User,
) -> None:
    user = User(token=loggined_user.token)
    response = await user_profile(service_client, user)

    assert response.status == HTTPStatus.NOT_FOUND
