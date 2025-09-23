#!/bin/sh

XBPS_DIR=~/.local/share/virt-pack/xbps

if [ ! -d $XBPS_DIR ]; then
    echo "Bootstrapping xbps, one time process only"
    /usr/local/share/virt-pack/scripts/xbps-bootstrap.sh
fi

pkgname=$(echo $PWD | sed 's/\//_/g')
echo "currently at $PWD"

cd $XBPS_DIR

echo "currently in $PWD"

# pkgname="$1"

# shift
PKG_DIR="$XBPS_DIR/srcpkgs/$pkgname"
echo "made dir $PKG_DIR"

mkdir -p "$PKG_DIR"

echo "resolving libs, in case of multiple choose one"

pkgs=$(xlocate $1.pc | fzf -1 | cut -d' ' -f1)
shift

for arg in "$@"; do
    pkgs="$pkgs $(xlocate $arg.pc | fzf -1 | cut -d' ' -f1)"
done

cat > "$PKG_DIR/template" <<EOF
pkgname="$pkgname"
version=1.0
revision=1
metapackage=yes
short_desc="virt-pkg for $pkgs"
homepage="https://voidlinux.org/"
maintainer="you@example.com"
license="public-domain"
depends="$pkgs"
provides="$pkgname-1.0_1"
EOF

echo "pwd is $PWD"

./xbps-src pkg $pkgname

echo "(*) Installing the virtual package"

xi -y -R $XBPS_DIR/hostdir/binpkgs $pkgname && echo "(*) Installation succeeded"
