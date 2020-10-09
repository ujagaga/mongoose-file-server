#!/bin/bash

# Helper script to also display the IP address before the file server starts.
# call from thunar using:
# exo-open --working-directory %f --launch TerminalEmulator /home/rada/Applications/share-files.sh

# To use on your computer just change the WIFI name accordingly. Chack the wifi card name by running "ip a"
WIFI_CARD_NAME="wlx60e327174a87"
CALL_DIR=$PWD
SCRIPT_RELATIVE_PATH=$(dirname "$0")

#ip="$(ip a | grep -A 1 'wlp1s0' | tail -1)"
myip="$(ip a | grep -A 1 $WIFI_CARD_NAME | grep -A 1 'inet' | grep -oE '\b([0-9]{1,3}\.){3}[0-9]{1,3}\b')"
firstip="$(echo $myip | cut -d ' ' -f 1)"

echo
echo "Sharing files from: $CALL_DIR" 
echo "IP $firstip"

./mongoose-fs $CALL_DIR



