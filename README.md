# Creamlinux
CreamApi(dlc unlocker) for Linux.

## Support
This should work for any linux native steam title that doesn't block LD_PRELOAD. Not all games will be visible in the launcher. 
## Tested games
 - Hearts Of Iron IV
 - Stellaris 

## Usage
First, clone the project recursively:
```
git clone https://github.com/20PercentRendered/creamlinux --recursive
```
1. build the project like any standard CMake project:
```
mkdir build
cd build
cmake ..
make
```

2. Copy `build/lib` folder contents to the game folder.
3. Then set the game's steam launch params to `./cream.sh %command%`.
4. If you copied an existing `cream_api.ini`, make sure the line endings are set to linux line endings, otherwise this will not work (just open it and save it again)

When the dialog pops up, it will look a bit strange due to steam's bundled deps being ancient (gtk2 and zenity).

If you want to load `cream_api.ini` from a separate path, specify the path with `CREAM_CONFIG_PATH` in the launch options.

## Credits
[pulzed](https://github.com/pulzed) for [mINI](https://github.com/pulzed/mINI)(ini.h)

[Valve](https://www.valvesoftware.com/) for [steamworks](https://partner.steamgames.com/)

[gabime](https://github.com/gabime) for [spdlog](https://github.com/gabime/spdlog)

[goddeysfreya](https://github.com/goddessfreya) for [hookey](https://github.com/goddessfreya/hookey)
