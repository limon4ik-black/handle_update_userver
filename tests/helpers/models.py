from typing import Optional

from pydantic import BaseModel, Field

from utils import fake_factory


class User(BaseModel):
    username: Optional[str] = Field(default_factory=fake_factory.user_name)
    email: Optional[str] = Field(default_factory=fake_factory.email)
    password: Optional[str] = Field(
        default_factory=lambda: fake_factory.password(length=20))
    id: Optional[str] = None
    token: Optional[str] = None


    def create_user_for_update(self, field: str = None) -> dict:
            payload = {
                "new_username": self.username,
                "email": self.email,
                "password": self.password
            }
            if field in payload:
                del payload[field]
            return {
                "user_id": self.id,
                "payload": payload
            }
        


def create_user_without_field(field: str) -> User:
    data = User().model_dump(exclude={field})
    return User.model_construct(**data)
