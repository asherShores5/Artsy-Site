#!/bin/bash

# Update package list and install dependencies
apt update
apt install -y make flex bison wabt build-essential gcc

# Change permissions for the specified directory
chown -R www-data:www-data /home/site/wwwroot/src
chmod -R 755 /home/site/wwwroot/src