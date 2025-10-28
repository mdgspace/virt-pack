#!/bin/sh
dir=$(echo $PWD | sed 's/\//_/g')
rm events.json packages.txt
sudo xbps-remove -R $dir
