#!/bin/bash
dir=$(echo $PWD | sed 's/\///g')
dir=${dir,,}
sudo apt remove $dir
sudo apt autoremove

