# Creamlinux
CreamApi clone for Linux.

## Support
This should work for any linux native steam title that doesn't block LD_PRELOAD. Not all games will however be visible in the popup. Read [Advanced](https://github.com/20PercentRendered/creamlinux#advanced) if this is the case.  

## Tested games
 - Hearts Of Iron IV
 - Stellaris 
 - PAYDAY 2
 
## Usage 
1. Download the [latest](https://github.com/20PercentRendered/creamlinux/releases/latest/download/creamlinux.zip) release of Creamlinux.
2. Unzip it and copy the files to the game's directory
3. Set the game's steam launch params to `sh ./cream.sh %command%`.
4. If you have your own cream_api.ini, copy the DLC lines to Creamlinux's cream_api.ini in the game directory. 
5. Launch your game and have fun! 

## Building from source
1. Clone the project recursively:
```
git clone https://github.com/20PercentRendered/creamlinux --recursive
```
2. build the project like any standard CMake project:
```
mkdir build
cd build
cmake ..
make
```

3. Copy `build/lib` folder contents to the game folder.
4. Then set the game's steam launch params to `sh ./cream.sh %command%`.
5. If you have your own cream_api.ini, copy the DLC lines to Creamlinux's cream_api.ini in the game directory. 
## Advanced 

If you want to load `cream_api.ini` from a specific path, specify the path with `CREAM_CONFIG_PATH` in the launch options.

If the game you want to test isn't supported, use `CREAM_GAME_NAME` to specify the game executable's name.

## Credits
[pulzed](https://github.com/pulzed) for [mINI](https://github.com/pulzed/mINI)(ini.h)

[Valve](https://www.valvesoftware.com/) for [steamworks](https://partner.steamgames.com/)

[gabime](https://github.com/gabime) for [spdlog](https://github.com/gabime/spdlog)

[goddeysfreya](https://github.com/goddessfreya) for [hookey](https://github.com/goddessfreya/hookey)
