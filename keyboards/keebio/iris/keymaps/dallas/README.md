
keebio iris rev8 rgb led index locations (based on keebio/rev8/keyboard.json)
             1 (BackLight)     4 (BL)            7 (BL)                                   41 (BL)           38 (BL)           35 (BL) 
    ┌────────┬────────┬────────┬────────┬────────┬────────┐                       ┌────────┬────────┬────────┬────────┬────────┬────────┐
        0    ,   2    ,   3    ,   5    ,   6    ,   8    ,                           42   ,   40   ,   39   ,   37   ,   36   ,   34   ,
    ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
        14   ,   13   ,   12   ,   11   ,   10   ,   9    ,                           43   ,   44   ,   45   ,   46   ,   47   ,   48   ,
    ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
        15   ,   16   ,   17   ,   18   ,   19   ,   20   ,                           54   ,   53   ,   52   ,   51   ,   50   ,   49   ,
    ├────────┼────────┼────────┼────────┼────────┼────────┼────────┐     ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
        28   ,   26   ,   25   ,   23   ,   22   ,   21   ,   29   ,         67   ,   55   ,   56   ,   57   ,   59   ,   60   ,   62   ,
    └────────┴────────┴────────┴────┬───┴────┬───┴────┬───┴────┬───┘     └────┬───┴────┬───┴────┬───┴────┬───┴────────┴────────┴────────┘
             27 (BL)          24 (BL)   33   ,   32 ,     30   ,                  66   ,   64   ,   63       58 (BL)           61 (BL)
                                    └────────┴────────┴────────┘              └────────┴────────┴────────┘
                                                     31 (BL)                          65 (BL) 
look into "super alt tab" https://docs.qmk.fm/feature_macros#tap-code-kc

debug with the `qmk console` command
`qmk doctor` can help environment issues
`qmk migrate` searches for legacy code that can be converted to the `info.json` format

matrix scan frequency with all rgb code off is ~1430, 
1rgb effect is 1410, 
1320 1 rgb effect & layer checks & rgb overrides
1500 after fixing bug in the rgb overrides. Drops to 1480 with the rgb overrides

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
