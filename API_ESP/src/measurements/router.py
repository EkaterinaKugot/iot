from fastapi import APIRouter, Depends
from src.measurements.schemas import TempData
from sqlalchemy.orm import Session
from . import models
from database import SessionLocal, engine

models.Base.metadata.create_all(bind=engine)

measurements_router = APIRouter()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@measurements_router.get("/get_temperature/")
def get_temperature(
    location: str, db: Session = Depends(get_db)
):
    db_items = db.query(models.Item).filter(models.Item.location == location).all()
    if len(db_items) == 0:
        return {"message": "none"}
    values = [db_items[i].value for i in range(len(db_items))]
    return {
        "Values": values,
        "Location": db_items[0].location
        }

@measurements_router.post("/send_temperature")
def send_temperature(
    data: TempData, db: Session = Depends(get_db)
):
    db_item = models.Item(**data.dict())
    db.add(db_item)
    db.commit()
    db.refresh(db_item)
    return db_item