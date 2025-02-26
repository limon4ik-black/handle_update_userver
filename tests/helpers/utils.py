from enum import Enum

from faker import Faker

fake_factory = Faker()
fake_factory.seed_instance(42)


class Routes(str, Enum):
    SIGNUP = "/robin-id/users/v1/signup"
    LOGIN = "/robin-id/users/v1/login"
    PROFILE = "/robin-id/users/v1/profile"
    UPDATE = "/robin-id/users/v1/update"

    def __str__(self) -> str:
        return self.value


class RequiredFields(tuple, Enum):
    SIGNUP = 'username', 'email', 'password'
    LOGIN = 'username', 'password'
    UPDATE = 'user_id', 'payload'
