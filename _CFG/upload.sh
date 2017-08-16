#!/usr/bin/env bash

# Pass binary file to upload to this script
BIN_FILE=$1

# Validating file
if [ ! -f "$BIN_FILE" ]; then
	echo "Binary not found: $BIN_FILE"
	exit
fi

# Uploading file to STM32 Board
st-flash write $BIN_FILE '0x8000000'

# Board reset to make sure program is executed
st-info --reset
