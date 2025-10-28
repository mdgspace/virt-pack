#!/bin/bash
pkgname=$(echo $PWD | sed 's/\///g')
# if [[ ! "$pkgname" =~ ^[a-z0-9][a-z0-9+.-]*$ ]]; then
#     echo "Error: Invalid package name '$pkgname'"
#     exit 1
# fi

# Read pkg-config packages into array from packages.txt
if [ ! -f packages.txt ]; then
    echo "Error: packages.txt not found. Run bear-intercept.sh first."
    exit 1
fi

mapfile -t pkg_list < packages.txt

echo "Found packages: ${pkg_list[@]}"

# Process first package
pkgs=$(apt-file find "${pkg_list[0]}.pc" | fzf -1 | cut -d':' -f1)

# Process remaining packages
for pkg in "${pkg_list[@]:1}"; do
    pkgs="$pkgs, $(apt-file find $pkg.pc | fzf -1 | cut -d':' -f1)"
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
Depends: $pkgs
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
echo "Depends: $pkgs"
echo "Staging directory: $tmpdir"
