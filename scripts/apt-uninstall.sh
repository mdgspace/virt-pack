#!/bin/bash
dir=$(echo $PWD | sed 's/\///g')
dir=${dir,,}
rm events.json packages.txt
sudo apt remove $dir
sudo apt autoremove
