#!/bin/bash

## Create root folder to serve
sudo mkdir serve_root
sudo chmod 777 serve_root

## Disable any remaining service
sudo systemctl disable mongoosefs.service
sudo rm /etc/systemd/system/mongoosefs.service

## Build the binary
/bin/bash build.sh

## Create and run the new file server service 
sudo cp mongoosefs.service /etc/systemd/system/
sudo systemctl enable mongoosefs.service
sudo systemctl start mongoosefs.service
