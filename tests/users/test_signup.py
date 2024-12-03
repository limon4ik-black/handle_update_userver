import pytest
from http import HTTPStatus

from testsuite.daemons.service_client import Client

from models import User, create_user_without_field
from endpoints import user_signup
from validators import validate_signup_response


async def test_signup(service_client: Client) -> None:
    user = User()
    response = await user_signup(service_client, user)

    assert response.status == HTTPStatus.OK, user.model_dump()
    validate_signup_response(response)


@pytest.mark.parametrize(
    ('duplicate_field', 'expected_status', 'expected_response'),
    [
        pytest.param(
            'username',
            HTTPStatus.CONFLICT,
            {'username': 'this username is already taken'},
            id='same_username',
        ),
        pytest.param('email', HTTPStatus.OK, None, id='same_email'),
        pytest.param('password', HTTPStatus.OK, None, id='same_password'),
    ]
)
async def test_signup_uniqueness(
    service_client: Client,
    duplicate_field: str,
    expected_status: int,
    expected_response: dict,
) -> None:
    # Create the initial user
    user = User()
    response = await user_signup(service_client, user)
    assert response.status_code == HTTPStatus.OK

    # Create another user with a duplicate field
    another_user = User(**{duplicate_field: getattr(user, duplicate_field)})
    response = await user_signup(service_client, another_user)

    assert response.status_code == expected_status

    if expected_response is not None:
        assert response.json() == expected_response


@pytest.mark.parametrize(
    ('user_data',),
    [
        pytest.param({'username': ''}, id='invalid_username'),
        pytest.param({'email': 'not_email'}, id='invalid_email'),
        pytest.param({'password': 'weak_password'}, id='invalid_password'),
        pytest.param({'username': None}, id='none_username'),
        pytest.param({'email': None}, id='none_email'),
        pytest.param({'password': None}, id='none_password'),
    ]
)
async def test_signup_validation(
    service_client: Client,
    user_data: dict
) -> None:
    user = User(**user_data)
    response = await user_signup(service_client, user)

    assert response.status_code == HTTPStatus.UNPROCESSABLE_ENTITY
    assert set(response.json().keys()) == set(user_data.keys())


@pytest.mark.parametrize(
    ('exclude_field', ),
    [
        pytest.param('username', id='without_username'),
        pytest.param('email', id='without_email'),
        pytest.param('password', id='without_password'),
    ],
)
async def test_without_field(
    service_client: Client,
    exclude_field: str,
) -> None:
    user = create_user_without_field(exclude_field)
    response = await user_signup(
        service_client, user, exclude=(exclude_field, ))

    assert response.status_code == HTTPStatus.UNPROCESSABLE_ENTITY
