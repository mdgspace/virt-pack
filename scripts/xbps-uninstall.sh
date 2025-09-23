#!/bin/sh
dir=$(echo $PWD | sed 's/\//_/g')
sudo xbps-remove -R $dir
