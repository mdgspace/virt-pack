#!/bin/sh
pkgname=$(echo $PWD | sed 's/\///g')
apt show $pkgname
