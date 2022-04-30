#!/bin/bash
result=$(zenity --title="Launch Options" --list "Start Game" "Edit Creamlinux settings" --column="Launch Options")
# Supported games
GAME_NAMES=("Hearts Of Iron IV" "Europa Universalis IV" "Cities: Skylines" "Stellaris" "PDX Launcher (don't click play)")
GAME_BINARIES=("./hoi4" "./eu4" "./Cities.x64" "./stellaris" "./dowser")
function launch_game {
  # LD_PRELOAD doesn't support spaces, do this instead
  cp "$PWD/libCreamlinux.so" /tmp/libCreamlinux.so
  # reset backwards compatible stuff so games that DO use it don't break
  unset IFS
  # paradox launcher segfaults with creamlinux, if launching launcher, do it differently
  if [ "$SELECTED_GAME" = "./dowser" ]; then
      $SELECTED_GAME "$@"
      exit 0
  fi
  # actually launch the game
  LD_PRELOAD="$LD_PRELOAD /tmp/libCreamlinux.so" $SELECTED_GAME "$@"
}
# ifs is required because zenity fucks up without it
export IFS=""

if [ -z "$CREAM_GAME_NAME" ]; then
  echo ""
else
  SELECTED_GAME=$CREAM_GAME_NAME
  launch_game
fi

case $result in

  "Start Game")
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
      # unset ifs before we quit
      unset IFS
      exit 1
    else
     launch_game
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

