from fastapi import APIRouter
from .schemas import User

user_router = APIRouter()
@user_router.post("/user", response_model=User)
def new_user(
    user: User
):
    return user