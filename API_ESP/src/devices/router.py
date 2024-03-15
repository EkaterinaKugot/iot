from fastapi import APIRouter
from .schemas import Device

device_router = APIRouter()
@device_router.post("/device", response_model=Device)
def new_device(
    device: Device
):
    return device