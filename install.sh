#!/bin/sh -e

# install script for pi
sudo apt-get install -y build-essential
sudo apt-get install -y gpsd gpsd-clients
cd wiringPi/
./build
