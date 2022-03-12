# Creamlinux
CreamApi for Linux.

## BIG NOTICE PLEASE READ
This has primarily been made for my personal use, however I have decided to open source it. A lot of the code is based on [hookey](https://github.com/goddessfreya/hookey). One of the goals of this project was being able to play internet multiplayer with dlcs. The code is pretty janky, but it works. This should work for most games, but you'll need to LD_PRELOAD manually, or add the game binary to the supported games array. Maybe don't try it on VAC-secure games though. Also, keep in mind this was written with HOI4 in mind. THIS WILL NOT WORK THROUGH WINE/PROTON

## Usage
First, build the project with `sh build.sh`.

Then, copy build/lib folder contents to the game folder. Then set steam launch params: `./cream.sh %command%`

This will open a dialog that allows you to launch the game and edit cream_api.ini.

## Credits
mINI for ini.h

Valve for steamworks
