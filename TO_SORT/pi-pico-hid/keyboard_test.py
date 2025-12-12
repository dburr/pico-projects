"""CircuitPython Essentials Pin Map Script"""
import time
import microcontroller
import board
from digitalio import DigitalInOut, Direction, Pull
import busio
import usb_hid
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS
from adafruit_hid.keycode import Keycode
from adafruit_debouncer import Debouncer

led = DigitalInOut(board.LED)
led.direction = Direction.OUTPUT

buttonA = DigitalInOut(board.GP0)
buttonA.direction = Direction.INPUT
buttonA.pull = Pull.UP
# https://learn.adafruit.com/debouncer-library-python-circuitpython-buttons-sensors/basic-debouncing
# switch = Debouncer(pin, interval=0.05)
dButtonA = Debouncer(buttonA)

buttonB = DigitalInOut(board.GP1)
buttonB.direction = Direction.INPUT
buttonB.pull = Pull.UP
# https://learn.adafruit.com/debouncer-library-python-circuitpython-buttons-sensors/basic-debouncing
# switch = Debouncer(pin, interval=0.05)
dButtonB = Debouncer(buttonB)

buttonC = DigitalInOut(board.GP2)
buttonC.direction = Direction.INPUT
buttonC.pull = Pull.UP
# https://learn.adafruit.com/debouncer-library-python-circuitpython-buttons-sensors/basic-debouncing
# switch = Debouncer(pin, interval=0.05)
dButtonC = Debouncer(buttonC)

#setup a keyboard as a device
kbd = Keyboard(usb_hid.devices)
keyboard = KeyboardLayoutUS(kbd)

buttonDown = False
buttonPressed = None

# https://docs.circuitpython.org/projects/hid/en/latest/_modules/adafruit_hid/keycode.html
# https://learn.adafruit.com/circuitpython-essentials/circuitpython-hid-keyboard-and-mouse
shift_key = Keycode.SHIFT
control_key = Keycode.CONTROL
escape_key = Keycode.ESCAPE
up_arrow = Keycode.UP_ARROW
down_arrow = Keycode.DOWN_ARROW
left_arrow = Keycode.LEFT_ARROW
right_arrow = Keycode.RIGHT_ARROW

while True:
    dButtonA.update()
    dButtonB.update()
    dButtonC.update()
    if not buttonDown and (not dButtonA.value or not dButtonB.value or not dButtonC.value):
        #print("DOWN")
        #time.sleep(1)
        buttonDown = True
        if not dButtonA.value:
            #print("A DOWN")
            buttonPressed = "A"
        elif not dButtonB.value:
            #print("B DOWN")
            buttonPressed = "B"
        elif not dButtonC.value:
            #print("C DOWN")
            buttonPressed = "C"
    else:
        #print("UP")
        if buttonPressed == "A" and dButtonA.value:
            #print("A UP")
            buttonDown = False
        elif buttonPressed == "B" and dButtonB.value:
            #print("B UP")
            buttonDown = False
        elif buttonPressed == "C" and dButtonC.value:
            #print("C UP")
            buttonDown = False

    if not buttonDown and buttonPressed is not None:
        keyboard.write(buttonPressed)
        #kbd.press(right_arrow)
        #time.sleep(.05)
        #kbd.release_all()
        #time.sleep(5)
        # keyboard.press(control_key, key)  #
        #led.value = 1
        #keyboard.write('z')
        #time.sleep(.01)
        #keyboard.write('y')
        #time.sleep(.05)
        buttonPressed = None
        
"""
Test Area
"""
