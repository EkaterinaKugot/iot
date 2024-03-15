from sqlalchemy import Column, Integer, String, Float, DateTime
from database import Base
from datetime import datetime

class Item(Base):
    __tablename__ = "items"

    id = Column(Integer, primary_key=True, index=True, autoincrement=True)
    value = Column(Float)
    location = Column(String)
    device_id = Column(Integer)
    timestamp = Column(DateTime, default=datetime.utcnow)