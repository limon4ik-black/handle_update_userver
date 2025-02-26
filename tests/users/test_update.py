from http import HTTPStatus
import pytest
from testsuite.daemons.service_client import Client
from models import User
from validators import validate_update_response
from endpoints import user_update


async def test_update(service_client: Client, registered_user: User) -> None:
    response = await user_update(service_client, registered_user)
    assert response.status_code == HTTPStatus.OK
    validate_update_response(response)

@pytest.mark.parametrize(
    ('field'),
    [
        pytest.param('password',id='without_password'),
        pytest.param('email',id='without_email'),
        pytest.param('new_username',id='without_new_username'),
    ]
)
async def test_update_required(service_client: Client, registered_user: User, field: str) -> None:
    response = await user_update(service_client, registered_user, field)
    assert response.status_code == HTTPStatus.OK
