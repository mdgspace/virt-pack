#!/bin/sh
pkgname=$(echo $PWD | sed 's/\///g')
xq $pkgname
