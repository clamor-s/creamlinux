#!/bin/bash
result=$(zenity --title="Launch Options" --list "Start Game" "Edit Creamlinux settings" --column="Launch Options")
# Supported games
GAME_NAMES=("Hearts Of Iron IV" "Europa Universalis IV" "Cities: Skylines" "Paradox Launcher (Don't launch game here)")
GAME_BINARIES=("./hoi4" "./eu4" "./Cities.x64" "./dowser")
# stupid fix for stupid "backwards compatability" reasons
# seriously i spent fucking 1h and 30mins figuring this shit out
# really, backwards compatibility is stupid and breaks stuff most of the time (windows is a good example)
# word splitting is one of those cases
# who has any genuine use for word splitting????
export IFS=""
case $result in

  "Start Game")
    # LD_PRELOAD doesn't support spaces, do this instead
    cp "$PWD/libCreamlinux.so" /tmp/libCreamlinux.so
    FOUND_BINS=()
    INDEX=0
    INDEX_ADDED=0
    MANGLED_OUTPUT=()

    for GAMEBIN in "${GAME_BINARIES[@]}"
    do
      if [ -f "$GAMEBIN" ]; then
        echo "$GAMEBIN exists."
        FOUND_BINS+=("$GAMEBIN")
        MANGLED_OUTPUT+=("$INDEX_ADDED")
        MANGLED_OUTPUT+=("\"${GAME_NAMES[$INDEX]}\"")
        MANGLED_OUTPUT+=("\"${GAME_BINARIES[$INDEX]}\"")
        ((INDEX_ADDED++))
      fi
      ((INDEX++))
      # do whatever on "$bin" here

    done
    if [ "${#FOUND_BINS[@]}" -eq "0" ]; then
       echo "No binaries found."
       zenity --error --text="No games were found in script directory."
       exit 1
    fi
    if [ "${#FOUND_BINS[@]}" -eq "1" ]; then
      echo "Found one binary, running"
      SELECTED_GAME="${FOUND_BINS[0]}"
    else
      echo "Found multiple binaries, triggering prompt"
      SELECTED_INDEX=$(zenity --list --column="I" --column="Game Name" --column="Binary Path" --title="Launch Options" --text="Multiple game binaries were found. Please pick the game you want to run." ${MANGLED_OUTPUT[@]} )
      SELECTED_GAME=${FOUND_BINS[$SELECTED_INDEX]}
    fi
    # is the game variable set
    if [ -z "$SELECTED_INDEX" ]; then
      zenity --error --text="Cancelled: No game selected"
      # unset ifs so we don't fuck up every other program
      unset IFS
      exit 1
    else
     # reset stupid backwards compatible stuff so games that DO use it don't break
     unset IFS
     # paradox launcher segfaults with creamlinux, if launching launcher, do it differently
     if [ "$SELECTED_GAME" -eq "./dowser" ]; then
         $SELECTED_GAME "$@"
         exit 0
     fi
     # actually launch the game
     LD_PRELOAD="$LD_PRELOAD /tmp/libCreamlinux.so" $SELECTED_GAME "$@"
    fi
    ;;

  "Edit Creamlinux settings")
    # is the cream_api.ini path set
    if [ -z "$CREAM_CONFIG_PATH" ]; then
      # no path was set
      xdg-open $PWD/cream_api.ini
    else
      # custom path was set
      xdg-open $CREAM_CONFIG_PATH
    fi
    # restart the script
    $PWD/cream.sh
    ;;

  *)

    ;;
esac

