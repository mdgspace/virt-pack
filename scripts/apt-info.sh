#!/bin/sh
pkgname=$(echo $PWD | sed 's/\///g')
dpkg -s $pkgname
