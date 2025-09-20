#!/bin/bash
INSTALL_PREFIX=${1:-/usr/local}
mkdir -p BUILD
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX -B BUILD -S . -DBUILD_EXAMPLE=OFF
sudo cmake --build BUILD --target uninstall
sudo cmake --build BUILD --config Release --target install
echo "WeaChart library installation done!"
