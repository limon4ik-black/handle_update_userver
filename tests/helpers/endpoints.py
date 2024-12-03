from typing import Optional, Tuple

from testsuite.daemons.service_client import Client

from utils import Routes, RequiredFields
from models import User


async def user_signup(
    service_client: Client,
    user: User,
    exclude: Optional[Tuple[str]] = None,
):
    include = RequiredFields.SIGNUP.value

    return await service_client.post(
        Routes.SIGNUP,
        json=user.model_dump(include=include, exclude=exclude)
    )


async def user_login(
    service_client: Client,
    user: User,
    exclude: Optional[Tuple[str]] = None,
):
    include = RequiredFields.LOGIN.value

    return await service_client.post(
        Routes.LOGIN,
        json=user.model_dump(include=include, exclude=exclude)
    )


async def user_profile(service_client: Client, user: User):
    return await service_client.get(
        Routes.PROFILE.value,
        bearer=user.token,
        params={'username': user.username},
    )
