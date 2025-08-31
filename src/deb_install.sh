#!/bin/bash
set -e
if [ $# -lt 2 ]; then
    echo "Usage: $0 <virtual-package-name> <dependency1> [dependency2 ...]"
    exit 1
fi
pkgname="$1"
#might have to do something about the directory currently uses current directory
#remove the first argument
shift
# Validate package name (only allow letters, digits, dashes, and underscores)
if [[ ! "$pkgname" =~ ^[a-z0-9][a-z0-9+.-]*$ ]]; then
    echo "Error: Invalid package name '$pkgname'"
    exit 1
fi
depends="$(printf '%s, ' "$@")"
depends="${depends%, }"
version="1.0"
#temporary directory for staging
tmpdir=$(mktemp -d)
mkdir -p "$tmpdir/DEBIAN"
cat> "$tmpdir/DEBIAN/control" <<EOF
Package: $pkgname
Version: $version
Section: misc
Priority: optional
Architecture: all
Depends: $depends
Maintainer: You <you@example.com>
Description: Virtual package $pkgname
EOF

debfile="${pkgname}_${version}_all.deb"
if ! dpkg-deb --build "$tmpdir" "$debfile"; then
    echo "Error: Failed to build .deb package"
    rm -rf "$tmpdir"
    exit 1
fi

# Install package
if ! sudo apt install -y "./$debfile"; then
    echo "Error: Failed to install .deb package"
    rm -rf "$tmpdir"
    exit 1
fi
rm -rf "$tmpdir"

# save package info to project local .virt-pack
{
    echo "Package: $pkgname"
    echo "Version: $version"
    echo "Depends: $depends"
} > .virt-pack

echo 
echo "Package built: $pkgname"
echo "Depends: $depends"
echo "Staging directory: $tmpdir"