import time
from paho.mqtt.client import Client
from paho.mqtt.enums import CallbackAPIVersion
import random
import serial

broker="broker.emqx.io"
luminosity_topic = "laboratory/greenhouse/luminosity_dzz"

# Важно: идентификатор клиента должен быть уникален
if __name__ == "__main__":
    client = Client(
        callback_api_version=CallbackAPIVersion.VERSION2,
        client_id=f'MY_CLIENT_ID_{random.randint(10000, 99999)}'
    )
    client.connect(broker)
    ser = serial.Serial('/dev/ttyUSB1', 9600, timeout=1)
    time.sleep(2)

    ser.write(b's')
    time.sleep(0.1)

    while True:
        if ser.in_waiting > 0:
            raw_line = ser.readline().decode('utf-8', errors='ignore').strip()
            print(raw_line)
            client.publish(luminosity_topic, raw_line, qos=0)

    client.disconnect()
