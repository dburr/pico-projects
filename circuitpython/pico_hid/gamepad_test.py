import board
from digitalio import DigitalInOut, Direction, Pull
import time
from adafruit_debouncer import Debouncer
from hid_gamepad import Gamepad
import usb_hid

gp = Gamepad(usb_hid.devices)

up = DigitalInOut(board.GP0)
up.direction = Direction.INPUT
up.pull = Pull.UP
dUp = Debouncer(up)

down = DigitalInOut(board.GP1)
down.direction = Direction.INPUT
down.pull = Pull.UP
dDown = Debouncer(down)

left = DigitalInOut(board.GP2)
left.direction = Direction.INPUT
left.pull = Pull.UP
dLeft = Debouncer(left)

right = DigitalInOut(board.GP3)
right.direction = Direction.INPUT
right.pull = Pull.UP
dRight = Debouncer(right)

fire = DigitalInOut(board.GP4)
fire.direction = Direction.INPUT
fire.pull = Pull.UP
dFire = Debouncer(fire)

while True:
    dUp.update()
    dDown.update()
    dLeft.update()
    dRight.update()
    dFire.update()
    
    if not dUp.value:
        print("UP")
        gp.press_buttons(1)
    else:
        gp.release_buttons(1)
    if not dDown.value:
        print("DOWN")
        gp.press_buttons(2)
    else:
        gp.release_buttons(2)
    if not dLeft.value:
        print("LEFT")
        gp.press_buttons(3)
    else:
        gp.release_buttons(3)
    if not dRight.value:
        print("RIGHT")
        gp.press_buttons(4)
    else:
        gp.release_buttons(4)
    if not dFire.value:
        print("BUTTON")
        gp.press_buttons(5)
    else:
        gp.release_buttons(5)

    time.sleep(0.1)