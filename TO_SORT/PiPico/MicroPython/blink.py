from machine import Pin
from time import sleep
led_gpio25 = Pin(25, Pin.OUT)
while True:
    led_gpio25.value(1)
    print("ON")
    sleep(1)
    led_gpio25.value(0)
    print("OFF")
    sleep(1)
    