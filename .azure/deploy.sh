#!/bin/bash

# Update package list and install dependencies
sudo apt-get update
sudo apt-get install -y make flex bison wabt build-essential gcc

# Change permissions for the specified directory
sudo chown -R www-data:www-data /home/site/wwwroot/src
sudo chmod -R 755 /home/site/wwwroot/src