#!/bin/sh -e

# install script for pi
sudo apt-get install update
sudo apt-get install -y upgrade
sudo rpi-update

sudo apt-get install git

echo "install latest version of wiring pi"
