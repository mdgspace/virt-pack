#!/bin/sh
pkgname=$(echo $PWD | sed 's/\//_/g')
XBPS_HOSTDIR=~/.local/share/virt-pack/xbps/hostdir xq $pkgname
