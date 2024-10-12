
my keebio iris rev8 changelog
- `keyboard.json` added `vid 0x4447 which is hexadecimal for DG` jk didn't actually do this

TAP_DANCE_ENABLE = yes

try out
AUTO_SHIFT_ENABLE = yes
AUTOCORRECT_ENABLE = yes (with a library)
COMBO_ENABLE = yes (oooo combos)
EEPROM for persistent settings
ONESHOT key for switching layers?
RAW HID for programming practice
try layer? maybe for "locking away" the fourth section



underglow docs (addressable RGB https://docs.qmk.fm/features/rgblight)
perkey rgb docs (rgb matrix https://docs.qmk.fm/features/rgb_matrix)




(flashing the bootloader)[https://docs.keeb.io/flashing-firmware#rp2040-board-uf2-bootloader]
1 Enter bootloader mode (use one of the following methods):
  - Double-tap the Reset button
  - Hold the Reset button for at least 1 second and then let go
2 Wait for OS to detect the board in bootloader mode
  - It will show up as a USB mass-storage device named RPI-RP2
3 Copy the .uf2 file to the RPI-RP2 USB drive

just put in bootloader mode with the reset button on the back of the keyboard
then run `qmk flash -kb <my_keyboard> -km <my_keymap>`
make sure to program each half