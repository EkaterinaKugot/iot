import time
import paho.mqtt.client as paho
import random

broker = "broker.emqx.io"

client = paho.Client("client-isu-153")

print("Connection to broker", broker)
client.connect(broker)
client.loop_start()
print("Publishing")

for _ in range(20):
    state = "u" if random.randint(0,1) else "d"
    #print(f"state is {state}")
    #client.publish("iotps/homework", state)
    st = 'led off'
    client.publish("house/led", st)
    print(f"state is {st}")
    time.sleep(3)

client.disconnect()
client.loop_stop()