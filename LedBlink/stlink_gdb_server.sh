#!/usr/bin/env bash

if [ -z ${STLINK_HOME} ]; then
    echo "STLINK_HOME is not set";
    exit 1;
fi

${STLINK_HOME}/st-util -p2331