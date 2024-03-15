from fastapi import FastAPI
from src.users.router import user_router
from src.devices.router import device_router
from src.measurements.router import measurements_router


app = FastAPI()

app.include_router(measurements_router)
app.include_router(user_router)
app.include_router(device_router)

@app.get("/")
def greet():
    return {"Message": "Hello!!!"}
