# Boot2vWii Channel

Boots any vWii channel and skips all Wiimote prompts! This is meant to be compiled manually and used with Aroma so you
have "custom channels" that each boot different titles.

## How to use

1. Install the current versions of devkitPPC, wut and wut-tools
2. Run `make` to compile the standard title that boots the Wii Menu
3. Copy the resulting `wuhb` to `SD://wiiu/apps/`
4. Run `make clean` to delete the generated files before you build another channel!

## Booting other channels

Just specify them as compile time options after `make`! Supported parameters are:

- `APP_NAME`: Name of the title in the Wii U Menu (defaults to `Boot2vWii`)
- `APP_SHORTNAME`: Short app name, if needed
- `APP_AUTHOR`: Homebrew author
- `ICON`: Path to a 128x128 PNG used as Wii U Menu icon
- `TV_SPLASH`: Path to a 1280x720 PNG used as the TV's splash screen
- `DRC_SPLASH`: Path to a 854x480 PNG used as the Wii U GamePad's splash screen
- `TID`: Title ID of the channel that you want to boot (defaults to `0`, the Wii Menu)
- `DISPLAY_ON`: Display mode of the booted vWii channel. Supported modes are: `TV`, `DRC` (GamePad) or `BOTH` (defaults
  to `BOTH`). Note that it falls back to the GamePad if no TV is connected.
- `FORCERES`: Forces resolution when booting a vWii channel. Supported modes are: `NONE`, `P720` or `P480` (defaults
  to `NONE`). Note that it only works with HDMI.

### Examples

**Wii Menu**:

    make TID=0x0001000144574641 APP_NAME="Wii Menu"

---

**WiiFlow** (DWFA):

    make TID=0x0001000144574641 APP_NAME="WiiFlow" APP_AUTHOR="Fledge68" ICON="assets/wiiflow.png"

---

**USB Loader GX** (UNEO):

    make TID=0x00010001554e454f APP_NAME="USB Loader GX" APP_AUTHOR="USB Loader GX Team" ICON="assets/usb-loader-gx.png"

---

**Nintendont** (WWND), but only on the TV:

    make TID=0x0001000157574e44 APP_NAME="Nintendont" APP_AUTHOR="FIX94" ICON="assets/nintendont.png" DISPLAY_ON=TV
