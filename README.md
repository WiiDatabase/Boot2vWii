# Boot2vWii Channel

Boots any vWii channel and skips all Wiimote prompts! This is meant to be compiled manually and used with Aroma so you
have "custom channels" that each boot different titles.

## How to use

1. Install the current versions of devkitPPC, wut and wut-tools
2. Run `make` to compile the standard title that boots the "ConnectMii" channel from RiiConnect24
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
- `TIDHIGH`: First 8 chars of the title ID of the channel that you want to boot (defaults to `0x00010001`)
- `TIDLOW`: Last 8 chars of the title ID of the channel that you want to boot (defaults to `0x44434f41`, which is the
  ConnectMii channel)
- `DISPLAY`: Display mode of the booted vWii channel. Supported modes are: `TV`, `DRC` (GamePad) or `BOTH` (defaults
  to `BOTH`). Note that it falls back to the GamePad if no TV is connected.
- `FORCERES`: Forces resolution when booting a vWii channel. Supported modes are: `NONE`, `P720` or `P480` (defaults
  to `NONE`). Note that it only works with HDMI.

### Examples

**WiiFlow** (DWFA):

    make TIDLOW=0x44574641 APP_NAME="WiiFlow" AUTHOR="Fledge68" ICON="assets/wiiflow.png"

---

**USB Loader GX** (UNEO):

    make TIDLOW=0x554e454f APP_NAME="USB Loader GX" AUTHOR="USB Loader GX Team" ICON="assets/usb-loader-gx.png"

---

**Nintendont** (WWND), but only on the TV:

    make TIDLOW=0x57574e44 APP_NAME="Nintendont" AUTHOR="FIX94" ICON="assets/nintendont.png" DISPLAY=TV
