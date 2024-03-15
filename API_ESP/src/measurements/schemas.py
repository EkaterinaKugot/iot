from datetime import datetime
from pydantic import BaseModel

class TempData(BaseModel):
    value: float
    location: str
    device_id: int
    timestamp: datetime

    class Config:
        from_attributes = True