import digitalio
import usb_hid
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS
from adafruit_hid.gamepad import Gamepad
from adafruit_hid.keycode import Keycode
from time import sleep
import board
            
gamepad = Gamepad(usb_hid.devices)

button_status = {'up' : 0, 'down' : 0, 'left' : 0, 'right' : 0, 'fire' : 0}
buttons = {0 : 'up', 1 : 'down', 2 : 'left', 3 : 'right', 4 : 'fire'}

keyboard_buttons = {'up' : Keycode.UP_ARROW, 'down' : Keycode.DOWN_ARROW, 'left' : Keycode.LEFT_ARROW, 'right' : Keycode.RIGHT_ARROW,
                  'fire' : Keycode.SPACE}

mode = 1 # 1 for Keyboard Emulation, 2 for Joystick
latch = digitalio.DigitalInOut(board.GP5)
clock = digitalio.DigitalInOut(board.GP4)
data = digitalio.DigitalInOut(board.GP6)

latch.direction = digitalio.Direction.OUTPUT
clock.direction = digitalio.Direction.OUTPUT
data.direction = digitalio.Direction.INPUT

data.pull = digitalio.Pull.UP

keyboard = Keyboard(usb_hid.devices)
layout = KeyboardLayoutUS(keyboard)
latch.value = False
clock.value = False
previousState = False

delaytime = 0.0001

while True:
    latch.value = True
    sleep(delaytime)
    latch.value = False
    sleep(delaytime)
    button_status[buttons[0]] = data.value
    for x in range(0, 7, 1):
        clock.value = True
        sleep(delaytime)
        clock.value = False
        sleep(delaytime)
        button_status[buttons[x + 1]] = data.value
    if button_status['select'] == False and button_status['start'] == False and button_status['up'] == False:
        if mode == 2:
            mode = 1
        else:
            mode = 2
        keyboard.release_all()
        print(mode)
        sleep(0.3)  
    press_buttons = []
    release_buttons = []
    for x in buttons:
        if mode == 2:
            if button_status[buttons[x]] == True:
                gamepad.release_buttons(x + 1)
            else:
                gamepad.press_buttons(x + 1)
        else:
            if button_status[buttons[x]] == True:
                release_buttons.append(keyboard_buttons[buttons[x]])
            else:
                press_buttons.append(keyboard_buttons[buttons[x]])
    
    if mode == 1:
        keyboard.press(*press_buttons)
        keyboard.release(*release_buttons)
