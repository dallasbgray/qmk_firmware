
keebio iris rev8 rgb led index locations (based on keebio/rev8/keyboard.json)

debug with the `qmk console` command
`qmk doctor` can help environment issues
`qmk migrate` searches for legacy code that can be converted to the `info.json` format

Compiling
`qmk compile -kb <my_keyboard> -km <my_keymap>`, although `qmk flash` also compiles the code
`qmk clean` to clean build folders

(flashing the bootloader)[https://docs.keeb.io/flashing-firmware#rp2040-board-uf2-bootloader]
1 Enter bootloader mode (use one of the following methods):
  - Double-tap the Reset button
  - Hold the Reset button for at least 1 second and then let go
2 Wait for OS to detect the board in bootloader mode
  - It will show up as a USB mass-storage device named RPI-RP2
3 Either:
  - Copy the .uf2 file to the RPI-RP2 USB drive
  - Run `qmk flash -kb <my_keyboard> -km <my_keymap>`
 - make sure to flash each half if a split keyboard


Personal changelog for `keyboard.json` under the rev8 folder
- set all rgb effects to `false`
- `nkro` and `force_nkro` set to `true`
- enabled a couple rgb effects
- added `"layer_state": true`
