import board
from digitalio import DigitalInOut, Direction, Pull

kbd_mode_select = DigitalInOut(board.GP5)
kbd_mode_select.direction = Direction.INPUT
kbd_mode_select.pull = Pull.UP

gamepad_mode_select = DigitalInOut(board.GP6)
gamepad_mode_select.direction = Direction.INPUT
gamepad_mode_select.pull = Pull.UP

kbd_mode = not kbd_mode_select.value
gamepad_mode = not gamepad_mode_select.value

if kbd_mode:
    print("KBD")
elif gamepad_mode:
    print("GAMEPAD")
elif kbd_mode and gamepad_mode:
    print("both?!")
elif not kbd_mode and not gamepad_mode:
    print("neither?!")

