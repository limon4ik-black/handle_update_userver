import os
import pathlib
import sys
from http import HTTPStatus

import pytest

from testsuite.databases.pgsql import discover

sys.path.append(os.path.join(os.path.dirname(__file__), 'helpers'))


pytest_plugins = [
    'pytest_userver.plugins.core',
    'pytest_userver.plugins.postgresql',
]


@pytest.fixture
async def registered_user(service_client):
    from endpoints import user_signup
    from models import User

    user = User()
    response = await user_signup(service_client, user)
    assert response.status == HTTPStatus.OK

    user.id = response.json().get('user_id')
    return user


@pytest.fixture
async def loggined_user(service_client, registered_user):
    from endpoints import user_login
    from models import User

    response = await user_login(service_client, registered_user)
    assert response.status == HTTPStatus.OK

    registered_user.token = response.json().get('token')
    return registered_user


@pytest.fixture(scope='session')
def service_source_dir():
    """Path to root directory service."""
    return pathlib.Path(__file__).parent.parent


@pytest.fixture(scope='session')
def initial_data_path(service_source_dir):
    """Path for find files with data"""
    return [
        service_source_dir / 'postgresql/data',
    ]


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'pg_service_template',  # service name that goes to the DB connection
        [service_source_dir.joinpath('postgresql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))
