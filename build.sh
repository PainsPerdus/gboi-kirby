#!/bin/bash

# Note: we are well aware that this build system, if it can even be called that,
# is hideous, but we had no time to care.

git submodule update --init

cd gbdk-2020
make

cd ../main-rom
make clean
make assets
make || true
make

cp pod.gb ../
