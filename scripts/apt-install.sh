#!/bin/bash
pkgname=$(echo $PWD | sed 's/\//_/g')
# if [[ ! "$pkgname" =~ ^[a-z0-9][a-z0-9+.-]*$ ]]; then
#     echo "Error: Invalid package name '$pkgname'"
#     exit 1
# fi

pkgs=$(xlocate $1.pc | fzf -1 | cut -d' ' -f1)
echo $pkgs
shift

for arg in "$@"; do
    pkgs="$pkgs, $(xlocate $arg.pc | fzf -1 | cut -d' ' -f1)"
    echo $pkgs
done

# depends="$(printf '%s, ' "$@")"
# depends="${depends%, }"
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

echo 
echo "Package built: $pkgname"
echo "Depends: $depends"
echo "Staging directory: $tmpdir"
