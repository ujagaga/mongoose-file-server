#!/bin/bash

BIN_NAME="${1:-ujagagafs}"

echo "Building binary: ${BIN_NAME}"
gcc web_file_server.c mongoose.c -g -W -Wall -Wno-unused-function -DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK -pthread -Wno-format-truncation -o ${BIN_NAME}
