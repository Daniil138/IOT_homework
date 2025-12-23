import time
import serial
from paho.mqtt.client import Client, MQTTMessage
from paho.mqtt.enums import CallbackAPIVersion
import random

broker = "broker.emqx.io"
luminosity_topic = "laboratory/greenhouse/luminosity_dzz"
ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=2)  
light_state = "off"

def on_connect(client: Client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("Успешное подключение к MQTT-брокеру!")
    else:
        print("Не удалось подключиться, код возврата %d", reason_code)

def on_message(client: Client, userdata, message: MQTTMessage):
    global light_state
    data = int(message.payload.decode("utf-8"))
    print(f"Получено сообщение {data} из топика {message.topic}")
    
    new_state = "u" if data > 1020 else "d"
    if new_state != light_state:
        ser.write(new_state.encode())
        light_state = new_state

if __name__ == "__main__":
    client = Client(
        callback_api_version=CallbackAPIVersion.VERSION2,
        client_id=f'MY_CLIENT_ID_{random.randint(10000, 99999)}'
    )
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(broker)

    client.loop_start()
    client.subscribe(luminosity_topic, qos=2)
    time.sleep(1800)
    client.disconnect()
    client.loop_stop()
    ser.close()
