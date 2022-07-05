#!/bin/bash

BIN_NAME=ujagagafs
ROOT_DIR=serve_root
PORT=8000

echo "Deploying app in current folder: $PWD"

## Create root folder to serve
sudo mkdir ${ROOT_DIR}
sudo chmod 777 ${ROOT_DIR}

## Disable any remaining service
sudo systemctl disable ${BIN_NAME}.service
sudo rm /etc/systemd/system/${BIN_NAME}.service

## Build the binary
/bin/bash build.sh ${BIN_NAME}

## Create file server service 
service_file="
[Unit]
Description=File server
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=10
StartLimitBurst=2
StartLimitIntervalSec=30
ExecStart=$PWD/${BIN_NAME} -d ${ROOT_DIR} -p ${PORT}

[Install]
WantedBy=multi-user.target
"
echo $service_file > /tmp/${BIN_NAME}.service
sudo mv /tmp/${BIN_NAME}.service /etc/systemd/system/

## Run the service 
sudo systemctl enable ${BIN_NAME}.service
sudo systemctl start ${BIN_NAME}.service
