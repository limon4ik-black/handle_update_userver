from http import HTTPStatus

from testsuite.daemons.service_client import Client

from endpoints import user_login
from models import User
from validators import validate_login_response
from utils import fake_factory


async def test_login(service_client: Client, registered_user: User) -> None:
    response = await user_login(service_client, registered_user)
    assert response.status_code == HTTPStatus.OK
    validate_login_response(registered_user, response)


async def test_login_unknown(service_client: Client) -> None:
    user = User()
    response = await user_login(service_client, user)
    assert response.status_code == HTTPStatus.NOT_FOUND


async def test_invalid_password(
    service_client: Client,
    registered_user: User,
) -> None:
    registered_user.password = fake_factory.password(length=20)
    response = await user_login(service_client, registered_user)
    assert response.status_code == HTTPStatus.FORBIDDEN
