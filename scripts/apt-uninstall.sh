#/bin/sh
dir=$(echo $PWD | sed 's/\//_/g')
sudo apt remove $dir
sudo apt autoremove

