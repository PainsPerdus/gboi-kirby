#!/bin/bash

# YES, this is awful, and this point we have no time to care.
# If someone wants to fix the Makefile, be my guest...


git submodule update --init

cd gbdk-2020
make

cd ../main-rom
make clean
make assets
make || true
make
