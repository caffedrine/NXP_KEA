#!/usr/bin/env bash

# Pass .elf file to this script
BIN_FILE=$1

# Check if passed file is valid
if [ ! -f "$BIN_FILE" ]; then
	echo "ELF file not found: $BIN_FILE"
	exit
fi 

# Build command
COMMAND='arm-none-eabi-gdb --eval-command="target remote localhost:4242" --tui'" $BIN_FILE";

# Launch GDB TUI into a new terminal
gnome-terminal -e "$COMMAND"
